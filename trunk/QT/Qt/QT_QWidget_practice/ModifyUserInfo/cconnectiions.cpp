#include "cconnectiions.h"

CConnectiions::CConnectiions(QWidget *parent) : QWidget(parent)
{
    m_pLabelEmail = new QLabel(tr("电子邮件:"), this);
    m_pLineEditEmail = new QLineEdit(this);
    m_pLabelAddress = new QLabel(tr("联系地址:"), this);
    m_pLineEditAddress = new QLineEdit(this);
    m_pLabelPostNumber = new QLabel(tr("邮政编码:"), this);
    m_pLineEditPostNumber = new QLineEdit(this);
    m_pLabelPhoneNumber = new QLabel(tr("移动电话:"), this);
    m_pLineEditPhoneNumber = new QLineEdit(this);
    m_pCheckGetSaveMessage = new QCheckBox(tr("接收留言"), this);
    m_pLabelFixNumber = new QLabel(tr("固定电话:"), this);
    m_pLineEditFixNumber = new QLineEdit(this);

    m_pGridLayout = new QGridLayout;
    m_pGridLayout->addWidget(m_pLabelEmail, 0, 0);
    m_pGridLayout->addWidget(m_pLineEditEmail, 0, 1);
    m_pGridLayout->addWidget(m_pLabelAddress, 1, 0);
    m_pGridLayout->addWidget(m_pLineEditAddress, 1, 1);
    m_pGridLayout->addWidget(m_pLabelPostNumber, 2, 0);
    m_pGridLayout->addWidget(m_pLineEditPostNumber, 2, 1);
    m_pGridLayout->addWidget(m_pLabelPhoneNumber, 3, 0);
    m_pGridLayout->addWidget(m_pLineEditPhoneNumber, 3, 1);
    m_pGridLayout->addWidget(m_pCheckGetSaveMessage, 3, 2);
    m_pGridLayout->addWidget(m_pLabelFixNumber, 4, 0);
    m_pGridLayout->addWidget(m_pLineEditFixNumber, 4, 1);

    this->setLayout(m_pGridLayout);
}

CConnectiions::~CConnectiions()
{
}

