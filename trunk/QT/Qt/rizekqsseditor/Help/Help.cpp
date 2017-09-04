#include "Help.h"
#include "QFile"
#include "QSettings"
#include "QDebug"


RHelp::RHelp(QWidget *parent, Qt::WindowFlags flags)
: QMainWindow(parent, flags)
{
	setupUi(this);
	//------
	QIcon ic;
	ic.addFile(":/ico/icos/Help.png");
	this->setWindowIcon(ic);

	QSettings settings("Rizek", "Rizek Qss Editor");
	if (settings.value("this\UseStyleSheet").toBool())
	{
		QFile file(":/Other/qss/default.qss");
		file.open(QFile::ReadOnly);
		QString styleSheet = QLatin1String(file.readAll());
		this->setStyleSheet(styleSheet);
	}
	connect(listWidget, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(selectHtml(QListWidgetItem*)));
	this->horizontalLayout->setStretch(1,1);
}

RHelp::~RHelp()
{
	
}
void RHelp::selectHtml(QListWidgetItem* item)
{
	if (listWidget->currentRow()==0)
	{
		QFile file(":/Help/Documentation/Actions documentation.html");
		file.open(QFile::ReadOnly);
		QString text = QLatin1String(file.readAll());
		textBrowser->setText(text);
	}
	if (listWidget->currentRow()==1)
	{
		QFile file(":/Help/Documentation/Script Documentation.html");
		file.open(QFile::ReadOnly);
		QString text = QLatin1String(file.readAll());
		textBrowser->setText(text);
	}
	if (listWidget->currentRow()==2 || listWidget->currentRow()==3 || listWidget->currentRow()==4)
	{
		QFile file(":/Help/Documentation/Actions documentation.html");
		file.open(QFile::ReadOnly);
		QString text = QLatin1String(file.readAll());
		textBrowser->setText(text);
		textBrowser->scrollToAnchor("Tools");
	}
	if (listWidget->currentRow()==5)
	{
		QFile file(":/Help/Documentation/Hidden possibilities.html");
		file.open(QFile::ReadOnly);
		QString text = QLatin1String(file.readAll());
		textBrowser->setText(text);
	}
}
