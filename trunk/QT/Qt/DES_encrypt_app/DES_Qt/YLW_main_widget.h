#ifndef YLW_MAIN_WIDGET_H
#define YLW_MAIN_WIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QFileSystemWatcher>

#include "YLW_des_encrypt_widget.h"
#include "YLW_des_decrypt_widget.h"
#include "YLW_http_reqeust_tools.h"

class MainWidget : public QTabWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();

    void registerFileWatcher(const QString &strFilePath);
public slots:
    void slotFileChanged(const QString &strFilePath);
private slots:
    void slotChangeSize(bool);    
private:
    DESEncryptWidget *m_pEncryptWidget;
    DESDecryptWidget *m_pDecryptWidget;
    CHttpReqeustTools *m_pHttpRequestTools;
    QFileSystemWatcher *m_pFileWatcher;
    bool m_bOpenDatabaseOper;
};

#endif // YLW_MAIN_WIDGET_H
