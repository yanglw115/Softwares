#ifndef SEARCHMISTAKES
#define SEARCHMISTAKES

#include "ui_searchMistakes.h"

#include <QDockWidget>
#include "QPlainTextEdit"
#include "searchMistakeThread/searchMistakeThread.h"

class searchMistakes : public QDockWidget, public Ui::searchMistakesWindow
{
	Q_OBJECT
public:
    searchMistakes(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~searchMistakes();
	void setTextEdit(QPlainTextEdit*);
	void findMistakes();

	searchMistakesThread * getThread(){return thread;}
private:
	QPlainTextEdit * plainTextEdit;
	searchMistakesThread * thread;
public slots:
	void refreash();
	void slotAddToListWidget(QString);
signals:
	void setTextToThread(QString);
};
#endif
