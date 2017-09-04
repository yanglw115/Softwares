#include "search.h"
#include "QFile"
#include "QSettings"

search::search(QWidget *parent, Qt::WindowFlags flags)
: QDialog(parent, flags)
{
	setupUi(this);
	//------
	QSettings settings("Rizek", "Rizek Qss Editor");
	if (settings.value("this\UseStyleSheet").toBool())
	{
		QFile file(":/Other/qss/default.qss");
		file.open(QFile::ReadOnly);
		QString styleSheet = QLatin1String(file.readAll());
		this->setStyleSheet(styleSheet);
	}
	//------	
	QIcon ic;
	ic.addFile(":/ico/icos/find.png");
	this->setWindowIcon(ic);
	connect(pushButton, SIGNAL(clicked()), this, SLOT(findNext()));
	connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(findNext()));
}

search::~search()
{

}
void search::setPlaintTextEdit(qssTextEdit * textEdit)
{
	plainTextEdit = textEdit;
}
void search::findNext()
{
	bool is = true;
	is = plainTextEdit->find(lineEdit->text(), QTextDocument::FindBackward); 
	if (!is)
	{
		plainTextEdit->moveCursor(QTextCursor::End);
		plainTextEdit->find(lineEdit->text(), QTextDocument::FindBackward);
	}
}
