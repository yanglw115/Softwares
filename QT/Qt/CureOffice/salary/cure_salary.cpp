#include <QFileDialog>
#include <QtDebug>
#include <QMessageBox>
#include <QHeaderView>
#include <QFile>
#include <QValidator>
#include <QDate>

#include "cure_salary.h"
#include "msvs_charset.h"
#include "hanz2pinyin/Hanz2Piny.h"
#include "smtpClient/SmtpMime.h"
#include "cure_tableheaderview.h"
#include "cure_global.h"

using namespace std;

static const QString g_strOpenExcel("打开文件");
static const QString g_strCloseExcel("关闭");
static const QString g_strDefaultSMTPServer("smtp.ym.163.com");
static const QString g_strDefautlSMTPPort("465");

static const QString g_strSalarySendTemplate = "SalaryEmailTemplate.xlsx";
static const QString g_strSalaryEmailExcel = "SalaryEmailAddresses.xlsx";
/* 数据模板真正员工数据的起始行 */
static const int g_nStartRow = 5;
/* 姓名所在邮件发送模块中的索引 */
static const int g_nIndexName = 2; // from 0
static const int g_nIndexNumber = 3; // from 0 工号

static const QString g_strSenderCheckState[] = {
    "未检测",
    "无效",
    "有效"
};

CureSalary::CureSalary(QWidget *parent)
    : QWidget(parent)
    , m_pLabelOpenExcel(NULL)
    , m_pEditOpenedExcel(NULL)
    , m_pButtonOpenExcel(NULL)
    , m_pTableExcel(NULL)
    , m_pXlsxDoc(NULL)
    , m_pVLayoutMain(NULL)
    , m_pHLayoutOpenExcel(NULL)
    , m_pProgressDialog(NULL)
    , m_bStateOpenExcel(false)
    , m_nStateSenderCheck(State_UnChecked)
    , m_bEmailCanceled(false)
{
    initWidgets();

    /* 以下对TableView设置header的方式不生效，不能够展示自定义的效果，对TableWidget可能是OK的 */
    // HeaderViewCheck *pHeaderView = new HeaderViewCheck(Qt::Vertical, m_pTableExcel);
    // m_pTableExcel->setVerticalHeader(pHeaderView);
    CheckBoxDelegate *pDelegate = new CheckBoxDelegate(this);
    m_pTableExcel->setItemDelegate(pDelegate);

    /* 这里虽然设置了自定义的headerView，但是model的headerData还会生效，只是自定义的headerView会覆盖其设置区域的值 */
    TableHeaderView *pHeaderView = new TableHeaderView(Qt::Horizontal, this);
    m_pTableExcel->setHorizontalHeader(pHeaderView);

    /* 上面使用了自定义水平方向的头，所以下面需要show()一下，否则不显示 */
    m_pTableExcel->horizontalHeader()->show();
}

CureSalary::~CureSalary()
{

}

void CureSalary::slotOpenExcel()
{
    if (!m_bStateOpenExcel) {
        QString strFilePath = QFileDialog::getOpenFileName(this, tr("Excel文件选择"), "Desktop", "*.xlsx");
        if (!strFilePath.isEmpty()) {
            m_pEditOpenedExcel->setText(strFilePath);
            freeXlsxDocument();
            m_pXlsxDoc = new Document(strFilePath);
            if (m_pXlsxDoc) {
                QString strSheetName = m_pXlsxDoc->sheetNames()[0];
                Worksheet *pWorksheet = dynamic_cast<Worksheet*>(m_pXlsxDoc->sheet(strSheetName));
                if (pWorksheet) {
                    SheetModel *pModel = new SheetModel(pWorksheet, m_pTableExcel, g_nStartRow);
                    m_pTableExcel->setModel(pModel);

                    connect(pModel, SIGNAL(sigCheckStateChanged(int)), m_pTableExcel->horizontalHeader(), SLOT(slotCheckStateChanged(int)));
                    connect(m_pTableExcel->horizontalHeader(), SIGNAL(sigCheckStateChanged(int)), pModel, SLOT(slotCheckStateChanged(int)));
                    m_bStateOpenExcel = true;
                    m_pEditFilter->clear();
                    saveSalaryExcelHead(pWorksheet);
                    qDebug() << "Open excel: " << strFilePath << "success!";
                    //qDebug() << "Table row count: " << m_pTableExcel->model()->rowCount() << ", colum count: " << m_pTableExcel->model()->columnCount();
                }
            } else {
                qCritical() << "Open salary xlsx document to table view failed!";
                QMessageBox::critical(this, tr("Excel文件读取"), tr("打开并读取.xlsx文件失败，请确保选择的文件是.xlsx格式！"));
            }
        }
    } else {
        qDebug() << "Close excel.";
        freeXlsxDocument();
        m_bStateOpenExcel = false;
    }

    if (m_bStateOpenExcel) {
        m_pButtonOpenExcel->setText(g_strCloseExcel);
    } else {
        m_pButtonOpenExcel->setText(g_strOpenExcel);
    }
}

