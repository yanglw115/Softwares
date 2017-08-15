#include "ccontent.h"

CContent::CContent(QWidget *parent)
    : QFrame(parent)
{
    m_pBaseInfo = new CBasicInfo(this);
    m_pConnection = new CConnectiions(this);
    m_pDetailInfo = new CDetailInfo(this);

    /* 这里的两个按钮是与QStackedWidget并行的，上下位置关系 */
    m_pButtonModify = new QPushButton(tr("修改"), this);
    m_pButtonClose = new QPushButton(tr("关闭"), this);
    m_pHBoxButton = new QHBoxLayout;
    /* 这里是为了添加一个可拉伸的space空间到后面两个按钮的前面，使按钮靠右放置 */
    m_pHBoxButton->addStretch(1);
    m_pHBoxButton->addWidget(m_pButtonModify);
    m_pHBoxButton->addWidget(m_pButtonClose);

    /* 定义一个QStackedWidget */
    m_pStackWidget = new QStackedWidget(this);
    m_pStackWidget->addWidget(m_pBaseInfo);
    m_pStackWidget->addWidget(m_pConnection);
    m_pStackWidget->addWidget(m_pDetailInfo);

    m_pVBoxAll = new QVBoxLayout;
    m_pVBoxAll->setMargin(10);
    m_pVBoxAll->setSpacing(10);
    m_pVBoxAll->addWidget(m_pStackWidget);
    m_pVBoxAll->addLayout(m_pHBoxButton);

    this->setLayout(m_pVBoxAll);
}

CContent::~CContent()
{

}

void CContent::slotSetCurrentStackWidget(int index)
{
    int length = m_pStackWidget->count();
    if (index >=0 && index < length) {
        m_pStackWidget->setCurrentIndex(index);
    }
}

