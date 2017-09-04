#include "aboutProgram.h"
#include "QFile"
#include "QUrl"
#include "QDesktopServices"
#include "QSettings"

aboutProgram::aboutProgram(QWidget *parent, Qt::WindowFlags flags)
: QDialog(parent, flags)
{
	setupUi(this);
	tabWidget->setTabText(0,tr("About program and us"));
	tabWidget->setTabText(1,tr("Using components"));
	QSettings settings("Rizek", "Rizek Qss Editor");
	if (settings.value("this\UseStyleSheet",true).toBool())
	{
		QFile file(":/Other/qss/default.qss");
		file.open(QFile::ReadOnly);
		QString styleSheet = QLatin1String(file.readAll());
		this->setStyleSheet(styleSheet);
	}
	tabWidget->setTabText(2,tr("Thanks"));
	QIcon ic;
	ic.addFile(":/ico/icos/home.png");
	this->setWindowIcon(ic);
	connect(pushButton_2, SIGNAL(clicked()), this, SLOT(goToWebSite()));
	connect(pushButton_3, SIGNAL(clicked()), this, SLOT(goToOurEmail()));
}

aboutProgram::~aboutProgram()
{

}
void aboutProgram::goToWebSite()
{
	QUrl url;
	url.setUrl("https://sites.google.com/site/myrizek/");
	QDesktopServices::openUrl(url);
}
void aboutProgram::goToOurEmail()
{
	QUrl url;
	url.setUrl("mailto:ourrizek@gmail.com");
	QDesktopServices::openUrl(url);
}
