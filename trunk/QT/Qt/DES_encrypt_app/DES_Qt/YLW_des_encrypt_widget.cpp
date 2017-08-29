#include <QRegExpValidator>
#include <QFileDialog>
#include <QFileInfo>
#include <QtDebug>

#include "YLW_des_source.h"
#include "YLW_des_encrypt_widget.h"
#include "YLW_VS_char_set.h"

const static QString g_strKeyDefault = "%U(^f6@R";

DESEncryptWidget::DESEncryptWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setAcceptDrops(true);
    m_pLabelFile = new QLabel(tr("加密文件(可选):"), this);
    m_pLineEditFile = new FileLineEdit(this);
    /* 这里的dragEnable只是针对行编辑里面的字符，可以对选中的字符进行拖动 */
//    m_pLineEditFile->setDragEnabled(true);
    m_pLineEditFile->setAcceptDrops(true);
    connect(m_pLineEditFile, SIGNAL(textChanged(QString)), this, SLOT(slotChangeFileName(QString)));

    m_pButtonBrowse = new QPushButton(tr("浏览"), this);
    connect(m_pButtonBrowse, SIGNAL(clicked()), this, SLOT(slotBrowseFile()));
    m_pButtonMD5 = new QPushButton(tr("MD5"), this);
    connect(m_pButtonMD5, SIGNAL(clicked()), this, SLOT(slotMD5ForFile()));

    m_pLineEditFileName = new QLineEdit(this);
    m_pLineEditFileName->setEnabled(false);
    m_pButtonFileNameCopy = new QPushButton(tr("复制"), this);
    connect(m_pButtonFileNameCopy, SIGNAL(clicked(bool)), this, SLOT(slotCopyFileName()));

    m_pLabelKey = new QLabel(tr("输入密钥(KEY):"), this);
    m_pLineEditKey = new QLineEdit(this);
    /* 匹配8个非空白字符 */
    QRegExp regKey("\\S{8}"); /* 这里\\s前一个\是转义符 */
    QValidator *pValidatorKey = new QRegExpValidator(regKey, this);
    m_pLineEditKey->setValidator(pValidatorKey);
    m_pLineEditKey->setAutoFillBackground(true);
    connect(m_pLineEditKey, SIGNAL(textChanged(QString)), this, SLOT(slotKeyChanged(QString)));
    m_pLineEditKey->setText(g_strKeyDefault);

    m_pButtonKeyReset = new QPushButton(tr("恢复默认值"), this);
    connect(m_pButtonKeyReset, SIGNAL(clicked(bool)), this, SLOT(slotKeyReset()));
    m_pButtonKeyClear = new QPushButton(tr("清空"), this);
    connect(m_pButtonKeyClear, SIGNAL(clicked(bool)), this, SLOT(slotKeyClear()));

    m_pLabelPlainInput = new QLabel(tr("输入明文(MD5):"), this);
    m_pLineEditPlainInput = new QLineEdit(this);
    m_pLineEditPlainInput->setAutoFillBackground(true);
    /* [0-9a-fA-F]代表这些字符中任意1个，即MD5每个字符的取值范围; []{16},有16个这个的字符；([]{16})+,至少有16或16多倍个这样的数据 */
    QRegExp regPlain("([0-9a-fA-F]{16})+");
    QValidator *pValidatorPlain = new QRegExpValidator(regPlain, this);
    m_pLineEditPlainInput->setValidator(pValidatorPlain);
    connect(m_pLineEditPlainInput, SIGNAL(textChanged(QString)), this, SLOT(slotPlainChanged(QString)));


    m_pButtonPlainCopy = new QPushButton(tr("复制"), this);
    m_pButtonPlainClear = new QPushButton(tr("清空"), this);
    connect(m_pButtonPlainCopy, SIGNAL(clicked(bool)), this, SLOT(slotPlainCopy()));
    connect(m_pButtonPlainClear, SIGNAL(clicked(bool)), this, SLOT(slotPlainClear()));

    m_pLabelCipherOutput = new QLabel(tr("输出密文(DES):"), this);
    m_pLineEditCipherOutput = new QLineEdit(this);
    /* setEnabled: 不响应鼠标键盘操作，不能鼠标拷贝 */
