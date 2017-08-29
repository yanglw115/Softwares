#include "YLW_main_widget.h"
#include "YLW_VS_char_set.h"

#include <QLabel>

MainWidget::MainWidget(QWidget *parent)
    : QTabWidget(parent)
    , m_bOpenDatabaseOper(false)
{
    this->resize(700, 720);
    m_pEncryptWidget = new DESEncryptWidget;
    m_pDecryptWidget = new DESDecryptWidget;
    m_pHttpRequestTools = new CHttpReqeustTools;
    this->addTab(m_pEncryptWidget, tr("DES加密"));
    this->addTab(m_pDecryptWidget, tr("DES解密"));
    this->addTab(m_pHttpRequestTools, tr("HttpTools"));
    /* 打开数据库操作对话框时，改变整个窗口大小 */
    connect(m_pEncryptWidget, SIGNAL(sigOperDatabase(bool)), SLOT(slotChangeSize(bool)));

}

MainWidget::~MainWidget()
{

}

void MainWidget::slotChangeSize(bool bOpenDatabaseOper)
{
    if (bOpenDatabaseOper) {
        m_bOpenDatabaseOper = true;
        this->setMinimumSize(700, 720);
        if (!this->isMaximized()) {
            this->resize(700, 720);
        }
    } else {
        m_bOpenDatabaseOper = false;
        this->setMinimumSize(700, 300);
        if (!this->isMaximized()) {
            this->resize(700, 300);
        }
    }
}
