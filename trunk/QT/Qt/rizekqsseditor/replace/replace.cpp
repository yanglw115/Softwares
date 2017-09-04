#include "replace.h"
#include "QFile"
#include "QSettings"


replace::replace(QWidget *parent, Qt::WindowFlags flags)
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
	ic.addFile(":/ico/icos/replace.png");
	this->setWindowIcon(ic);
	connect(pushButton, SIGNAL(clicked()), this, SLOT(replaceInTheText()));
}

replace::~replace()
{
	
}
void replace::setPlaintTextEdit(qssTextEdit * textEdit)
{
	plainTextEdit = textEdit;
}
void replace::replaceInTheText()
{
	if (lineEdit->text()!=lineEdit_2->text())
	{
		QString text = plainTextEdit->toPlainText();
		text = text.replace(lineEdit->text(),lineEdit_2->text());
		plainTextEdit->setPlainText(text);
	}
}
