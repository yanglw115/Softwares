#include "mydrawer.h"

MyDrawer::MyDrawer(QWidget *parent)
    : QToolBox(parent)
{
    m_pGroupBoxFriend = new QGroupBox(this);
    m_pGroupBoxStranger = new QGroupBox(this);
    m_pGroupBoxBlackList = new QGroupBox(this);

    m_pVBoxFriend = new QVBoxLayout(m_pGroupBoxFriend);
    m_pVBoxStranger = new QVBoxLayout(m_pGroupBoxStranger);
    m_pVBoxBlackList = new QVBoxLayout(m_pGroupBoxBlackList);

    m_pTButtonFriend01 = new QToolButton(this);
    m_pTButtonFriend01->setText(tr("王小一"));
    m_pTButtonFriend01->setIcon(QPixmap(":1.png"));
    m_pTButtonFriend01->setIconSize(QPixmap(":1.png").size());
    m_pTButtonFriend01->setAutoRaise(true);
    m_pTButtonFriend01->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    m_pTButtonFriend02 = new QToolButton(this);
    m_pTButtonFriend02->setText(tr("王小一"));
    m_pTButtonFriend02->setIcon(QPixmap(":2.png"));
    m_pTButtonFriend02->setIconSize(QPixmap(":2.png").size());
    m_pTButtonFriend02->setAutoRaise(true);
    m_pTButtonFriend02->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    m_pTButtonFriend03 = new QToolButton(this);
    m_pTButtonFriend03->setText(tr("王小一"));
    m_pTButtonFriend03->setIcon(QPixmap(":3.png"));
    m_pTButtonFriend03->setIconSize(QPixmap(":3.png").size());
    m_pTButtonFriend03->setAutoRaise(true);
    m_pTButtonFriend03->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    m_pTButtonFriend04 = new QToolButton(this);
    m_pTButtonFriend04->setText(tr("王小一"));
    m_pTButtonFriend04->setIcon(QPixmap(":4.png"));
    m_pTButtonFriend04->setIconSize(QPixmap(":4.png").size());
    m_pTButtonFriend04->setAutoRaise(true);
    m_pTButtonFriend04->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    m_pTButtonFriend05 = new QToolButton(this);
    m_pTButtonFriend05->setText(tr("王小一"));
    m_pTButtonFriend05->setIcon(QPixmap(":5.png"));
    m_pTButtonFriend05->setIconSize(QPixmap(":5.png").size());
    m_pTButtonFriend05->setAutoRaise(true);
    m_pTButtonFriend05->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    m_pTButtonFriend10 = new QToolButton(this);
    m_pTButtonFriend10->setText(tr("王小一"));
    m_pTButtonFriend10->setIcon(QPixmap(":6.png"));
    m_pTButtonFriend10->setIconSize(QPixmap(":6.png").size());
    m_pTButtonFriend10->setAutoRaise(true);
    m_pTButtonFriend10->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    m_pTButtonFriend11 = new QToolButton(this);
    m_pTButtonFriend11->setText(tr("王小一"));
    m_pTButtonFriend11->setIcon(QPixmap(":7.png"));
    m_pTButtonFriend11->setIconSize(QPixmap(":7.png").size());
    m_pTButtonFriend11->setAutoRaise(true);
    m_pTButtonFriend11->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    m_pTButtonFriend20 = new QToolButton(this);
    m_pTButtonFriend20->setText(tr("王小一"));
    m_pTButtonFriend20->setIcon(QPixmap(":8.png"));
    m_pTButtonFriend20->setIconSize(QPixmap(":8.png").size());
    m_pTButtonFriend20->setAutoRaise(true);
    m_pTButtonFriend20->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    m_pTButtonFriend21 = new QToolButton(this);
    m_pTButtonFriend21->setText(tr("王小一"));
    m_pTButtonFriend21->setIcon(QPixmap(":9.png"));
    m_pTButtonFriend21->setIconSize(QPixmap(":9.png").size());
    m_pTButtonFriend21->setAutoRaise(true);
    m_pTButtonFriend21->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);


    m_pVBoxFriend->setMargin(10);
    m_pVBoxFriend->setAlignment(Qt::AlignLeft);
    m_pVBoxFriend->addWidget(m_pTButtonFriend01);
    m_pVBoxFriend->addWidget(m_pTButtonFriend02);
    m_pVBoxFriend->addWidget(m_pTButtonFriend03);
    m_pVBoxFriend->addWidget(m_pTButtonFriend04);
    m_pVBoxFriend->addWidget(m_pTButtonFriend05);
    /* 这里addStretch是为了让QQ图标尽量往上排列，比如陌生人中只有一人的情况等 */
    m_pVBoxFriend->addStretch();

    m_pVBoxStranger->setMargin(10);
    m_pVBoxStranger->setAlignment(Qt::AlignLeft);
    m_pVBoxStranger->addWidget(m_pTButtonFriend10);
    m_pVBoxStranger->addWidget(m_pTButtonFriend11);
    m_pVBoxStranger->addStretch();

    m_pVBoxBlackList->setMargin(10);
    m_pVBoxBlackList->setAlignment(Qt::AlignLeft);
    m_pVBoxBlackList->addWidget(m_pTButtonFriend20);
    m_pVBoxBlackList->addWidget(m_pTButtonFriend21);
    m_pVBoxBlackList->addStretch();

    this->addItem((QWidget*)m_pGroupBoxFriend, tr("我的好友"));
    this->addItem((QWidget*)m_pGroupBoxStranger, tr("陌生人"));
    this->addItem((QWidget*)m_pGroupBoxBlackList, tr("黑名单"));
}

MyDrawer::~MyDrawer()
{

}

