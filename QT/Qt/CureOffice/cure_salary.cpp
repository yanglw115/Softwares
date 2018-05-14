#include <QFileDialog>
#include <memory>
#include <iostream>
#include <QtDebug>
#include <QMessageBox>
#include <QHeaderView>
#include <QFile>
#include <QValidator>

#include "cure_salary.h"
#include "msvs_charset.h"
#include "hanz2pinyin/Hanz2Piny.h"
#include "smtpClient/SmtpMime.h"
#include "cure_tableheaderview.h"

using namespace std;

static const QString g_strOpenExcel("打开文件");
static const QString g_strCloseExcel("关闭");
static const QString g_strDefaultSMTPServer("smtp.ym.163.com");
static const QString g_strDefautlSMTPPort("465");

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
    , m_bStateOpenExcel(false)
    , m_nStateSenderCheck(State_UnChecked)
{
    initWidgets();
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
                    int nStartRow = 5;
                    SheetModel *pModel = new SheetModel(pWorksheet, m_pTableExcel, nStartRow);
                    m_pTableExcel->setModel(pModel);
                    /* 以下对TableView设置header的方式不生效，不能够展示自定义的效果，对TableWidget可能是OK的 */
//                    HeaderViewCheck *pHeaderView = new HeaderViewCheck(Qt::Vertical, m_pTableExcel);
//                    m_pTableExcel->setVerticalHeader(pHeaderView);
                    CheckBoxDelegate *pDelegate = new CheckBoxDelegate(this);
                    m_pTableExcel->setItemDelegate(pDelegate);

                    /* 这里虽然设置了自定义的headerView，但是model的headerData还会生效，只是自定义的headerView会覆盖其设置区域的值 */
                    TableHeaderView *pHeaderView = new TableHeaderView(Qt::Horizontal, this);
                    m_pTableExcel->setHorizontalHeader(pHeaderView);

                    /* 上面使用了自定义水平方向的头，所以下面需要show()一下，否则不显示 */
                    m_pTableExcel->horizontalHeader()->show();
                    connect(pModel, SIGNAL(sigCheckStateChanged(int)), pHeaderView, SLOT(slotCheckStateChanged(int)));
                    connect(pHeaderView, SIGNAL(sigCheckStateChanged(int)), pModel, SLOT(slotCheckStateChanged(int)));
                    m_bStateOpenExcel = true;
                    m_pEditFilter->clear();
                    qDebug() << "Table row count: " << m_pTableExcel->model()->rowCount() << ", colum count: " << m_pTableExcel->model()->columnCount();
                }
            } else {
                qCritical() << "New xlsx document failed!";
            }
        }
    } else {
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
            //qDebug() << "Hanz to pinyin: " << strPinyin;
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

    SmtpClient email(m_pEditSMTPServer->text(), m_pEditSMTPPort->text().toInt(), SmtpClient::SslConnection); // ssl端口是465，非SSL是25
    email.setUser(m_pEditSenderAddress->text());
    email.setPassword(m_pEditSenderPasswd->text());

    if (email.connectToHost()) {
        if (email.login()) {
            email.quit();
            m_pLabelSenderValid->setText(g_strSenderCheckState[State_Check_Success]);
        } else {
            qWarning() << "Email login failed!";
            m_pLabelSenderValid->setText(g_strSenderCheckState[State_Check_Failed]);
        }
    } else {
        qWarning() << "Email connect failed!";
        m_pLabelSenderValid->setText(g_strSenderCheckState[State_Check_Failed]);
    }

    m_pGroupSender->setEnabled(true);
}

void CureSalary::slotEmailSenderDataChanged(const QString &strText)
{
    Q_UNUSED(strText)
    m_pLabelSenderValid->setText(g_strSenderCheckState[State_UnChecked]);
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
    m_pHLayoutFilter = new QHBoxLayout;
    m_pHLayoutFilter->addWidget(m_pLabelFilter);
    m_pHLayoutFilter->addWidget(m_pEditFilter);
    connect(m_pEditFilter, SIGNAL(textChanged(QString)), this, SLOT(slotDoTableViewFilter(QString)));
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

/* 根据导入的数据模板，提取邮件发送的excel头部信息。也可以直接使用邮件发送模板，就不需要这样提取了。 */
void CureSalary::saveSalaryExcelHead(Worksheet *pWorksheet, const int nStartRow)
{
    QString strSaveFileName = "Salary_201808.xlsx";
    Document *pDoc = new Document;
    pDoc->addSheet(tr("工资明细"));

    Format format;
    format.setFont(QFont("宋体", 10, QFont::Bold));
    format.setPatternBackgroundColor(Qt::yellow);
    format.setTextWarp(true); // 自动换行
    format.setVerticalAlignment(Format::AlignVCenter);
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setBorderStyle(Format::BorderThin); // 边框为实线

    for (int i = 0; i < pWorksheet->dimension().lastColumn(); ++i) {
        Cell *pCell = pWorksheet->cellAt(nStartRow - 1, i + 1);
        if (!pCell->value().isNull()) {
            pDoc->write(1, i + 1, pCell->value(), format);
        }
    }
    if (QFile::exists(strSaveFileName)) {
        QFile::remove(strSaveFileName);
    }
    pDoc->saveAs(strSaveFileName);
    delete pDoc;
    pDoc = NULL;
}

bool CureSalary::sendEmail(const QString strTitle, const QString strAttachFile)
{
    SmtpClient email("smtp.ym.163.com", 465, SmtpClient::SslConnection); // ssl端口是465，非SSL是25
    email.setUser("xxxxxx@xxxx.com");
    email.setPassword("xxxxxx");

    MimeMessage message;
    message.setSender(new EmailAddress("yangliwei@95051.com", "yangliwei"));
    message.addRecipient(new EmailAddress("yanglw115@foxmail.com", "xingyu"));
    message.setSubject("2018年3月工资条");

    MimeAttachment attachMent(new QFile("Salary_201808.xlsx"));
    attachMent.setContentType("excel");
    message.addPart(&attachMent);

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

