#ifndef CURESALARY_H
#define CURESALARY_H

#include <QWidget>
#include <QTableView>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QtXlsx/QtXlsx>

#include "xlsx/xlsx_sheet_model.h"

QTXLSX_USE_NAMESPACE

class CureSalary : public QWidget
{
    Q_OBJECT
public:
    explicit CureSalary(QWidget *parent = 0);
    virtual ~CureSalary();

signals:

private slots:
    void slotOpenExcel();
    void slotDoTableViewFilter(const QString &strText);
public slots:
private:
    void initWidgets();
    void freeXlsxDocument();
    void saveSalaryExcelHead(Worksheet *pWorksheet, const int nStartRow);
private:
    /* open excel */
    QLabel *m_pLabelOpenExcel;
    QLineEdit *m_pEditOpenedExcel;
    QPushButton *m_pButtonOpenExcel;

    /* tableview filter */
    QLabel *m_pLabelFilter;
    QLineEdit *m_pEditFilter;
    QHBoxLayout *m_pHLayoutFilter;

    QTableView *m_pTableExcel;
    Document *m_pXlsxDoc;

    QVBoxLayout *m_pVLayoutMain;
    QHBoxLayout *m_pHLayoutOpenExcel;
    bool m_bStateOpenExcel;

};

#endif // CURESALARY_H
