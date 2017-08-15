#include <QMessageBox>

#include "msgboxdlg.h"

MsgBoxDlg::MsgBoxDlg(QWidget *parent)
    : QDialog(parent)
    , m_pLabel(NULL)
    , m_pButtonQuest(NULL)
    , m_pButtonInfo(NULL)
    , m_pButtonWarn(NULL)
    , m_pButtonCrit(NULL)
    , m_pButtonAbout(NULL)
    , m_pButtonQtAbout(NULL)
    , m_pGridLayout(NULL)
{
    m_pLabel = new QLabel(tr("请选择一种消息框："), this);
    m_pButtonQuest = new QPushButton(tr("QuestionMsg"), this);
    m_pButtonInfo = new QPushButton(tr("InformationMsg"), this);
    m_pButtonWarn = new QPushButton(tr("WarningMsg"), this);
    m_pButtonCrit = new QPushButton(tr("CriticalMsg"), this);
    m_pButtonAbout = new QPushButton(tr("AboutMsg"), this);
    m_pButtonQtAbout = new QPushButton(tr("QtAboutMsg"), this);

    m_pGridLayout = new QGridLayout(this);
    /* 把Label也放放到gridlayout中，这样也可以保证label无论多长，都与按钮对齐 */
    m_pGridLayout->addWidget(m_pLabel, 0, 0, 1, 2);
    m_pGridLayout->addWidget(m_pButtonQuest, 1, 0);
    m_pGridLayout->addWidget(m_pButtonInfo, 1, 1);
    m_pGridLayout->addWidget(m_pButtonWarn, 2, 0);
    m_pGridLayout->addWidget(m_pButtonCrit, 2, 1);
    m_pGridLayout->addWidget(m_pButtonAbout, 3, 0);
    m_pGridLayout->addWidget(m_pButtonQtAbout, 3, 1);

    connect(m_pButtonQuest, SIGNAL(clicked()), this, SLOT(slotShowQuestMsg()));
    connect(m_pButtonInfo, SIGNAL(clicked()), this, SLOT(slotShowInfoMsg()));
    connect(m_pButtonWarn, SIGNAL(clicked()), this, SLOT(slotShowWarnMsg()));
    connect(m_pButtonCrit, SIGNAL(clicked()), this, SLOT(slotShowCritMsg()));
    connect(m_pButtonAbout, SIGNAL(clicked()), this, SLOT(slotShowAboutMsg()));
    connect(m_pButtonQtAbout, SIGNAL(clicked()), this, SLOT(slotShowQtAboutMsg()));
}


MsgBoxDlg::~MsgBoxDlg()
{

}

void MsgBoxDlg::slotShowQuestMsg()
{
    m_pLabel->setText(tr("Question message box"));
    switch (QMessageBox::question(this, tr("Question消息框"), tr("您现在已经修改完成，是否要结束程序？"),
                                  QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok)) {
    case QMessageBox::Ok:
        m_pLabel->setText("Question button/Ok");
        break;
    case QMessageBox::Cancel:
        m_pLabel->setText("Question button/Cancel");
        break;
    }
}

void MsgBoxDlg::slotShowInfoMsg()
{
    m_pLabel->setText(tr("Information message box"));
    QMessageBox::information(this, tr("Information消息框"), tr("这是information消息测量框，欢迎您!"));
}

void MsgBoxDlg::slotShowWarnMsg()
{
    m_pLabel->setText(tr("Warning message box"));
    switch (QMessageBox::warning(this, tr("Warning消息框"), tr("当前修改的内容尚未保存，是否保存更改?"),
                         QMessageBox::Save | QMessageBox::Abort | QMessageBox::Cancel, QMessageBox::Save)) {
    case QMessageBox::Save:
        m_pLabel->setText(tr("Warning message box/save"));
        break;
    case QMessageBox::Abort:
        m_pLabel->setText(tr("Warning message box/abort"));
        break;
    case QMessageBox::Cancel:
        m_pLabel->setText(tr("Warning message box/cancel"));
        break;
    default:
        break;
    }
}

void MsgBoxDlg::slotShowCritMsg()
{
    m_pLabel->setText(tr("Critical message box"));
    QMessageBox::critical(this, tr("Critical消息框"), tr("这是一个Critical消息测试框"));
}

void MsgBoxDlg::slotShowAboutMsg()
{
    m_pLabel->setText(tr("About message box"));
    QMessageBox::about(this, tr("About消息框"), tr("这是一个About消息测试框"));
}

void MsgBoxDlg::slotShowQtAboutMsg()
{
    m_pLabel->setText(tr("Qt about box"));
    QMessageBox::aboutQt(this, "Qt about消息框");
}
