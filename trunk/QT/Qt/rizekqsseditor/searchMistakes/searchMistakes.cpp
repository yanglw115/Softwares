#include "searchMistakes.h"
#include "QString"
#include "QSettings"
#include "searchMistakeThread/searchMistakeThread.h"
#include "rizekqsseditor.h"

#include "QDebug"

searchMistakes::searchMistakes(QWidget *parent, Qt::WindowFlags flags)
: QDockWidget(parent, flags)
{
	setupUi(this);
	connect(pushButton, SIGNAL(clicked()), this, SLOT(refreash()));
	thread = new searchMistakesThread;
}

searchMistakes::~searchMistakes()
{
	thread->quit();
}
void searchMistakes::setTextEdit(QPlainTextEdit* te)
{
	plainTextEdit = te;
}
void searchMistakes::findMistakes()
{
	try
	{
		thread = new searchMistakesThread;

		connect(thread, SIGNAL(finished()), thread, SLOT(quit()));
		
		connect(this, SIGNAL(setTextToThread(QString)), thread, SLOT(getText(QString)));
		emit setTextToThread(plainTextEdit->toPlainText());

		RizekQssEditor * rqe = qobject_cast<RizekQssEditor*>(parent());
		rqe->getOutputWindow()->addItem(tr("---Search Mistakes---"));
		rqe->getOutputWindow()->addItem(tr("Create thread of search mistakes"));

		connect(thread, SIGNAL(toMistakesList(QString)), this, SLOT(slotAddToListWidget(QString)));
		connect(thread, SIGNAL(toOutput(QString)), rqe , SLOT(slotAddToOutput(QString)));

		thread->setObjects(plainTextEdit,listWidget,rqe->getOutputWindow());
		thread->start();

	}
	catch(...)
	{
		qDebug() << "Error";
	}
}
void searchMistakes::refreash()
{
	listWidget->clear();
	findMistakes();
}
void searchMistakes::slotAddToListWidget(QString str)
{
	listWidget->addItem(str);
}
