#include "prevnextWidget.h"
#include "QFile"

prevnextWidget::prevnextWidget(QWidget *parent, Qt::WindowFlags flags)
: QWidget(parent, flags)
{
	setupUi(this);
	//QSS
	QFile file(":/Other/qss/prevNextStyleSheet.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	this->setStyleSheet(styleSheet);
}

prevnextWidget::~prevnextWidget()
{

}

