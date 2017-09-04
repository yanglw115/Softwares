#include "ConnectedResourcres.h"
#include "QString"
#include "qDebug"
#include "QSettings"
#include "QFile"
#include "QFileInfo"
#include "QFileDialog"
#include "QProcess"
#include "QResource"

RConnectedResource::RConnectedResource(QWidget *parent, Qt::WindowFlags flags)
: QDockWidget(parent, flags)
{
	setupUi(this);
	connect(comboBox, SIGNAL(activated(int)), this, SLOT(selectActionFromComboBox(int)));
}

RConnectedResource::~RConnectedResource()
{

}
void RConnectedResource::addNewResources()
{
	QString qrcFile = QFileDialog::getOpenFileName(this, "Select resource file", "/home/" , "Resource File (*.qrc)");
	listWidget->addItem(qrcFile);
}
void RConnectedResource::selectActionFromComboBox(int i)
{
	if (i==0)
		addNewResources();
	if (i==1)
		deleteCurrentResource();
	if (i==2)
		deleteAllResources();
}
void RConnectedResource::deleteCurrentResource()
{
	delete listWidget->currentItem();
}
void RConnectedResource::deleteAllResources()
{
	listWidget->clear();
}
