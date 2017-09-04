#ifndef HELP
#define HELP

#include "ui_help.h"

class RHelp : public QMainWindow, public Ui::HelpWindow
{
	Q_OBJECT
public:
    RHelp(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~RHelp();
private slots:
	void selectHtml(QListWidgetItem*);
};
#endif
