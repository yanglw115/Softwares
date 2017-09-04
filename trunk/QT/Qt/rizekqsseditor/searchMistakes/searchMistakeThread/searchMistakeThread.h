#ifndef SEARCHMISTAKESTHREAD
#define SEARCHMISTAKESTHREAD

#include <QDockWidget>
#include "QPlainTextEdit"
#include "QListWidget"
#include "QThread"
#include "QString"
#include "Output/Output.h"

class searchMistakesThread : public QThread
{
	Q_OBJECT
public:
	searchMistakesThread();
	~searchMistakesThread();
	void run();

	void setObjects(QPlainTextEdit*,QListWidget*,outputWindow*);
	void setKindOfSearch(int);
private:
	QString text;
	QPlainTextEdit * plainTextEdit;
	QListWidget * listWidget;
	outputWindow * output;
	int kindOfSearch;

	void findLightMistakes();
	void findMidleMistakes();
	void findStrongMistakes();
signals:
	void finished();

	void toOutput(QString);
	void toMistakesList(QString);
public slots:
	void getText(QString);
};
#endif
