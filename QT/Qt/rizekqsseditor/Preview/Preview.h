#ifndef PREVIEW
#define PREVIEW

#include "ui_Preview.h"
#include "qssTextEdit/qssTextEdit.h"
#include <QMainWindow>
#include "QString"
#include "Output/Output.h"

class preview : public QMainWindow, public Ui::Preview_Window
{
	Q_OBJECT
public:
    preview(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~preview();
	void setTextOfEditor(QString txt);
	void setOutputWindow(outputWindow *);
	void setResorces(QStringList);
	void findClassesAndAddIt();
protected:
	QString text;
	QString file;
	outputWindow * ouptput;
	QStringList resorces;
protected slots:
	void createQMainWindow();
	void createQDialog();
	void createQDockWidget();
};
#endif
