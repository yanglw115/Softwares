#include <QDate>

#include "cure_email_dialog.h"
#include "msvs_charset.h"

static const QString g_strControlStrings[] = {
    "开始发送",
    "取消",
    "关闭",
    "已取消",
    "完成"
};

CureEmailDialog::CureEmailDialog(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
    , m_pLabelTotal(NULL)
    , m_pLabelChecked(NULL)
    , m_pLabelSuccess(NULL)
    , m_pLabelFailed(NULL)
    , m_pLabelValueTotal(NULL)
    , m_pLabelValueChecked(NULL)
    , m_pLabelValueSuccess(NULL)
    , m_pLabelValueFailed(NULL)
    , m_pProgressBar(NULL)
    , m_pButtonStart(NULL)
    , m_pButtonCancel(NULL)
    , m_pMainLayout(NULL)
    , m_nSendButtonState(TYPE_SEND)
{
    initDialog();
}

CureEmailDialog::~CureEmailDialog()
{

}

void CureEmailDialog::initDialog()
{
    m_pLabelSalaryDate = new QLabel(tr("工资年月："), this);
    m_pLabelHR = new QLabel(tr("HR："), this);
    m_pLabelTotal = new QLabel(tr("总记录："), this);
    m_pLabelChecked = new QLabel(tr("已选择："), this);
    m_pLabelSuccess = new QLabel(tr("成功发送："), this);
    m_pLabelFailed = new QLabel(tr("发送失败："), this);

    m_pLabelValueDate = new QLabel(this);
    QDate date = QDate::currentDate();
    int nMonth = date.month();
    nMonth = (nMonth == 1? 12: (nMonth - 1));
    m_pLabelValueDate->setText(QString("%1年%2月").arg(date.year()).arg(nMonth));
    m_pLabelValueHR = new QLabel(this);
    m_pLabelValueTotal = new QLabel(this);
    m_pLabelValueChecked = new QLabel(this);
    m_pLabelValueSuccess = new QLabel(tr("0"), this);
    m_pLabelValueFailed = new QLabel(tr("0"), this);

    m_pProgressBar = new QProgressBar(this);
    m_pButtonStart = new QPushButton(g_strControlStrings[TYPE_SEND], this);
    m_pButtonCancel = new QPushButton(g_strControlStrings[TYPE_CANCEL], this);
    connect(m_pButtonStart, SIGNAL(clicked(bool)), this, SLOT(slotHandleSendButton()));
    connect(m_pButtonCancel, SIGNAL(clicked(bool)), this, SLOT(slotHandleCancelButton()));

    QHBoxLayout *pHLayoutDate = new QHBoxLayout;
    pHLayoutDate->addSpacing(40);
    pHLayoutDate->addWidget(m_pLabelSalaryDate);
    pHLayoutDate->addWidget(m_pLabelValueDate);
    pHLayoutDate->addStretch();

    QHBoxLayout *pHLayoutHR = new QHBoxLayout;
    pHLayoutHR->addSpacing(40);
    pHLayoutHR->addWidget(m_pLabelHR);
    pHLayoutHR->addWidget(m_pLabelValueHR);
    pHLayoutHR->addStretch();

    QHBoxLayout *pHLayoutTotal = new QHBoxLayout;
    pHLayoutTotal->addSpacing(40);
    pHLayoutTotal->addWidget(m_pLabelTotal);
    pHLayoutTotal->addWidget(m_pLabelValueTotal);
    pHLayoutTotal->addStretch();

    QHBoxLayout *pHLayoutChecked = new QHBoxLayout;
    pHLayoutChecked->addSpacing(40);
    pHLayoutChecked->addWidget(m_pLabelChecked);
    pHLayoutChecked->addWidget(m_pLabelValueChecked);
    pHLayoutChecked->addStretch();

    QHBoxLayout *pHLayoutSucess = new QHBoxLayout;
    pHLayoutSucess->addSpacing(40);
    pHLayoutSucess->addWidget(m_pLabelSuccess);
    pHLayoutSucess->addWidget(m_pLabelValueSuccess);
    pHLayoutSucess->addStretch();

    QHBoxLayout *pHLayoutFailed = new QHBoxLayout;
    pHLayoutFailed->addSpacing(40);
    pHLayoutFailed->addWidget(m_pLabelFailed);
    pHLayoutFailed->addWidget(m_pLabelValueFailed);
    pHLayoutFailed->addStretch();

    QHBoxLayout *pHLayoutButton = new QHBoxLayout;
    pHLayoutButton->addStretch();
    pHLayoutButton->addWidget(m_pButtonStart);
    pHLayoutButton->addWidget(m_pButtonCancel);
    pHLayoutButton->addStretch();

    m_pMainLayout = new QVBoxLayout(this);
    m_pMainLayout->addSpacing(10);
    m_pMainLayout->addLayout(pHLayoutDate);
    m_pMainLayout->addLayout(pHLayoutHR);
    m_pMainLayout->addLayout(pHLayoutTotal);
    m_pMainLayout->addLayout(pHLayoutChecked);
    m_pMainLayout->addLayout(pHLayoutSucess);
    m_pMainLayout->addLayout(pHLayoutFailed);
    m_pMainLayout->addStretch();
    m_pMainLayout->addWidget(m_pProgressBar);
    m_pMainLayout->addLayout(pHLayoutButton);

    /* 等触发邮件发送的时候再显示出来(但经常刷新不及时,还是直接显示算了) */
//    setSendStaticsVisible(false);
    /* 未发送前禁止取消 */
    m_pButtonCancel->setEnabled(false);

    this->setFixedSize(400, 200);
}

void CureEmailDialog::setHRName(const QString &strName)
{
    m_pLabelValueHR->setText(strName);
}

void CureEmailDialog::setProgressRange(const uint nStart, const uint nEnd)
{
    m_pProgressBar->setRange((int)nStart, (int)nEnd);
}

int CureEmailDialog::getProgressRange() const
{
    return m_pProgressBar->maximum();
}

void CureEmailDialog::setTotalValue(const uint nValue)
{
    m_pLabelValueTotal->setText(QString("%1").arg(nValue));
}

void CureEmailDialog::setCheckedValue(const uint nValue)
{
    m_pLabelValueChecked->setText(QString("%1").arg(nValue));
}

void CureEmailDialog::setSuccessValue(const uint nValue)
{
    m_pLabelValueSuccess->setText(QString("%1").arg(nValue));
}

void CureEmailDialog::setFailedValue(const uint nValue)
{
    m_pLabelValueFailed->setText(QString("%1").arg(nValue));
}

void CureEmailDialog::setProgressValue(const uint nValue)
{
    m_pProgressBar->setValue(nValue);
    if (nValue == m_pProgressBar->maximum()) {
        m_pButtonStart->setEnabled(true);
        m_pButtonCancel->setEnabled(false);
        m_nSendButtonState = TYPE_FINISH;
        m_pButtonStart->setText(g_strControlStrings[TYPE_FINISH]);
    }
}

void CureEmailDialog::setSendStaticsVisible(bool bShow)
{
    m_pLabelSuccess->setVisible(bShow);
    m_pLabelValueSuccess->setVisible(bShow);
    m_pLabelFailed->setVisible(bShow);
    m_pLabelValueFailed->setVisible(bShow);
}

void CureEmailDialog::slotHandleSendButton()
{
    if (TYPE_SEND == m_nSendButtonState) {
        m_pButtonStart->setEnabled(false);
        m_pButtonCancel->setEnabled(true);
        //setSendStaticsVisible(true);
        emit sigStartSend();
    } else if (TYPE_CLOSE == m_nSendButtonState || TYPE_FINISH == m_nSendButtonState) {
        this->close();
        this->deleteLater();
    }
}

void CureEmailDialog::slotHandleCancelButton()
{
    if (TYPE_SEND == m_nSendButtonState) {
        m_nSendButtonState = TYPE_CLOSE;
        emit sigCancelSend();
        m_pButtonCancel->setEnabled(false);
        m_pButtonStart->setEnabled(true);
        m_pButtonCancel->setText(g_strControlStrings[TYPE_CANCELED]);
        m_pButtonStart->setText(g_strControlStrings[TYPE_CLOSE]);
    }
}

