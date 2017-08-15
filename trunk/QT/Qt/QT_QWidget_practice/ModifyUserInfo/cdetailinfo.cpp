#include "cdetailinfo.h"

CDetailInfo::CDetailInfo(QWidget *parent) : QWidget(parent)
{
    m_strListCountry.clear();
    m_strListCountry.append(tr("中国"));
    m_strListCountry.append(tr("美国"));
    m_strListCountry.append(tr("德国"));
    m_strListCountry.append(tr("法国"));
    m_mapProvince.clear();
    m_mapProvince.insert(tr("中国"), QStringList()<< tr("湖北") << tr("广东") << tr("河南"));
    m_mapProvince.insert(tr("美国"), QStringList()<< tr("洛汕基") << tr("加洲") << tr("拉斯"));
    m_mapProvince.insert(tr("德国"), QStringList()<< tr("1") << tr("2") << tr("3"));
    m_mapProvince.insert(tr("法国"), QStringList()<< tr("a") << tr("b") << tr("c"));
    m_pLabelCountry = new QLabel(tr("国家/地址:"), this);
    m_pComboBoxCountry = new QComboBox(this);
    m_pComboBoxCountry->addItems(m_strListCountry);

    connect(m_pComboBoxCountry, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChangProvinceStringList(int)));
    m_pLabelProvince = new QLabel(tr("省份:"), this);
    m_pComboBoxProvince = new QComboBox(this);
    m_pComboBoxProvince->addItems(m_mapProvince[m_strListCountry.at(0)]);
    m_pLabelCity = new QLabel(tr("城市:"), this);
    m_pLineEditCity = new QLineEdit(this);
    m_pLabelPersonal = new QLabel(tr("个人说明:"), this);
    m_pTextEditPersonal = new QTextEdit(this);

    m_pGridLayout = new QGridLayout;
    m_pGridLayout->addWidget(m_pLabelCountry, 0, 0);
    m_pGridLayout->addWidget(m_pComboBoxCountry, 0, 1);
    m_pGridLayout->addWidget(m_pLabelProvince, 1, 0);
    m_pGridLayout->addWidget(m_pComboBoxProvince, 1, 1);
    m_pGridLayout->addWidget(m_pLabelCity, 2, 0);
    m_pGridLayout->addWidget(m_pLineEditCity, 2, 1);
    m_pGridLayout->addWidget(m_pLabelPersonal, 3, 0);
    m_pGridLayout->addWidget(m_pTextEditPersonal, 3, 1);

    this->setLayout(m_pGridLayout);
}

CDetailInfo::~CDetailInfo()
{

}

void CDetailInfo::slotChangProvinceStringList(int index)
{
    m_pComboBoxProvince->clear();
    m_pComboBoxProvince->addItems(m_mapProvince[m_strListCountry.at(index)]);
}