void CureSalary::slotDoTableViewFilter(const QString &strText)
{
    if (!m_bStateOpenExcel) {
        return;
    }
    for (int i = 0; i < m_pTableExcel->model()->rowCount(); ++i) {
        bool bShow = false;
        for (int j = 0; j < m_pTableExcel->model()->columnCount(); ++j) {
            QModelIndex modelIndex = m_pTableExcel->model()->index(i, j);
            QString strItem = modelIndex.data().toString();
            if (strItem.contains(strText, Qt::CaseInsensitive)) {
                bShow = true;
                break;
            }

            /* 下面处理汉字转拼音，即过滤输入的是拼音的时候处理 */
            QString strPinyin;
            Hanz2Piny HP;
            vectorPair vectorResult = HP.toPinyinFromUtf8(strItem.toUtf8().toStdString(), false);
            for (uint k = 0; k < vectorResult.size(); ++k) {
                if (vectorResult.size() && vectorResult.at(k).first) {
                    vectorString value = vectorResult.at(k).second;
                    strPinyin.append(QString::fromStdString(value.at(0)));//这里存在多音字的情况，暂时只用第一个
                }
            }
            if (strPinyin.contains(strText, Qt::CaseInsensitive)) {
                bShow = true;
                break;
            }
        }
        if (bShow) {
            m_pTableExcel->showRow(i);
        } else {
            m_pTableExcel->hideRow(i);
        }
    }
}

void CureSalary::slotCheckEmailSenderValid()
{
    m_pGroupSender->setEnabled(false);

    // ssl端口是465，非SSL是25
    SmtpClient email(m_pEditSMTPServer->text(), m_pEditSMTPPort->text().toInt(), SmtpClient::SslConnection);
    email.setUser(m_pEditSenderAddress->text());
    email.setPassword(m_pEditSenderPasswd->text());

    if (email.connectToHost()) {
        if (email.login()) {
            email.quit();
            m_nStateSenderCheck = State_Check_Success;
            m_pLabelSenderValid->setText(g_strSenderCheckState[m_nStateSenderCheck]);
        } else {
            qWarning() << "Email login failed: " << m_pEditSenderAddress->text();
            m_nStateSenderCheck = State_Check_Failed;
            m_pLabelSenderValid->setText(g_strSenderCheckState[m_nStateSenderCheck]);
        }
    } else {
        qWarning() << "Email connect failed: " << m_pEditSMTPServer->text() << "-" << m_pEditSMTPPort->text().toInt();
        m_nStateSenderCheck = State_Check_Failed;
        m_pLabelSenderValid->setText(g_strSenderCheckState[State_Check_Failed]);
    }

    m_pGroupSender->setEnabled(true);
}

void CureSalary::slotEmailSenderDataChanged(const QString &strText)
{
    Q_UNUSED(strText)
    m_nStateSenderCheck = State_UnChecked;
    m_pLabelSenderValid->setText(g_strSenderCheckState[m_nStateSenderCheck]);
}

void CureSalary::slotSendEmail()
{
    if (m_nStateSenderCheck != State_Check_Success) {
        qWarning() << "发件箱不可用或者未检测其可用性.";
        QMessageBox::warning(this, tr("工资条发送"), tr("发件箱不可用或者未检测其可用性,请确认!"));
        return;
    }

    if (!QFile::exists(g_strSalarySendTemplate) || !QFile::exists(g_strSalaryEmailExcel)) {
        QString strInfo = tr("工资数据发送模板不存在或员工邮箱excel表未找到,请确认!");
        qWarning() << strInfo;
        QMessageBox::critical(this, tr("工资条发送"), strInfo);
    }

    if (m_pTableExcel->model() && m_pTableExcel->model()->rowCount() > 0) {
        SheetModel *pModel = dynamic_cast<SheetModel*>(m_pTableExcel->model());
        QVector<int> vectorCheckState = pModel->getCheckStateVector();
        int nChecked = 0;
        for (int i = 0; i < vectorCheckState.size(); ++i) {
            if (vectorCheckState[i] & SALARY_CHECKED) {
                ++nChecked;
            }
        }
        if (nChecked > 0) {
            /* 自定义邮件发送Dialog,展示发送进度及发送结果 */
            initProgressDialog(nChecked);
            m_pProgressDialog->setTotalValue(vectorCheckState.size());
            m_pProgressDialog->setCheckedValue(nChecked);
        } else {
            QMessageBox::warning(this, tr("工资条发送"), tr("没有选择需要发送的记录!"));
        }
    } else {
        QString strInfo = tr("工资信息列表为空,不可发送!");
        qWarning() << strInfo;
        QMessageBox::warning(this, tr("工资条发送"), strInfo);
    }
}

