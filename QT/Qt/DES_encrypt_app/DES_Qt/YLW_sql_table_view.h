#ifndef SQLTABLEVIEW_H
#define SQLTABLEVIEW_H

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QLayout>
#include <QFileDialog>
#include <QItemSelectionModel>

#include "YLW_file_line_edit.h"
#include "YLW_my_table_view.h"

class SQLOperateWidget: public QWidget
{
    Q_OBJECT
public:
    explicit SQLOperateWidget(QWidget *parent = 0);
    virtual ~SQLOperateWidget();
    void setSqlDatabaseFile(const QString &strFilePath);

private slots:
    void slotSetCurrentTable(const QString &strTableName);
    void slotBrowseSqlFile();
    void slotSetSqlDatabaseFile(const QString &strFilePath);
    void slotRereshCurrentTable();
    void slotSaveChanges();
    void slotAddNewRowRecord();
    void slotDeleteRowRecord();
    void slotSqlModelDataChanged();

protected:
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);

signals:

private:
    void initDatabase();
    bool openDatabase();
    void closeDatabase();
    void initTableViewShow();
    void setButtonDeleteEnable();
private:
    QString m_strSqlFilePath;
    QString m_strCurrentTable;
    QSqlDatabase m_database;
    QSqlTableModel *m_pSqlTableModel;
    MyTableView *m_pTableView;
    bool m_bOpenDatabase;

    QLabel *m_pLabelSqlFile;
    FileLineEdit *m_pLineEditSqlFile;
    QPushButton *m_pButtonSqlFileBrowse;

    QLabel *m_pLabelDataTable;
    QComboBox *m_pComboBoxDataTable;
    QPushButton *m_pButtonRefresh;
    QPushButton *m_pButtonSaveChanges;
    QPushButton *m_pButtonNewRecord;
    QPushButton *m_pButtonDeleteRecord;
    QVBoxLayout *m_pVLayoutSqlOper;
};

#endif // SQLTABLEVIEW_H
