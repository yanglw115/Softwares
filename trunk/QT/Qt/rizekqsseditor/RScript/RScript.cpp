#include "RScript.h"
#include "rizekqsseditor.h"
#include "QClipboard"
#include "QSettings"
#include "QPalette"
#include "QtScript"
#include "QDebug"

scriptWindow::scriptWindow(QWidget *parent, Qt::WindowFlags flags)
: QDockWidget(parent, flags)
{
	setupUi(this);

	QSettings settings("Rizek", "Rizek Qss Editor");
	QPalette newPal(scriptTextEdit->palette());
	newPal.setColor(QPalette::Highlight, QColor(settings.value("TextEdit\HC",QColor("#468dd4")).toString()));
	newPal.setColor(QPalette::HighlightedText,QColor(settings.value("TextEdit\HTC",QColor("#ffffff")).toString()));
	scriptTextEdit->setPalette(newPal);


	connect(comboBox, SIGNAL(activated(int)), this, SLOT(doActionFromComboBox(int)));
	connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(findInTheText(QString)));
	connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(findNext()));
	connect(pushButton, SIGNAL(clicked()), this, SLOT(runCurrentScript()));
}

scriptWindow::~scriptWindow()
{

}
void scriptWindow::configure()
{
	RizekQssEditor * rqe = qobject_cast<RizekQssEditor*>(parent());
 	scriptPlainTextEdit = scriptEngine.newQObject(rqe->getPlainTextEdit());
 	rqe->getPlainTextEdit()->setPlainText("dsg");
 	scriptEngine.globalObject().setProperty("textEdit",scriptPlainTextEdit);
}
void scriptWindow::runCurrentScript()
{
	QString text = scriptTextEdit->toPlainText();
	scriptEngine.evaluate(text);
}
void scriptWindow::doActionFromComboBox(int a)
{
	if (a==0)
	{
		QClipboard *clipboard = QApplication::clipboard();
		clipboard->setText(scriptTextEdit->toPlainText());
	}
	if (a==1)
	{
		scriptTextEdit->clear();
	}
}
void scriptWindow::findNext()
{
	bool is = true;
	is = scriptTextEdit->find(lineEdit->text(), QTextDocument::FindBackward);
	if (!is)
	{
		scriptTextEdit->moveCursor(QTextCursor::End);
		scriptTextEdit->find(lineEdit->text(), QTextDocument::FindBackward);
	}
}
void scriptWindow::findInTheText(const QString & text)
{
	scriptTextEdit->moveCursor(QTextCursor::End);
	scriptTextEdit->find(lineEdit->text(), QTextDocument::FindBackward);
}