void CureSalary::slotDialogSendStart()
{
    makeAndSendEmailData();
}

void CureSalary::slotDialogSendCancel()
{
    m_bEmailCanceled = true;
}


void CureSalary::initWidgets()
{
    m_pLabelOpenExcel = new QLabel(tr("请打开.xlsx格式的工资表: "), this);
    m_pEditOpenedExcel = new QLineEdit(this);
    m_pEditOpenedExcel->setReadOnly(true);
    m_pButtonOpenExcel = new QPushButton(g_strOpenExcel, this);
    connect(m_pButtonOpenExcel, SIGNAL(clicked(bool)), this, SLOT(slotOpenExcel()));
    m_pTableExcel = new QTableView(this);

     /* email sender config */
    m_pGroupSender = new QGroupBox(tr("发件箱设置"), this);
    m_pLabelSenderAddress = new QLabel(tr("邮箱："), m_pGroupSender);
    m_pLabelSenderPasswd = new QLabel(tr("密码："), m_pGroupSender);
    m_pLabelSMTPServer = new QLabel(tr("SMPT服务器地址："), m_pGroupSender);
    m_pLabelSMTPPort = new QLabel(tr("SMPT端口(SSL)："), m_pGroupSender);
    m_pLabelSenderValid = new QLabel(g_strSenderCheckState[State_UnChecked], m_pGroupSender);

    m_pEditSenderAddress = new QLineEdit(m_pGroupSender);
    m_pEditSenderPasswd = new QLineEdit(m_pGroupSender);
    m_pEditSenderPasswd->setEchoMode(QLineEdit::Password);
    m_pEditSMTPServer = new QLineEdit(m_pGroupSender);
    m_pEditSMTPServer->setText(g_strDefaultSMTPServer);
    m_pEditSMTPPort = new QLineEdit(m_pGroupSender);
    QIntValidator *pValidatorSMTPPort = new QIntValidator(1, 65536, m_pGroupSender);
    m_pEditSMTPPort->setValidator(pValidatorSMTPPort);
    m_pEditSMTPPort->setText(g_strDefautlSMTPPort);
    connect(m_pEditSenderAddress, SIGNAL(textChanged(QString)), this, SLOT(slotEmailSenderDataChanged(QString)));
    connect(m_pEditSenderPasswd, SIGNAL(textChanged(QString)), this, SLOT(slotEmailSenderDataChanged(QString)));
    connect(m_pEditSMTPServer, SIGNAL(textChanged(QString)), this, SLOT(slotEmailSenderDataChanged(QString)));
    connect(m_pEditSMTPPort, SIGNAL(textChanged(QString)), this, SLOT(slotEmailSenderDataChanged(QString)));
    m_pButtonSenderValidCheck = new QPushButton(tr("检测发件箱是否有效"), m_pGroupSender);
    connect(m_pButtonSenderValidCheck, SIGNAL(clicked(bool)), this, SLOT(slotCheckEmailSenderValid()));
    QHBoxLayout *pHBoxEmail = new QHBoxLayout;
    pHBoxEmail->addWidget(m_pLabelSenderAddress);
    pHBoxEmail->addWidget(m_pEditSenderAddress);
    pHBoxEmail->addWidget(m_pLabelSenderPasswd);
    pHBoxEmail->addWidget(m_pEditSenderPasswd);
    pHBoxEmail->addStretch();
    QHBoxLayout *pHBoxEmailServer = new QHBoxLayout;
    pHBoxEmailServer->addWidget(m_pLabelSMTPServer);
    pHBoxEmailServer->addWidget(m_pEditSMTPServer);
    pHBoxEmailServer->addWidget(m_pLabelSMTPPort);
    pHBoxEmailServer->addWidget(m_pEditSMTPPort);
    QHBoxLayout *pHBoxCheckEmailValid = new QHBoxLayout;
//    pHBoxCheckEmailValid->addStretch();
    pHBoxCheckEmailValid->addWidget(m_pButtonSenderValidCheck);
    pHBoxCheckEmailValid->addWidget(m_pLabelSenderValid);
    pHBoxCheckEmailValid->addStretch();
    pHBoxEmailServer->addStretch();

    QVBoxLayout *m_pVLayoutGroupSender = new QVBoxLayout(m_pGroupSender);
    m_pVLayoutGroupSender->addLayout(pHBoxEmail);
    m_pVLayoutGroupSender->addLayout(pHBoxEmailServer);
    m_pVLayoutGroupSender->addLayout(pHBoxCheckEmailValid);

    /* 对QTableView进行过滤 */
    m_pLabelFilter = new QLabel(tr("查询："), this);
    m_pEditFilter = new QLineEdit(this);
    m_pLabelHRName = new QLabel(tr("HR姓名："), this);
    m_pEditHRName = new QLineEdit(this);
    m_pEditHRName->setText(tr("冯丽萍"));
    m_pButtonSendEmail = new QPushButton(tr("发送邮件"), this);
    m_pHLayoutFilter = new QHBoxLayout;
    m_pHLayoutFilter->addWidget(m_pLabelFilter);
    m_pHLayoutFilter->addWidget(m_pEditFilter);
    m_pHLayoutFilter->addSpacing(100);
    m_pHLayoutFilter->addWidget(m_pLabelHRName);
    m_pHLayoutFilter->addWidget(m_pEditHRName);
    m_pHLayoutFilter->addWidget(m_pButtonSendEmail);
    connect(m_pEditFilter, SIGNAL(textChanged(QString)), this, SLOT(slotDoTableViewFilter(QString)));
    connect(m_pButtonSendEmail, SIGNAL(clicked(bool)), this, SLOT(slotSendEmail()));
    m_pHLayoutFilter->addStretch(1);

    m_pVLayoutMain = new QVBoxLayout;
    m_pHLayoutOpenExcel = new QHBoxLayout;

    this->setLayout(m_pVLayoutMain);
    m_pHLayoutOpenExcel->addWidget(m_pLabelOpenExcel);
    m_pHLayoutOpenExcel->addWidget(m_pEditOpenedExcel);
    m_pHLayoutOpenExcel->addWidget(m_pButtonOpenExcel);

    m_pVLayoutMain->addWidget(m_pGroupSender);
    m_pVLayoutMain->addLayout(m_pHLayoutOpenExcel);
    m_pVLayoutMain->addLayout(m_pHLayoutFilter);
    m_pVLayoutMain->addWidget(m_pTableExcel);
}

