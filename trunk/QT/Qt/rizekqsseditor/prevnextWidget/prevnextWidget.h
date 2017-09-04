#ifndef PREVNEXTWIDGET
#define PREVNEXTWIDGET

#include <QDockWidget>
#include "ui_prevnextWidget.h"
#include "QPlainTextEdit"

class prevnextWidget : public QWidget, public Ui::prevNext
{
	Q_OBJECT
public:
    prevnextWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~prevnextWidget();
};
#endif
