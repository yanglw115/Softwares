#include <QtDebug>

#include "main_widget.h"
#include "vs_char_set.h"

MainWidget::MainWidget(QWidget *parent)
    : QTabWidget(parent)
{
    this->setFixedSize(360, 400);

    m_pHeightWidget = new HeightWidget(this);
    m_pFatWidget = new FatWidget(this);
    m_pG4Widget = new G4Widget(this);

    this->addTab(m_pHeightWidget, tr("身高测量校正"));
    this->addTab(m_pFatWidget, tr("体脂测量检测"));
    this->addTab(m_pG4Widget, tr("4G网卡配置"));

    /* 注意，这里一定要加这句话，不然测量界面由于没有父窗口，导致主窗口关闭时，测量界面不会关闭 */
    this->setAttribute(Qt::WA_DeleteOnClose);
}

MainWidget::~MainWidget()
{

}