void CureSalary::freeXlsxDocument()
{
    if (m_pXlsxDoc) {
        SheetModel *pModel = dynamic_cast<SheetModel*>(m_pTableExcel->model());
        if (pModel) {
            m_pTableExcel->clearSpans();
            m_pTableExcel->setModel(NULL);
            delete pModel;
            pModel = NULL;
        } else {
            qWarning() << "Get SheetModel failed from tableView!";
            //QMessageBox::warning(this, tr(""), tr(""));
            return;
        }
        delete m_pXlsxDoc;
        m_pXlsxDoc = NULL;
    }
}

void CureSalary::initProgressDialog(const int nMaxValue)
{
    if (m_pProgressDialog) {
        m_pProgressDialog->close();
        delete m_pProgressDialog;
        m_pProgressDialog = NULL;
    }
    m_pProgressDialog = new CureEmailDialog(this);
    m_pProgressDialog->setProgressRange(0, (uint)nMaxValue);
    m_pProgressDialog->setWindowModality(Qt::WindowModal);
    m_pProgressDialog->setWindowTitle(tr("员工工资条邮件发送"));
    m_pProgressDialog->show();

    connect(m_pProgressDialog, SIGNAL(sigStartSend()), this, SLOT(slotDialogSendStart()));
    connect(m_pProgressDialog, SIGNAL(sigCancelSend()), this, SLOT(slotDialogSendCancel()));
}

