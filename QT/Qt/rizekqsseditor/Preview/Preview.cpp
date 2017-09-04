#include "Preview/Preview.h"
#include "QString"
#include "QDirModel"
#include "QDebug"
#include "QResource"
#include "QProcess"
#include "QSettings"


#include "QPushButton"
#include "QCheckBox"
#include "QComboBox"
#include "QDateEdit"
#include "QDialogButtonBox"
#include "QDoubleSpinBox"
#include "QGroupBox"
#include "QLabel"
#include "QProgressBar"
#include "QPushButton"
#include "QRadioButton"
#include "QColumnView"
#include "QTableView"
#include "QTreeView"
#include "QListView"
#include "QTableWidget"
#include "QTreeWidget"
#include "QListWidget"
#include "QScrollBar"
#include "QSpinBox"
#include "QToolButton"
#include "QLineEdit"

#include "QDockWidget"
#include "QFrame"
#include "QToolBox"
#include "QTabBar"
#include "QTabWidget"
#include "QHeaderView"

#include "QDialog"

preview::preview(QWidget *parent, Qt::WindowFlags flags)
: QMainWindow(parent, flags)
{
	setupUi(this);
	QToolButton * tb = new QToolButton;
	tb->setText("Some QToolButton");
	toolBar->addWidget(tb);	

	QIcon ic;
	ic.addFile(":/ico/icos/document-preview.png");
	this->setWindowIcon(ic);

	connect(actionQDialog, SIGNAL(triggered()), this, SLOT(createQDialog()));
	connect(actionQMainWindow, SIGNAL(triggered()), this, SLOT(createQMainWindow()));
	connect(actionQDockWidget, SIGNAL(triggered()), this, SLOT(createQDockWidget()));
}

