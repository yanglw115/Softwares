#include <QRegExpValidator>

#include "YLW_des_decrypt_widget.h"
#include "YLW_des_source.h"

const static QString g_strKeyDefault = "%U(^f6@R";

DESDecryptWidget::DESDecryptWidget(QWidget *parent)
    : QWidget(parent)
{
    m_pLabelKey = new QLabel(tr("输入密钥(KEY):"), this);
    m_pLineEditKey = new QLineEdit(this);
    m_pLineEditKey->setAutoFillBackground(true);

    /* \\S第一个\是转义符,\S正则里面代表任意一个字符或符号, {8}代表重复8次 */
    QRegExp regKey("\\S{8}");
    QValidator *pValidatorKey = new QRegExpValidator(regKey, this);
    m_pLineEditKey->setValidator(pValidatorKey);
    connect(m_pLineEditKey, SIGNAL(textChanged(QString)), this, SLOT(slotKeyChanged(QString)));
    m_pLineEditKey->setText(g_strKeyDefault);

    m_pButtonKeyReset = new QPushButton(tr("恢复默认"), this);
    m_pButtonKeyClear = new QPushButton(tr("清空"), this);
    connect(m_pButtonKeyReset, SIGNAL(clicked(bool)), this, SLOT(slotKeyRest()));
    connect(m_pButtonKeyClear, SIGNAL(clicked(bool)), this, SLOT(slotKeyClear()));

    m_pLabelCipherInput = new QLabel(tr("输入密文(DES):"), this);
    m_pLineEditCipher = new QLineEdit(this);
    m_pLineEditCipher->setAutoFillBackground(true);
    /* [0-9a-fA-F]代表0-9或a-f或A-F中的任一字符，{16}代表共有16个这样的字符 */
    QRegExp regCipher("([0-9a-fA-F]{16})+");
    QValidator *pValidatorCipher = new QRegExpValidator(regCipher, this);
    m_pLineEditCipher->setValidator(pValidatorCipher);

    connect(m_pLineEditCipher, SIGNAL(textChanged(QString)), this, SLOT(slotCipherChanged(QString)));

    m_pButtonCipherCopy = new QPushButton(tr("复制"), this);
    m_pButtonCipherClear = new QPushButton(tr("清空"), this);
    connect(m_pButtonCipherCopy, SIGNAL(clicked(bool)), this, SLOT(slotCipherCopy()));
    connect(m_pButtonCipherClear, SIGNAL(clicked(bool)), this, SLOT(slotCipherClear()));

    m_pLabelPlainOutput = new QLabel(tr("输出明文(MD5):"), this);
    m_pLineEditPlain = new QLineEdit(this);
    m_pLineEditPlain->setReadOnly(true);

    m_pButtonPlainCopy = new QPushButton(tr("复制"), this);
    m_pButtonPlainClear = new QPushButton(tr("清空"), this);
    connect(m_pButtonPlainCopy, SIGNAL(clicked(bool)), this, SLOT(slotPlainCopy()));
    connect(m_pButtonPlainClear, SIGNAL(clicked(bool)), this, SLOT(slotPlainClear()));

    m_pButtonDecrypt = new QPushButton(tr("DES解密"), this);
    m_pButtonClearAll = new QPushButton(tr("清空所有"), this);
    connect(m_pButtonDecrypt, SIGNAL(clicked(bool)), this, SLOT(slotDesDecrypt()));
    connect(m_pButtonClearAll, SIGNAL(clicked(bool)), this, SLOT(slotClearAll()));

    m_pGridLayout = new QGridLayout(this);
    m_pGridLayout->addWidget(m_pLabelKey, 0, 0);
    /* Qt::AlignLeft向左对齐，不然就占一个GRID全位，向左对齐之后点位会小一点，表现更加紧凑 */
    m_pGridLayout->addWidget(m_pLineEditKey, 0, 1, Qt::AlignLeft);
    m_pGridLayout->addWidget(m_pButtonKeyReset, 0, 2, Qt::AlignLeft);
    m_pGridLayout->addWidget(m_pButtonKeyClear, 0, 3, Qt::AlignLeft);

    m_pGridLayout->addWidget(m_pLabelCipherInput, 1, 0);
    m_pGridLayout->addWidget(m_pLineEditCipher, 1, 1);
    m_pGridLayout->addWidget(m_pButtonCipherCopy, 1, 2, Qt::AlignLeft);
    m_pGridLayout->addWidget(m_pButtonCipherClear, 1, 3, Qt::AlignLeft);

    m_pGridLayout->addWidget(m_pLabelPlainOutput, 2, 0);
    m_pGridLayout->addWidget(m_pLineEditPlain, 2, 1);
    m_pGridLayout->addWidget(m_pButtonPlainCopy, 2, 2, Qt::AlignLeft);
    m_pGridLayout->addWidget(m_pButtonPlainClear, 2, 3, Qt::AlignLeft);

    m_pGridLayout->addWidget(m_pButtonDecrypt, 3, 0);
    m_pGridLayout->addWidget(m_pButtonClearAll, 3, 1, Qt::AlignLeft);

    /* 这里为了调整协调好位置效果，只能借助于最小行度和列度来进行 */
    m_pGridLayout->setRowMinimumHeight(0, 50);
    m_pGridLayout->setRowMinimumHeight(1, 50);
    m_pGridLayout->setRowMinimumHeight(2, 50);
    m_pGridLayout->setRowMinimumHeight(3, 50);
    m_pGridLayout->setColumnMinimumWidth(1, 220);
    /* 这里暂时设置其他行没有作用，设置与不设置效果是一样的 */
    m_pGridLayout->setRowStretch(4, 1);

    /* 下面第2列与第5列保持一样的宽度 */
    m_pGridLayout->setColumnStretch(1, 1);
    m_pGridLayout->setColumnStretch(4, 1);

}

