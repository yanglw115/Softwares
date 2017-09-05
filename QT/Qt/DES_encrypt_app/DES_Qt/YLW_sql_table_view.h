#ifndef SQLTABLEVIEW_H
#define SQLTABLEVIEW_H

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QLabel>
#include <QPushButton>
#include <QLayout>
#include <QFileDialog>
#include <QItemSelectionModel>
#include <QGroupBox>
#include <QSpinBox>

#include "YLW_file_line_edit.h"
#include "YLW_my_table_view.h"
#include "YLW_mycombobox.h"

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
    void slotConvertTimeToBJ();
    void slotConvertTimeToUnix();

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
    CMyComboBox *m_pComboBoxDataTable;
    QPushButton *m_pButtonRefresh;
    QPushButton *m_pButtonSaveChanges;
    QPushButton *m_pButtonNewRecord;
    QPushButton *m_pButtonDeleteRecord;
    QVBoxLayout *m_pVLayoutSqlOper;

    QGroupBox *m_pGroupBoxStampConvert;
    QLabel *m_pLabelUnixBefore;
    QLineEdit *m_pLineEditUnixBefore;
    QPushButton *m_pButtonConvertToBJ;
    QLineEdit *m_pLineEditBJAfter;

    QLabel *m_pLabelBJBefore;
    QLineEdit *m_pLineEditBJBefore;
    QPushButton *m_pButtonConvertToUnix;
    QLineEdit *m_pLineEditUnixAfter;
    QPushButton *m_pButtonCopyUnix;

};

#endif // SQLTABLEVIEW_H