bool CureSalary::writePersonalInfoToFile(const int index, Format &format, QString &strOutFilePath, QString &strName, int &nIndexNumber)
{

    Document doc(g_strSalarySendTemplate);
    Worksheet *pWorkSheetWrite = dynamic_cast<Worksheet *>(doc.sheet(doc.sheetNames()[0]));
    SheetModel *pModel = dynamic_cast<SheetModel *>(m_pTableExcel->model());
    Worksheet *pWorkSheetRead = pModel->sheet();

    bool bWrite = true;
    Cell *pCell = NULL;

    /* 循环使用邮件发送模板的头，其自身没有数据模板的第1列 */
    for (int i = 0; i < pWorkSheetWrite->dimension().lastColumn(); ++i) {
        pCell = pWorkSheetRead->cellAt(index + g_nStartRow, i + 2); /* i + 2是因为过滤掉数据模板的第1列序号列数据 */
        if (g_nIndexName == i) {
            /* 返回员工姓名 */
            strName = pCell->value().toString();
        } else if (g_nIndexNumber == i) {
            /* 返回员工编号 */
            nIndexNumber = pCell->value().toInt();
        }
        //qDebug() << "Cell value: " << pCell->value().toString();
        if (!pWorkSheetWrite->write(index + 2, i + 1, pCell->value(), format)) {
            bWrite = false;
            break;
        }
    }
    if (bWrite) {
        //qDebug() << "Write data success!";
        QDate date = QDate::currentDate();
        int nYear = date.year();
        int nMonth = date.month();
        nMonth = (nMonth == 1? 12: (nMonth - 1));
        strOutFilePath = QString("%1年%2月工资单_%3.xlsx").arg(nYear).arg(nMonth).arg(strName);
        if (QFile::exists(strOutFilePath)) {
            QFile::remove(strOutFilePath);
        }
        bWrite = doc.saveAs(strOutFilePath);
        if (bWrite) {
            qDebug() << "Make salary email attachment file success: " << strOutFilePath;
        }
    }

    return bWrite;
}

/* 根据导入的数据模板，提取邮件发送的excel头部信息。也可以直接使用邮件发送模板，就不需要这样提取了。 */
void CureSalary::saveSalaryExcelHead(Worksheet *pWorksheet)
{
    qDebug() << "Make salary attchment template: " << g_strSalarySendTemplate;

    QString strSaveFileName = g_strSalarySendTemplate;
    Document *pDoc = new Document;
    pDoc->addSheet(tr("工资明细"));

    Format format;
    format.setFont(QFont("宋体", 10, QFont::Bold));
    format.setPatternBackgroundColor(Qt::yellow);
    format.setTextWarp(true); // 自动换行
    format.setVerticalAlignment(Format::AlignVCenter);
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setBorderStyle(Format::BorderThin); // 边框为实线

    /* i从1开始，是因为要过滤掉第1列的序号 */
    for (int i = 1; i < pWorksheet->dimension().lastColumn(); ++i) {
        Cell *pCell = pWorksheet->cellAt(g_nStartRow - 1, i + 1);
        if (!pCell->value().isNull()) {
            pDoc->write(1, i/* + 1*/, pCell->value(), format);
        }
    }
    if (QFile::exists(strSaveFileName)) {
        QFile::remove(strSaveFileName);
    }

    pDoc->saveAs(strSaveFileName);
    delete pDoc;
    pDoc = NULL;
}

bool CureSalary::findEmailAddressFromExcel(const Worksheet* pWorksheet, const int &nIndexNumber, const QString &strName, QString &strAddress)
{
    /* 第一行是标题,数据从第二行起 */
    for (int i = 0; i < pWorksheet->dimension().rowCount() - 1; ++i) {
        /* excel的worksheet数据起始索引为1,包括列 */
        if (pWorksheet->cellAt(i + 2, 4)->value().toInt() == nIndexNumber) {
            /* 第4列是工号 */
            if (pWorksheet->cellAt(i + 2, 2)->value().toString().trimmed() == strName.trimmed()) {
                /* 第2列是员工姓名,第3列是邮箱地址 */
                strAddress = pWorksheet->cellAt(i + 2, 3)->value().toString().trimmed();
                qDebug() << "Find email address of" << strName << "success: " << strAddress;
                return true;
            }
        }
    }
    return false;
}

