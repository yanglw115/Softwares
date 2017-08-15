#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QDialog>

class myWindow : public QDialog
{
    Q_OBJECT

public:
    myWindow(QWidget *parent = 0);
    ~myWindow();
};

#endif // MYWINDOW_H
