#include "updater.h"
#include "QNetworkAccessManager"
#include "QNetworkRequest"
#include "QNetworkReply"
#include "QVariant"
//#include "QHttp"
#include "QFile"
#include "QIODevice"
#include "QTextStream"
#include "QSettings"
#include "QDir"
#include "QUrl"
#include "QDesktopServices"
#include "QDebug"
#include "QMessageBox"

RUpdater::RUpdater(QWidget *parent, Qt::WindowFlags flags)
	: QDialog(parent, flags)
{
	setupUi(this);
	QSettings settings("Rizek", "Rizek Qss Editor");
	if (settings.value("this\UseStyleSheet",true).toBool())
	{
		QFile file(":/Other/qss/default.qss");
		file.open(QFile::ReadOnly);
		QString styleSheet = QLatin1String(file.readAll());
		this->setStyleSheet(styleSheet);
	}
}
void RUpdater::setInformation()
{
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(InformationReplyFinished(QNetworkReply*)));
	QNetworkReply * networkReply = manager->get(QNetworkRequest(QUrl("http://rizek.ucoz.ru/RizekQssEditor/RizekQssEditorRefresh.ini")));
	label_9->setText(tr("Add file with information of new versions."));
}

RUpdater::~RUpdater()
{

}
void RUpdater::InformationReplyFinished(QNetworkReply* qnr)
{
	QFile file("RizekQssEditorRefresh.ini");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	file.write(qnr->readAll());
	file.close();
	label_9->setText(tr("File with the new information received."));
	openInformationData();
	connect(pushButton_3, SIGNAL(clicked()), this, SLOT(goToUrl()));
	connect(pushButton_4, SIGNAL(clicked()), this, SLOT(goToUrl()));
}
void RUpdater::openInformationData()
{
	QSettings settings(QDir::currentPath() + "\\RizekQssEditorRefresh.ini",QSettings::IniFormat);
	label_2->setText(currentVersion);
	label_4->setText(settings.value("NewVersion").toString());
	label_11->setText(settings.value("WhatsNew").toString());
	label_6->setText(settings.value("UrlWin").toString());
	label_8->setText(settings.value("UrlLin").toString());
	if (currentVersion<settings.value("NewVersion").toString())
		this->show();
	else
	{
		QMessageBox msgBox;
		msgBox.setText("There are no new version of Rizek Qss Editor");

		QFile file(":/Other/qss/default.qss");
		file.open(QFile::ReadOnly);
		QString styleSheet = QLatin1String(file.readAll());
		msgBox.setStyleSheet(styleSheet);

		QIcon ic;
		ic.addFile(":/ico/icos/update.png");
		msgBox.setWindowIcon(ic);

		msgBox.exec();
	}
}
void RUpdater::goToUrl()
{
	if (sender()==pushButton_3)
	{
		QUrl url;
		url.setUrl(label_6->text());
		QDesktopServices::openUrl(url);
	}
	if (sender()==pushButton_4)
	{
		QUrl url;
		url.setUrl(label_8->text());
		QDesktopServices::openUrl(url);
	}
}
