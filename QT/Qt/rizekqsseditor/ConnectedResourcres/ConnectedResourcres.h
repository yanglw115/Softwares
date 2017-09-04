#ifndef CONNECTEDRESOURCES
#define CONNECTEDRESOURCES

#include "ui_ConnectedResourcres.h"
#include <QDockWidget>
#include "QPlainTextEdit"
#include "qssTextEdit/qssTextEdit.h"

class RConnectedResource : public QDockWidget, public Ui::connectedResourcresWindow
{
	Q_OBJECT
public:
    RConnectedResource(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~RConnectedResource();
private:
	void addNewResources();
	void deleteCurrentResource();
	void deleteAllResources();
private slots:
	void selectActionFromComboBox(int);
};
#endif
