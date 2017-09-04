#ifndef ABOUTPROGRAM
#define ABOUTPROGRAM

#include "ui_aboutprogram.h"

class aboutProgram : public QDialog, public Ui::About_Program_Dialog
{
	Q_OBJECT
public:
    aboutProgram(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~aboutProgram();
protected slots:
	void goToWebSite();
	void goToOurEmail();
};
#endif
