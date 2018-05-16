#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QStackedWidget>
#include <QLayout>
#include <QResizeEvent>

#include "cure_salary.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void initMainWindow();
protected:
    virtual void resizeEvent(QResizeEvent * event);

private slots:
private:
    QWidget *m_pWidgetOffice;
    QListWidget *m_pListOffice;
    QStackedWidget *m_pStackedOffice;
    QHBoxLayout *m_pHLayoutOffice;
    CureSalary *m_pWidgetSalary;
};

#endif // MAINWINDOW_H