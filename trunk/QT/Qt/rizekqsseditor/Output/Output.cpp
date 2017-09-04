#include "Output/Output.h"
#include "QClipboard"
#include "QSettings"
#include "QPalette"

outputWindow::outputWindow(QWidget *parent, Qt::WindowFlags flags)
: QDockWidget(parent, flags)
{
	setupUi(this);
	outputTextEdit->setReadOnly(true);
	QSettings settings("Rizek", "Rizek Qss Editor");
	QPalette newPal(outputTextEdit->palette());
	newPal.setColor(QPalette::Highlight, QColor(settings.value("TextEdit\HC",QColor("#468dd4")).toString()));
	newPal.setColor(QPalette::HighlightedText,QColor(settings.value("TextEdit\HTC",QColor("#ffffff")).toString()));
	outputTextEdit->setPalette(newPal);

	connect(comboBox, SIGNAL(activated(int)), this, SLOT(doActionFromComboBox(int)));
	connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(findInTheText(QString)));
	connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(findNext()));
}

outputWindow::~outputWindow()
{

}
void outputWindow::addItem(QString text)
{
	outputTextEdit->appendPlainText(text);
}
void outputWindow::doActionFromComboBox(int a)
{
	if (a==0)
	{
		QClipboard *clipboard = QApplication::clipboard();
		clipboard->setText(outputTextEdit->toPlainText());
	}
	if (a==1)
	{
		outputTextEdit->clear();
	}
}
void outputWindow::findNext()
{
	bool is = true;
	is = outputTextEdit->find(lineEdit->text(), QTextDocument::FindBackward);
	if (!is)
	{
		outputTextEdit->moveCursor(QTextCursor::End);
		outputTextEdit->find(lineEdit->text(), QTextDocument::FindBackward);
	}
}
void outputWindow::findInTheText(const QString & text)
{
	outputTextEdit->moveCursor(QTextCursor::End);
	outputTextEdit->find(lineEdit->text(), QTextDocument::FindBackward);
}
