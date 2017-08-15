#ifndef YLW_MAIN_WIDGET_H
#define YLW_MAIN_WIDGET_H

#include <QWidget>
#include <QTabWidget>

#include "YLW_des_encrypt_widget.h"
#include "YLW_des_decrypt_widget.h"

class MainWidget : public QTabWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();
private slots:
    void slotChangeSize(bool);
private:
    DESEncryptWidget *m_pEncryptWidget;
    DESDecryptWidget *m_pDecryptWidget;
    bool m_bOpenDatabaseOper;
};

#endif // YLW_MAIN_WIDGET_H