DESDecryptWidget::~DESDecryptWidget()
{

}

void DESDecryptWidget::slotKeyChanged(const QString &strKey)
{
    QPalette palette;
    /* 如果输入不完整，则用红色表示，否则用黑色表示 */
    if (strKey.length() != 8) {
        palette.setColor(QPalette::Text, Qt::red);
    } else {
        palette.setColor(QPalette::Text, Qt::black);
    }
    m_pLineEditKey->setPalette(palette);
}

void DESDecryptWidget::slotKeyRest()
{
    m_pLineEditKey->setText(g_strKeyDefault);
}

void DESDecryptWidget::slotKeyClear()
{
    m_pLineEditKey->clear();
}

void DESDecryptWidget::slotCipherChanged(const QString &strCipher)
{
    QPalette palette;
    if ((strCipher.length() < 16) || (strCipher.length() % 16 != 0)) {
        palette.setColor(QPalette::Text, Qt::red);
    } else {
        palette.setColor(QPalette::Text, Qt::black);
    }
    m_pLineEditCipher->setPalette(palette);
}

void DESDecryptWidget::slotCipherCopy()
{
    m_pLineEditCipher->selectAll();
    m_pLineEditCipher->copy();
}

void DESDecryptWidget::slotCipherClear()
{
    m_pLineEditCipher->clear();
}

void DESDecryptWidget::slotPlainCopy()
{
    m_pLineEditPlain->selectAll();
    m_pLineEditPlain->copy();
}

void DESDecryptWidget::slotPlainClear()
{
    m_pLineEditPlain->clear();
}

void DESDecryptWidget::slotDesDecrypt()
{
    QString strPlain = CMyDES::decryptDataByDES(m_pLineEditCipher->text(), m_pLineEditKey->text());
    m_pLineEditPlain->setText(strPlain);
}

void DESDecryptWidget::slotClearAll()
{
    slotKeyClear();
    slotCipherClear();
    slotPlainClear();
}

