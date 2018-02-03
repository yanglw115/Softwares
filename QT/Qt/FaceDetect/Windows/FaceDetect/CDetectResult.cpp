#include "CDetectResult.h"

CDetectResult::CDetectResult(QWidget * parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    this->resize(800, 600);
    initWindow();
}


CDetectResult::~CDetectResult()
{

}

void CDetectResult::initWindow()
{
    m_pHLayoutMain = new QHBoxLayout;
    m_pVLayoutLeft = new QVBoxLayout;
    m_pVLayoutRight = new QVBoxLayout;
    m_pLabelResult = new QLabel(tr("检测项目"), this);
    m_pLabelDetails = new QLabel(tr("结果详情"), this);
    m_pLineLeft = new QFrame(this);
    m_pLineLeft->setFrameShape(QFrame::HLine);
    m_pLineRight = new QFrame(this);
    m_pLineRight->setFrameShape(QFrame::HLine);
    m_pLineMiddle = new QFrame(this);
    m_pLineMiddle->setFrameShape(QFrame::VLine);
    m_pListWidget = new QListWidget(this);
    m_pListWidget->setSpacing(5);
    m_pListWidget->addItem(tr("痘痘"));
    m_pListWidget->addItem(tr("肤色"));
    m_pListWidget->addItem(tr("黑头"));
    m_pListWidget->addItem(tr("毛孔粗大度"));
    m_pListWidget->addItem(tr("皮肤光滑度"));
    //m_pListWidget->item(0)->setHidden(true);
    CResultDetail *pDetail = new CResultDetail(TYPE_PORE, this);
    m_pStacked = new QStackedWidget(this);
    m_pStacked->addWidget(pDetail);
    pDetail->setImagePath(":/images/photo.jpg");

    m_pVLayoutLeft->addWidget(m_pLabelResult, 0, Qt::AlignHCenter);
    m_pVLayoutLeft->addWidget(m_pLineLeft);
    m_pVLayoutLeft->addWidget(m_pListWidget);

    m_pVLayoutRight->addWidget(m_pLabelDetails, 0, Qt::AlignHCenter);
    m_pVLayoutRight->addWidget(m_pLineRight);
    m_pVLayoutRight->addWidget(m_pStacked);

    m_pHLayoutMain->addLayout(m_pVLayoutLeft);
    m_pHLayoutMain->addWidget(m_pLineMiddle);
    m_pHLayoutMain->addLayout(m_pVLayoutRight, 1);
    this->setLayout(m_pHLayoutMain);
}
