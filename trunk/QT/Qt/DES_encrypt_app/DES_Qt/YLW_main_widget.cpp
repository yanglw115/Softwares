#include "YLW_main_widget.h"
#include "YLW_VS_char_set.h"

#include <QLabel>
#include <QFile>
#include <QApplication>

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

void MainWidget::registerFileWatcher(const QString &strFilePath)
{
    m_pFileWatcher = new QFileSystemWatcher(this);
    if (m_pFileWatcher->addPath(strFilePath)) {
        qDebug() << "添加QSS文件监控成功: " << strFilePath;
    } else {
        qDebug() << "添加QSS文件监控失败: " << strFilePath;
    }
    connect(m_pFileWatcher, SIGNAL(fileChanged(QString)), this, SLOT(slotFileChanged(QString)));
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

void MainWidget::slotFileChanged(const QString &strFilePath)
{
    QFile fileQss(strFilePath);
    if (fileQss.open(QFile::ReadOnly)) {
        QTextStream textFile(&fileQss);
        QString strStyleSheet = textFile.readAll();
        qApp->setStyleSheet(strStyleSheet);
        fileQss.close();
    } else {
        qDebug() << "Open QSS file failed: " << strFilePath;
    }
}
