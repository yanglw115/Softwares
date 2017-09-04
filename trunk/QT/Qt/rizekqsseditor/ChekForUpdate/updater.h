#ifndef UPDATER_H
#define UPDATER_H

#include <QMainWindow>
#include "ui_updater.h"
#include <QNetworkReply>
#include "QString"

class RUpdater : public QDialog, Ui::UpdaterClass
{
	Q_OBJECT

public:
    RUpdater(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~RUpdater();
	void setInformation();
	void setCurrentVersion(QString cv) { currentVersion = cv;}
private slots:
	void InformationReplyFinished(QNetworkReply*);
	void openInformationData();
	void goToUrl();
private:
	QString currentVersion;
};

#endif // UPDATER_H