preview::~preview()
{

}
void preview::setTextOfEditor(QString txt)
{
	text = txt;
	this->setStyleSheet(text);
}
void preview::findClassesAndAddIt()
{
	ouptput->addItem("----Preview----");
	if (text.indexOf("QCheckBox")!=-1)
	{
		QCheckBox * a = new QCheckBox;
		a->setText("Test QCheckBox");
		gridLayout->addWidget(a);
		ouptput->addItem("Add QChekBox");
	}
	//----QComboBox
	if (text.indexOf("QComboBox")!=-1)
	{
		QComboBox * a = new QComboBox;
		a->addItem("First Item");
		a->addItem("Second Item");
		QComboBox * b = new QComboBox;
		b->setEditable(true);
		b->addItem("First Item");
		b->addItem("Second Item");
		gridLayout->addWidget(a);
		gridLayout->addWidget(b);
		ouptput->addItem("Add QComboBox");
	}
	//----QDateEdit
	if (text.indexOf("QDateEdit")!=-1)
	{
		QDateEdit * a = new QDateEdit;
		gridLayout->addWidget(a);
		ouptput->addItem("Add QDateEdit");
	}
	//----QDateTimeEdit
	if (text.indexOf("QDateTimeEdit")!=-1)
	{
		QDateTimeEdit * a = new QDateTimeEdit;
		gridLayout->addWidget(a);
		ouptput->addItem("Add QDateTimeEdit");
	}
	//----QDialogButtonBox
	if (text.indexOf("QDialogButtonBox")!=-1)
	{
		QDialogButtonBox * a = new QDialogButtonBox(QDialogButtonBox::Ignore| QDialogButtonBox::Apply | QDialogButtonBox::Abort);
		gridLayout->addWidget(a);
		ouptput->addItem("Add QDialogButtonBox");
	}
	//----QDoubleSpinBox
	if (text.indexOf("QDoubleSpinBox")!=-1)
	{
		QDoubleSpinBox * a = new QDoubleSpinBox;
		gridLayout->addWidget(a);
		ouptput->addItem("Add QDoubkeSpinBox");
	}
	//----QLabel
	if (text.indexOf("QLabel")!=-1)
	{
		QLabel * a = new QLabel;
		a->setText("Test QLabel");
		gridLayout->addWidget(a);
		ouptput->addItem("Add QLabel");
	}
	//----QProgressBar
	if (text.indexOf("QProgressBar")!=-1)
	{
		QProgressBar * a = new QProgressBar;
		a->setValue(5);
		gridLayout->addWidget(a);
		ouptput->addItem("Add QProgressBar");
	}
	//----QPushButton
	if (text.indexOf("QPushButton")!=-1)
	{
		QPushButton * a = new QPushButton;
		a->setText("Test QPushButton");
		gridLayout->addWidget(a);
		ouptput->addItem("Add QPushButton");
	}
	//----QRadioButton
	if (text.indexOf("QRadioButton")!=-1)
	{
		QRadioButton * a = new QRadioButton;
		a->setText("Test QRadioButton");
		gridLayout->addWidget(a);
		ouptput->addItem("Add QRadioButton");
	}
	//----QLineEdit
	if (text.indexOf("QLineEdit")!=-1)
	{
		QLineEdit * a = new QLineEdit;
		a->setText("Test QLineEdit");
		gridLayout->addWidget(a);
		ouptput->addItem("Add QLineEdit");
	}
	//----Line
	/*if (text.indexOf("Line")!=-1)
	{
		Line * a = new Line;
		gridLayout->addWidget(a);
		ouptput->addItem("Add Line");
	}*/
	//----QColumnView
	if (text.indexOf("QColumnView")!=-1)
	{
		QColumnView * a = new QColumnView;
		QDirModel * model = new QDirModel;
		a->setModel(model);
		gridLayout->addWidget(a);
		ouptput->addItem("Add QColumnView");
	}
	//----QTableView
	if (text.indexOf("QTableView")!=-1)
	{
		QTableView * a = new QTableView;
		QDirModel * model = new QDirModel;
		a->setModel(model);
		gridLayout->addWidget(a);
		ouptput->addItem("Add QTableView");
	}
	//----QListView
	if (text.indexOf("QListView")!=-1)
	{
		QListView * a = new QListView;
		QDirModel * model = new QDirModel;
		a->setModel(model);
		gridLayout->addWidget(a);
		ouptput->addItem("Add QListView");
	}
	//----QTreeView
	if (text.indexOf("QTreeView")!=-1)
	{
		QTreeView * a = new QTreeView;
		QDirModel * model = new QDirModel;
		a->setModel(model);
		gridLayout->addWidget(a);
		ouptput->addItem("Add QTreeView");
	}
	//----QTableWidget
	if (text.indexOf("QTableWidget")!=-1)
	{
		QTableWidget * a = new QTableWidget;
		a->setRowCount(3);
		a->setColumnCount(3);
		gridLayout->addWidget(a);
		ouptput->addItem("Add QTableWidget");
	}
	//----QTextEdit
	if (text.indexOf("QTextEdit")!=-1)
	{
		QTextEdit * a = new QTextEdit;
		a->setText("Test QTextEdit");
		gridLayout->addWidget(a);
		ouptput->addItem("Add QTextEdit");
	}
	//----QPlainTextEdit
	if (text.indexOf("QPlainTextEdit")!=-1)
	{
		QPlainTextEdit * a = new QPlainTextEdit;
		a->setPlainText("Test QPlainTextEdit");
		gridLayout->addWidget(a);
		ouptput->addItem("Add QPlainTextEdit");
	}
	//----QListWidget
	if (text.indexOf("QListWidget")!=-1)
	{
		QListWidget * a = new QListWidget;
		a->addItem("1 item");
		a->addItem("2 item");
		gridLayout->addWidget(a);
		ouptput->addItem("Add QListWidget");
	}
	//----QTreeWidget
	if (text.indexOf("QTreeWidget")!=-1)
	{
		QTreeWidget *treeWidget = new QTreeWidget();
		treeWidget->setColumnCount(1);
		QList<QTreeWidgetItem *> items;
		for (int i = 0; i < 10; ++i)
			items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("item: %1").arg(i))));
		QTreeWidgetItem * item1 = new QTreeWidgetItem;
		item1->setText(0,"item 1");
		QTreeWidgetItem * item2 = new QTreeWidgetItem;
		item2->setText(0,"item 2");
		QTreeWidgetItem * item3 = new QTreeWidgetItem;
		item3->setText(0,"item 3");
		items[1]->addChild(item1);
		items[1]->addChild(item2);
		items[1]->addChild(item3);
		treeWidget->insertTopLevelItems(0, items);
		gridLayout->addWidget(treeWidget);
		ouptput->addItem("Add QTreeWidget");
	}
	//----QScrollBar
	if (text.indexOf("QScrollBar")!=-1)
	{
		QScrollBar * a = new QScrollBar;
		a->setOrientation(Qt::Horizontal);
		gridLayout->addWidget(a);
		QScrollBar * b = new QScrollBar;
		b->setOrientation(Qt::Vertical);
		gridLayout->addWidget(b);
		ouptput->addItem("Add QScrollBar");
	}
	//----QSpinBox
	if (text.indexOf("QSpinBox")!=-1)
	{
		QSpinBox * a = new QSpinBox;
		gridLayout->addWidget(a);
		ouptput->addItem("Add QSpinBox");
	}
	//----QDockWidget
	if (text.indexOf("QDockWidget")!=-1)
	{
		QDockWidget * a = new QDockWidget;
		a->setWindowTitle("QDockWidget");
		QLabel * l1 = new QLabel;
		l1->setText("QDockWidget example label\n Some string 1\nSome string 2");
		a->setWidget(l1);
		gridLayout->addWidget(a);
		ouptput->addItem("Add QDockWidget");
	}
	//----QFrame
	if (text.indexOf("QFrame")!=-1)
	{
		QFrame * a = new QFrame;
		a->setWindowTitle("QFrame");
		a->setFixedHeight(50);
		a->setFixedWidth(100);
		gridLayout->addWidget(a);
		ouptput->addItem("Add QFrame");
	}
	//----QToolBox
	if (text.indexOf("QToolBox")!=-1)
	{
		QToolBox * a = new QToolBox;
		a->setWindowTitle("QToolBox");
		QLabel * l1 = new QLabel;
		a->addItem(l1,"some item 1");
		l1->setFixedHeight(50);
		QLabel * l2 = new QLabel;
		a->addItem(l2,"some item 2");
		l2->setFixedHeight(50);
		a->setFixedHeight(80);
		a->setFixedWidth(180);
		gridLayout->addWidget(a);
		ouptput->addItem("Add QToolBox");
	}
	//----QTabWidget
	if (text.indexOf("QTabWidget")!=-1)
	{
		QTabWidget * a = new QTabWidget;
		a->setWindowTitle("QTabWidget");
		QLabel * l1 = new QLabel;
		l1->setText("some label 1");
		QLabel * l2 = new QLabel;
		l2->setText("some label 2");
		a->addTab(l1,"1 tab");
		a->addTab(l2,"2 tab");
		gridLayout->addWidget(a);
		ouptput->addItem("Add QTabWidget");
	}
	//----QGroupBox
	if (text.indexOf("QGroupBox")!=-1)
	{
		QGroupBox * a = new QGroupBox;
		a->setTitle("some QGroupBox");
		gridLayout->addWidget(a);
		ouptput->addItem("Add QGroupBox");
	}
	//----QTabBar
	if (text.indexOf("QTabBar")!=-1)
	{
		QTabBar * a = new QTabBar;
		a->setWindowTitle("QTabBar");
		a->addTab("1 tab");
		a->addTab("2 tab");
		gridLayout->addWidget(a);
		ouptput->addItem("Add QTabBar");
	}
	//----QWidget
	if (text.indexOf("QWidget")!=-1)
	{
		QWidget * a = new QWidget;
		a->setWindowTitle("QWidget");
		a->setFixedHeight(50);
		a->setFixedWidth(150);
		gridLayout->addWidget(a);
		ouptput->addItem("Add QWidget");
	}
	ouptput->addItem("----End of Preview----");
}