//    m_pLineEditCipherOutput->setEnabled(false);
    /* setReadOnly: 不能编辑，但是能够拷贝 */
    m_pLineEditCipherOutput->setReadOnly(true);

    m_pButtonCipherCopy = new QPushButton(tr("复制"), this);
    m_pButtonCipherClear = new QPushButton(tr("清空"), this);
    connect(m_pButtonCipherCopy, SIGNAL(clicked(bool)), this, SLOT(slotCipherCopy()));
    connect(m_pButtonCipherClear, SIGNAL(clicked(bool)), this, SLOT(slotCipherClear()));

    m_pButtonEncrypt = new QPushButton(tr("DES加密"), this);
    m_pButtonClearAll = new QPushButton(tr("清空所有"), this);
    m_pCheckBoxOper = new QCheckBox(tr("操作数据库"), this);
    connect(m_pButtonEncrypt, SIGNAL(clicked()), this, SLOT(slotEncrypt()));
    connect(m_pButtonClearAll, SIGNAL(clicked(bool)), this, SLOT(slotClearAll()));
    connect(m_pCheckBoxOper, SIGNAL(stateChanged(int)), this, SLOT(slotShowDatabaseOper(int)));


    /* 数据库 */
    m_pWidgetSqlOper = new SQLOperateWidget(this);


    m_pGridLayout = new QGridLayout(this);
    m_pGridLayout->addWidget(m_pLabelFile, 0, 0);
    m_pGridLayout->addWidget(m_pLineEditFile, 0, 1);
    m_pGridLayout->addWidget(m_pButtonBrowse, 0, 2, Qt::AlignLeft);
    m_pGridLayout->addWidget(m_pButtonMD5, 0, 3, Qt::AlignLeft);

    m_pGridLayout->addWidget(m_pLineEditFileName, 1, 1, Qt::AlignRight);
    m_pGridLayout->addWidget(m_pButtonFileNameCopy, 1, 2, Qt::AlignLeft);

    m_pGridLayout->addWidget(m_pLabelKey, 2, 0);
    m_pGridLayout->addWidget(m_pLineEditKey, 2, 1, Qt::AlignLeft);
    m_pGridLayout->addWidget(m_pButtonKeyReset, 2, 2, Qt::AlignLeft);
    m_pGridLayout->addWidget(m_pButtonKeyClear, 2, 3, Qt::AlignLeft);

    m_pGridLayout->addWidget(m_pLabelPlainInput, 3, 0);
    m_pGridLayout->addWidget(m_pLineEditPlainInput, 3, 1);
    m_pGridLayout->addWidget(m_pButtonPlainCopy, 3, 2, Qt::AlignLeft);
    m_pGridLayout->addWidget(m_pButtonPlainClear, 3, 3, Qt::AlignLeft);

    m_pGridLayout->addWidget(m_pLabelCipherOutput, 4, 0);
    m_pGridLayout->addWidget(m_pLineEditCipherOutput, 4, 1);
    m_pGridLayout->addWidget(m_pButtonCipherCopy, 4, 2, Qt::AlignLeft);
    m_pGridLayout->addWidget(m_pButtonCipherClear, 4, 3, Qt::AlignLeft);

    m_pGridLayout->addWidget(m_pButtonEncrypt, 5, 0, Qt::AlignLeft);
    m_pGridLayout->addWidget(m_pButtonClearAll, 5, 1, Qt::AlignLeft);
    m_pGridLayout->addWidget(m_pCheckBoxOper, 5, 2, Qt::AlignLeft);

    /* 这里使用QWidget，便于隐藏数据库操作部分内容 */
    m_pGridLayout->addWidget(m_pWidgetSqlOper, 6, 0, 1, 4);
    m_pGridLayout->setRowStretch(6, 1);

    m_pGridLayout->setRowMinimumHeight(0, 36);
    m_pGridLayout->setRowMinimumHeight(1, 36);
    m_pGridLayout->setRowMinimumHeight(2, 36);
    m_pGridLayout->setRowMinimumHeight(3, 36);
    m_pGridLayout->setRowMinimumHeight(4, 36);
    m_pGridLayout->setRowMinimumHeight(5, 36);
