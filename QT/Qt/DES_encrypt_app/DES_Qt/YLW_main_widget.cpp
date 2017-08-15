#include "YLW_main_widget.h"
#include <QLabel>

MainWidget::MainWidget(QWidget *parent)
    : QTabWidget(parent)
    , m_bOpenDatabaseOper(false)
{
    this->resize(600, 500);
    m_pEncryptWidget = new DESEncryptWidget;
    m_pDecryptWidget = new DESDecryptWidget;
    this->addTab(m_pEncryptWidget, tr("DES加密"));
    this->addTab(m_pDecryptWidget, tr("DES解密"));
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
        this->setMinimumSize(600, 500);
        if (!this->isMaximized()) {
            this->resize(600, 500);
        }
    } else {
        m_bOpenDatabaseOper = false;
        this->setMinimumSize(600, 300);
        if (!this->isMaximized()) {
            this->resize(600, 300);
        }
    }
}
