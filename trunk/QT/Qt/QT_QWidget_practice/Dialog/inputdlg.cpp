#include <QInputDialog>
#include <QtDebug>

#include "inputdlg.h"

InputDlg::InputDlg(QWidget *parent)
    : QDialog(parent)
{
    this->setWindowTitle(tr("标准输入对话框"));
    m_pLabelName = new QLabel(tr("姓名: "));
    m_pLabelNameV = new QLabel(tr("杨利伟"));
    m_pLabelNameV->setFrameStyle(QFrame::Sunken | QFrame::Panel);
    m_pButtonName = new QPushButton(tr("修改姓名"));
    connect(m_pButtonName, SIGNAL(clicked()), this, SLOT(slotChangeName()));

    m_pLabelAge = new QLabel(tr("年龄: "));
    m_pLabelAgeV = new QLabel(tr("31"));
    m_pLabelAgeV->setFrameStyle(QFrame::Sunken | QFrame::Panel);
    m_pButtonAge = new QPushButton(tr("修改年龄"));
    connect(m_pButtonAge, SIGNAL(clicked()), this, SLOT(slotChangeAge()));

    m_pLabelSex = new QLabel(tr("性别: "));
    m_pLabelSexV = new QLabel(tr("男"));
    m_pLabelSexV->setFrameStyle(QFrame::Sunken | QFrame::Panel);
    m_pButtonSex = new QPushButton(tr("修改性别"));
    connect(m_pButtonSex, SIGNAL(clicked()), this, SLOT(slotChangeSex()));

    m_pLabelScore = new QLabel(tr("成绩: "));
    m_pLabelScoreV = new QLabel(tr("99.1"));
    m_pLabelScoreV->setFrameStyle(QFrame::Sunken | QFrame::Panel);
    m_pButtonScore = new QPushButton(tr("修改成绩"));
    connect(m_pButtonScore, SIGNAL(clicked()), this, SLOT(slotChangeScore()));

    m_pGridLayout = new QGridLayout;
    m_pGridLayout->addWidget(m_pLabelName, 0, 0);
    m_pGridLayout->addWidget(m_pLabelNameV, 0, 1);
    m_pGridLayout->addWidget(m_pButtonName, 0, 2);
    m_pGridLayout->addWidget(m_pLabelAge, 1, 0);
    m_pGridLayout->addWidget(m_pLabelAgeV, 1, 1);
    m_pGridLayout->addWidget(m_pButtonAge, 1, 2);
    m_pGridLayout->addWidget(m_pLabelSex, 2, 0);
    m_pGridLayout->addWidget(m_pLabelSexV, 2, 1);
    m_pGridLayout->addWidget(m_pButtonSex, 2, 2);
    m_pGridLayout->addWidget(m_pLabelScore, 3, 0);
    m_pGridLayout->addWidget(m_pLabelScoreV, 3, 1);
    m_pGridLayout->addWidget(m_pButtonScore, 3, 2);

    /* 注意，这里在设置stretch的时候，需要设置对应列或行的 */
    m_pGridLayout->setColumnStretch(1, 1);
    m_pGridLayout->setColumnStretch(2, 1);
    m_pGridLayout->setMargin(10);
    m_pGridLayout->setSpacing(10);

    this->setLayout(m_pGridLayout);
}

InputDlg::~InputDlg()
{

}

void InputDlg::slotChangeName()
{
    bool bOK;
    qDebug() << "打开标准字符串输入对话框：";
    QString strText = QInputDialog::getText(this, tr("字符串标准输入对话框"), tr("请输入姓名："), QLineEdit::Normal, m_pLabelNameV->text(), &bOK);
    if (bOK && !strText.isEmpty()) {
        m_pLabelNameV->setText(strText);
    }
}

void InputDlg::slotChangeAge()
{
    bool bOK;
    int age = QInputDialog::getInt(this, tr("标准int类型输入对话框"), tr("请选择年龄："), m_pLabelAgeV->text().toInt(&bOK), 0, 200, 1, &bOK);
    if (bOK) {
        m_pLabelAgeV->setText(QString("%1").arg(age));
    }
}

void InputDlg::slotChangeSex()
{
    QStringList sexListItems;
    bool bOK;
    sexListItems << tr("男") << tr("女");
    QString strSex = QInputDialog::getItem(this, tr("标准条目选择对话框"), tr("请选择性别："), sexListItems, 0, false, &bOK);
    if (bOK && !strSex.isEmpty()) {
        m_pLabelSexV->setText(strSex);
    }
}

void InputDlg::slotChangeScore()
{
    bool bOK;
    double score = QInputDialog::getDouble(this, tr("标准double类型输入对话框"), tr("请选择分数："), m_pLabelScoreV->text().toDouble(&bOK), 0.0, 100.0, 1, &bOK);
    if (bOK) {
        m_pLabelScoreV->setText(QString("%1").arg(score));
    }
}