void CureSalary::makeAndSendEmailData()
{
    qDebug() << "Start to send salary email......";

    int nValue = 0;
    int nSuccessCount = 0;
    int nFailedCount = 0;
    SheetModel *pModel = dynamic_cast<SheetModel*>(m_pTableExcel->model());
    /* 注意这里使用的是引用,可以直接修改QVector的实际值 */
    QVector<int>& vectorCheckState = pModel->getCheckStateVector();
    Format format = getEmailDataFormat();
    Document doc(g_strSalaryEmailExcel);
    Worksheet *pWorkSheetEmailAddresses = dynamic_cast<Worksheet *>(doc.sheet(doc.sheetNames()[0]));
    if (!pWorkSheetEmailAddresses) {
        qCritical() << "Cannot find any worksheet from email excel.";
        QMessageBox::critical(m_pProgressDialog, tr("邮件发送失败"), tr("员工邮箱excel文件是空文件!"));
        int nMaxValue = m_pProgressDialog->getProgressRange();
        m_pProgressDialog->setFailedValue(nMaxValue);
        m_pProgressDialog->setProgressValue(nMaxValue);
        return;
    }

    for (int i = 0; i < vectorCheckState.size(); ++i) {
        if (m_bEmailCanceled) {
            qDebug() << "User canceled email send, break.";
            return;
        }

        /* 先全部置为发送成功,因为最后要展示发送失败的记录 */
        vectorCheckState[i] |= SALARY_SEND_OK;
        if (vectorCheckState[i] & SALARY_CHECKED) {
            /* real send ... */
            QString strFilePath;
            QString strName;
            int nIndexNumber;
            bool bSendOk = false;
            if (writePersonalInfoToFile(i, format, strFilePath, strName, nIndexNumber)) {
                QString strEmailAddress;
                if (findEmailAddressFromExcel(pWorkSheetEmailAddresses, nIndexNumber, strName, strEmailAddress)) {
                    bSendOk = sendEmail(strEmailAddress, strName, strFilePath);
                } else {
                    qWarning() << "Cannot find email address of" << strName << "from excel!";
                }
                /* 邮件发送完成之后删除所有附件 */
                QFile::remove(strFilePath);
            } else {
                qWarning() << "Write salary email data to excel failed of " << strName;
            }
            if (bSendOk) {
                m_pProgressDialog->setSuccessValue(++nSuccessCount);
            } else {
                /* 发送失败的记录,简单地使用黄色背景进行标记 */
                m_pProgressDialog->setFailedValue(++nFailedCount);
                vectorCheckState[i] &= (~SALARY_SEND_OK);
            }
            m_pProgressDialog->setProgressValue(++nValue);
        }
    }
}

Format CureSalary::getEmailDataFormat()
{
    Format format;
    format.setFont(QFont("宋体", 10));
    format.setTextWarp(true); // 自动换行
    format.setVerticalAlignment(Format::AlignVCenter);
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setBorderStyle(Format::BorderThin); // 边框为实线
    return format;
}

bool CureSalary::sendEmail(const QString &strReceiverEmail, const QString &strReceiverName, const QString &strAttachFile)
{
    SmtpClient email(m_pEditSMTPServer->text(), m_pEditSMTPPort->text().toInt(), SmtpClient::SslConnection); // ssl端口是465，非SSL是25
    email.setUser(m_pEditSenderAddress->text());
    email.setPassword(m_pEditSenderPasswd->text());

    QDate date = QDate::currentDate();
    int nYear = date.year();
    int nMonth = date.month();
    nMonth = (nMonth == 1? 12: (nMonth - 1));
    MimeMessage message;
    message.setSender(new EmailAddress(m_pEditSenderAddress->text()));
    message.addRecipient(new EmailAddress(strReceiverEmail));
    message.setSubject(QString("%1年%2月工资条").arg(nYear).arg(nMonth));

    MimeAttachment attachMent(new QFile(strAttachFile));
    attachMent.setContentType("excel");
    message.addPart(&attachMent);

    MimeText text;
    QString strText = QString("%1 您好！附件为您%2年%3月的工资条，请注意查收！若有疑问请在此邮件发送后的5天内咨询HR%4进行处理，否则视为无疑问，谢谢！ ")
            .arg(strReceiverName).arg(nYear).arg(nMonth).arg(m_pEditHRName->text());
    text.setText(strText);
    message.addPart(&text);

    qDebug() << "Start to send email to: " << strReceiverEmail << ".\nContent: " << strText;
    if (email.connectToHost()) {
        if (email.login()) {
            if (email.sendMail(message)) {
                qDebug() << "Send email success!";
            } else {
                qWarning() << "Send email failed!";
            }
            email.quit();
        } else {
            qWarning() << "Email login failed!";
        }
    } else {
        qWarning() << "Email connect failed!";
    }

    return true;
}