//    m_pWidgetSqlOper->hide();

}


DESEncryptWidget::~DESEncryptWidget()
{

}

void DESEncryptWidget::slotBrowseFile()
{
#ifdef Q_OS_UNIX
    QString strFilePath = QFileDialog::getOpenFileName(this, tr("选择一个将计算其MD5值的文件"), "/", "file(*.*)");
#else
    QString strFilePath = QFileDialog::getOpenFileName(this, tr("选择一个将计算其MD5值的文件"), "Desktop", "file(*.*)");
#endif

    if (!strFilePath.isEmpty()) {
        qDebug() << "打开的文件为:" << strFilePath;
        m_pLineEditFile->setText(strFilePath);
        slotChangeFileName(strFilePath);
    }
}

void DESEncryptWidget::slotMD5ForFile()
{
    QString strFilePath = m_pLineEditFile->text();
    if (QFile::exists(strFilePath)) {
        QString strMD5 = CMyDES::md5ForFile(strFilePath);
        m_pLineEditPlainInput->setText(strMD5);
    } else {
        qCritical() << "File is not exist:" << strFilePath;
    }
}

void DESEncryptWidget::slotChangeFileName(const QString &strFilePath)
{
    /* 这里可以使用fileInfo方便地获取到文件路径中文件的名称 */
    QFileInfo fileInfo(strFilePath);
    if (fileInfo.exists()) {
        m_pLineEditFileName->setText(fileInfo.fileName());
    }
}

void DESEncryptWidget::slotCopyFileName()
{
    m_pLineEditFileName->selectAll();
    m_pLineEditFileName->copy();
}

void DESEncryptWidget::slotKeyReset()
{
    m_pLineEditKey->setText(g_strKeyDefault);
}

void DESEncryptWidget::slotKeyClear()
{
    m_pLineEditKey->clear();
}

void DESEncryptWidget::slotPlainCopy()
{
    m_pLineEditPlainInput->selectAll();
    m_pLineEditPlainInput->copy();
}

void DESEncryptWidget::slotPlainClear()
{
    m_pLineEditPlainInput->clear();
}

void DESEncryptWidget::slotCipherCopy()
{
    m_pLineEditCipherOutput->selectAll();
    m_pLineEditCipherOutput->copy();
}

void DESEncryptWidget::slotCipherClear()
{
    m_pLineEditCipherOutput->clear();
}

void DESEncryptWidget::slotEncrypt()
{
    m_pLineEditCipherOutput->setText(CMyDES::encryptDataByDES(m_pLineEditPlainInput->text(), m_pLineEditKey->text()));
}

void DESEncryptWidget::slotClearAll()
{
    slotKeyClear();
    slotPlainClear();
    slotCipherClear();
}

void DESEncryptWidget::slotShowDatabaseOper(int state)
{
    if (Qt::Unchecked == state) {
        emit sigOperDatabase(false);
        m_pWidgetSqlOper->hide();
    } else if (Qt::Checked == state) {
        emit sigOperDatabase(true);
        m_pWidgetSqlOper->show();
    }
}


void DESEncryptWidget::slotKeyChanged(const QString &strKey)
{
    QPalette palette;
    if (strKey.length() < 8) {
        palette.setColor(QPalette::Text, Qt::red);
        m_pLineEditKey->setPalette(palette);
    } else {
        palette.setColor(QPalette::Text, Qt::black);
        m_pLineEditKey->setPalette(palette);
    }
}

void DESEncryptWidget::slotPlainChanged(const QString &strPlain)
{
    QPalette palette;
    if ((strPlain.length() < 16) || (strPlain.length() % 16 != 0)) {
        palette.setColor(QPalette::Text, Qt::red);
        m_pLineEditPlainInput->setPalette(palette);
    } else {
        palette.setColor(QPalette::Text, Qt::black);
        m_pLineEditPlainInput->setPalette(palette);
    }
}
