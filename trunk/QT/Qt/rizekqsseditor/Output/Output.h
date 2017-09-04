#ifndef OUTPUT
#define OUTPUT

#include "ui_output.h"
#include <QMainWindow>

class outputWindow : public QDockWidget, public Ui::OutputWindow
{
	Q_OBJECT
public:
    outputWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~outputWindow();
	void addItem(QString);
private slots:
	void doActionFromComboBox(int);
	void findInTheText(const QString & text);
	void findNext();
};
#endif
