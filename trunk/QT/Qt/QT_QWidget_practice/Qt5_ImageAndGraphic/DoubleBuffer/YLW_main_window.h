#ifndef YLW_MAIN_WINDOW_H
#define YLW_MAIN_WINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QToolButton>
#include <QColorDialog>

#include "YLW_draw_widget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slotChangeColor();
    void slotChangeWidth(int);
    void slotChangeStyle(int);
    void slotClearDraw();

private:
    DrawWidget *m_pDrawWidget;
    QToolBar *m_pToolBar;
    QLabel *m_pLabelStyle;
    QLabel *m_pLabelWidth;
    QComboBox *m_pComboBoxStyle;
    QSpinBox *m_pSpinBoxWidth;
    QToolButton *m_pButtonColor;
    QToolButton *m_pButtonClear;
};

#endif // YLW_MAIN_WINDOW_H
