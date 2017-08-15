#ifndef YLW_MAIN_WINDOW_H
#define YLW_MAIN_WINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // YLW_MAIN_WINDOW_H