void preview::createQDialog()
{
	QDialog * dialog = new QDialog;
	dialog->setGeometry(30,60,300,120);
	dialog->setWindowTitle("QDialog example");
	dialog->setStyleSheet(text);
	dialog->show();
}
void preview::createQDockWidget()
{
	QDockWidget * dockWidget = new QDockWidget;
	dockWidget->setGeometry(30,60,300,120);
	dockWidget->setWindowTitle("QDockWidget example");
	dockWidget->setStyleSheet(text);
	dockWidget->show();
}
void preview::createQMainWindow()
{
	QMainWindow * mainWindow = new QMainWindow;
	mainWindow->setGeometry(30,60,300,120);
	mainWindow->setWindowTitle("QMainWindow example");
	mainWindow->setStyleSheet(text);
	mainWindow->show();
}
void preview::setOutputWindow(outputWindow * o)
{
	ouptput = o;
}
void preview::setResorces(QStringList r)
{
	resorces = r;
	for(int a=0;a < resorces.count();a++)
	{
		QString qrcFile = resorces[a];
		QFileInfo qrcFileInfo(qrcFile);
		QString rccFileName = qrcFileInfo.baseName() + ".rcc";
		QString rccPath = qrcFileInfo.absolutePath() + "/" + rccFileName;
		QStringList args;
		args << qrcFile;
		args << "-binary";
		args << "-o" << rccPath;
		QProcess rccProc;
		QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
		QString qtDirPath = env.value("QTDIR");
		bool isLoaded = false;
		int ret = -1;
		rccProc.setWorkingDirectory(qtDirPath + "bin/");
		ret = rccProc.execute("rcc", args);
		if (ret == 0) // rcc executed successfully
		{
			isLoaded = QResource::registerResource(rccPath);
			QIcon ic;
			ic.addFile(":/ico/icos/Info.png");
			this->setWindowIcon(ic);
			if (!isLoaded)
				ouptput->addItem("Resource " + rccPath + " can not be loaded!");
		}
	}
}
