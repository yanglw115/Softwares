#ifndef DESENCRYPTWIDGET_H
#define DESENCRYPTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLayout>
#include <QSpacerItem>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QtDebug>

#include "YLW_file_line_edit.h"
#include "YLW_sql_table_view.h"
#include "YLW_mycombobox.h"

class DESEncryptWidget: public QWidget
{
    Q_OBJECT
public:
    explicit DESEncryptWidget(QWidget *parent = 0);
    virtual ~DESEncryptWidget();

signals:
    void sigOperDatabase(bool);

protected:

private slots:
    void slotBrowseFile();
    void slotMD5ForFile();
    void slotChangeFileName(const QString &);
    void slotCopyFileName();
    void slotKeyReset();
    void slotKeyClear();
    void slotPlainCopy();
    void slotPlainClear();
    void slotCipherCopy();
    void slotCipherClear();
    void slotEncrypt();
    void slotClearAll();
    void slotShowDatabaseOper(int state);

    void slotKeyChanged(const QString &strKey);
    void slotPlainChanged(const QString &strPlain);

private:
    QLabel *m_pLabelFile;
    FileLineEdit *m_pLineEditFile;
    QPushButton *m_pButtonBrowse;
    QPushButton *m_pButtonMD5;

    QLineEdit *m_pLineEditFileName;
    QPushButton *m_pButtonFileNameCopy;

    QLabel *m_pLabelKey;
    QLineEdit *m_pLineEditKey;
    QPushButton *m_pButtonKeyReset;
    QPushButton *m_pButtonKeyClear;

    QLabel *m_pLabelPlainInput;
    QLineEdit *m_pLineEditPlainInput;
    QPushButton *m_pButtonPlainClear;
    QPushButton *m_pButtonPlainCopy;

    QLabel *m_pLabelCipherOutput;
    QLineEdit *m_pLineEditCipherOutput;
    QPushButton *m_pButtonCipherClear;
    QPushButton *m_pButtonCipherCopy;

    QPushButton *m_pButtonEncrypt;
    QPushButton *m_pButtonClearAll;
    QCheckBox *m_pCheckBoxOper;

    /* 数据库视图 */
    SQLOperateWidget *m_pWidgetSqlOper;

    QGridLayout *m_pGridLayout;

};

#endif // DESENCRYPTWIDGET_H
