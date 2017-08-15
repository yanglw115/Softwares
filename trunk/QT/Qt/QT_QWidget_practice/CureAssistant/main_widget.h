#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QTabWidget>

#include "height_widget.h"
#include "fat_widget.h"
#include "g4_widget.h"

class MainWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);
    virtual ~MainWidget();

private:
    HeightWidget *m_pHeightWidget;
    FatWidget *m_pFatWidget;
    G4Widget *m_pG4Widget;
};

#endif // MAINWIDGET_H
