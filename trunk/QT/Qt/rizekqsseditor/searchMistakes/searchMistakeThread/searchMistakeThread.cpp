#include "searchMistakeThread.h"
#include "QString"
#include "time.h"
#include "QDebug"

searchMistakesThread::searchMistakesThread()
{
}

searchMistakesThread::~searchMistakesThread()
{

}
void searchMistakesThread::run()
{
		emit toOutput("Start of search mistakes");
		time_t start,end;
		start = clock();
		findLightMistakes();
		findMidleMistakes();
		findStrongMistakes();
		end = clock();
		emit toOutput("Time of search mistake: " + QString::number(end-start) + "ms");
		emit toOutput("End of search mistake");
		emit finished();
}
// void searchMistakesThread::findMistakes()
// {
// 	run();
// }
void searchMistakesThread::findLightMistakes()
{
	int c1 = 0;
	while(c1!=-1)
	{
		c1 = text.indexOf(QRegExp("\\{ *\\}"),c1+1);
		if (c1 != -1)
			emit toMistakesList("empty stylesheet {it's empty!} in " + QString::number(c1) + " sign");
	}
	int c2 = 0;
	while(c2!=-1)
	{
		c2 = text.indexOf(QRegExp("\\( *\\)"),c2+1);
		if (c2 != -1)
			emit toMistakesList("empty between the parentheses in " + QString::number(c2) + " sign");
	}
	int c3 = 0;
	while(c3!=-1)
	{
		c3 = text.indexOf(QRegExp("\\[ *\\]"),c3+1);
		if (c3 != -1)
			emit toMistakesList("empty between the square brackets in " + QString::number(c3) + " sign");
	}
	int c4 = 0;
	while(c4!=-1)
	{
		c4 = text.indexOf(QRegExp("\" *\""),c4+1); 
		if (c4 != -1)
			emit toMistakesList("empty between the quotes in " + QString::number(c4) + " sign");
	}
}
void searchMistakesThread::findMidleMistakes()
{
	if (text.count()!=0)
 {
	int HowMany1 = 0;//{
	int HowMany11 = 0;//}
	int HowMany2 = 0;//(
	int HowMany21 = 0;//)
	int HowMany3 = 0;//[
	int HowMany31 = 0;//]
	int HowMany4 = 0;//"
	int HowMany5 = 0;// /*
	int HowMany51 = 0;// */
	bool isComment;
	isComment = false;
	for (int a=0;a<text.count();a++)
	{
			if (text[a]=='/' && text[a+1]=='*')
			{
				HowMany5++;
				isComment=true;
			}
			if (text[a]=='*' && text[a+1]=='/')
			{
				HowMany51++;
				isComment=false;
			}
		if (!isComment)
		{
			if (text[a]=='{')
				HowMany1++;
			if (text[a]=='}')
				HowMany11++;
			if (text[a]=='(')
				HowMany2++;
			if (text[a]==')')
				HowMany21++;
			if (text[a]=='[')
				HowMany3++;
			if (text[a]==']')
				HowMany31++;
			if (text[a]=='\"')
				HowMany4++;
		}
	}
	if (HowMany1>HowMany11)
		emit toMistakesList("Your write " + QString::number(HowMany1-HowMany11) + "extra { ");
	if (HowMany1<HowMany11)
		emit toMistakesList("Your write " + QString::number(HowMany11-HowMany1) + "extra } ");
	if (HowMany2>HowMany21)
		emit toMistakesList("Your write " + QString::number(HowMany2-HowMany21) + "extra ( ");
	if (HowMany2<HowMany21)
		emit toMistakesList("Your write " + QString::number(HowMany21-HowMany2) + "extra ) ");
	if (HowMany3>HowMany31)
		emit toMistakesList("Your write " + QString::number(HowMany3-HowMany31) + "extra [ ");
	if (HowMany3<HowMany31)
		emit toMistakesList("Your write " + QString::number(HowMany31-HowMany3) + "extra ] ");
	if (HowMany4%2!=0)
		emit toMistakesList("Your write 1 extra \" ");
	if (HowMany5>HowMany51)
		emit toMistakesList("Your write " + QString::number(HowMany5-HowMany51) + "extra /* ");
	if (HowMany5<HowMany51)
		emit toMistakesList("Your write " + QString::number(HowMany51-HowMany5) + "extra */ ");
 }
}
void searchMistakesThread::findStrongMistakes()
{
	/*QStringList classes;
	bool isInsideOfClass = false;
	QString currentClass;
	bool isComment = false;
	for(int a=0;a<=text.count();++a)
	{
		if (text[a]=='{')
		{
			isInsideOfClass = true;
			currentClass.remove("\n");
			currentClass.remove("//");
			currentClass.remove(" ");
			currentClass.remove("}");
			currentClass.remove(",");
			currentClass.remove("*");
			if (currentClass!="")
				classes << currentClass;
			currentClass = "";
		}
		if (!isComment && text[a]==',' && !isInsideOfClass)
		{
			currentClass.remove("\n");
			currentClass.remove("//");
			currentClass.remove(" ");
			currentClass.remove(",");
			currentClass.remove("*");
			if (currentClass!="")
				classes << currentClass;
			currentClass = "";
		}
		if (!isInsideOfClass && !isComment)
			currentClass += text[a];
		if (a!=text.count() && !isInsideOfClass)
		{
			if (text[a]=='/' && text[a+1]=='*')
				isComment=true;
			if (text[a]=='*' && text[a+1]=='/')
				isComment=false;
			if (text[a+1]=='[')
				isComment=true;
			if (text[a]==']')
				isComment=false;
			if (text[a+1]==':')
				isComment=true;
			if (text[a]==',')
				isComment=false;
		}
		if (!isComment && text[a]=='}')
		{
			isInsideOfClass = false;
		}
	}
	QStringList QSSclasses;
	QSSclasses << "QAbstractScrollArea" << "QCheckBox" << "QColumnView" << "QComboBox" << "QDateEdit" << "QTextBrowser"
		<< "QDateTimeEdit" << "QDialog" << "QDialogButtonBox" << "QDockWidget"  <<"QDoubleSpinBox" << "QFrame" << "QGraphicsView"
		<< "QGroupBox" << "QHeaderView" << "QLabel" << "QLineEdit"  <<"QListView" << "QListWidget" << "QMainWindow"
		<< "QMenu" << "QMenuBar" << "QMessageBox" << "QProgressBar" << "QPushButton" << "QRadioButton" << "QScrollBar" << "QSizeGrip"
		<< "QSlider" << "QSpinBox" << "QSplitter" << "QStatusBar" << "QTabBar" << "QTabWidget" << "QTableView" << "QTableWidget"
		<< "QTextEdit" << "QTimeEdit" << "QToolBar" << "QToolButton" << "QToolBox" << "QToolTip" << "QTreeView" << "QTreeWidget"
		<< "QWidget" << "QPlainTextEdit" << "Spacer" << "QCommandLinkButton" << "QStackedWidget" << "QMdiArea" << "QDial" << "QCalendarWidget"
		<< "QLCDNumber" << "Line" << "QDeclarativeView" << "QWebView" << "QAbstractItemView" << "*";
	foreach (const QString &cl, classes)
	{
		bool isFind;
		foreach (const QString &QSSclass, QSSclasses)
	{
		if (cl==QSSclass)
		{
			isFind = true;
			break;
		}
		else 
			isFind = false;
	}
		if(!isFind)
			emit toMistakesList("Found not a standard class: " + cl);
	}*/
	QStringList QSSclasses;
	QSSclasses << "QAbstractScrollArea" << "QCheckBox" << "QColumnView" << "QComboBox" << "QDateEdit" << "QTextBrowser"
		<< "QDateTimeEdit" << "QDialog" << "QDialogButtonBox" << "QDockWidget"  <<"QDoubleSpinBox" << "QFrame" << "QGraphicsView"
		<< "QGroupBox" << "QHeaderView" << "QLabel" << "QLineEdit"  <<"QListView" << "QListWidget" << "QMainWindow"
		<< "QMenu" << "QMenuBar" << "QMessageBox" << "QProgressBar" << "QPushButton" << "QRadioButton" << "QScrollBar" << "QSizeGrip"
		<< "QSlider" << "QSpinBox" << "QSplitter" << "QStatusBar" << "QTabBar" << "QTabWidget" << "QTableView" << "QTableWidget"
		<< "QTextEdit" << "QTimeEdit" << "QToolBar" << "QToolButton" << "QToolBox" << "QToolTip" << "QTreeView" << "QTreeWidget"
		<< "QWidget" << "QPlainTextEdit" << "Spacer" << "QCommandLinkButton" << "QStackedWidget" << "QMdiArea" << "QDial" << "QCalendarWidget"
		<< "QLCDNumber" << "Line" << "QDeclarativeView" << "QWebView" << "QAbstractItemView" << "*";
	QStringList encounteredClasses;

	bool isComment = false;
	bool isOutsideTheClass = true;
	bool isOutExcess = true;
	
	QString currentClass;
	register int a;
	for(a=0;a < text.size(); a++)
	{
		if(text[a]=='\\' && text[a+1]=='\*')
			isComment = true;
		if(text[a]=='\*' && text[a+1]=='\\')
			isComment = false;
		if (!isComment)
		{
			if (isOutsideTheClass && isOutExcess)
				currentClass += text[a];
			if (!isOutsideTheClass || !isOutExcess)
			{
				currentClass = currentClass.replace(" ", "");
				currentClass = currentClass.replace("\n", "");
				currentClass = currentClass.replace("{", "");
				currentClass = currentClass.replace(":", "");
				if (currentClass!="")
					encounteredClasses << currentClass.split("\,");
				currentClass.clear();
			}
			if (text[a]=='}')
				isOutsideTheClass = true;
			if (text[a]=='{')
				isOutsideTheClass = false;
			if (text[a]==':' || text[a]==' ' || text[a]=='['
				|| text[a]=='*')
				isOutExcess = false;
			if (isOutsideTheClass && (text[a]==','))
				isOutExcess = true;
		}
	}
	foreach (const QString &cl, encounteredClasses)
	{
		bool isFind;
		foreach (const QString &QSSclass, QSSclasses)
		{
			if (cl==QSSclass)
			{
				isFind = true;
				break;
			}
			else 
				isFind = false;
		}
		if(!isFind)
			emit toMistakesList("Found not a standard class: " + cl);
	}
}
void searchMistakesThread::setObjects(QPlainTextEdit* pte,QListWidget* lw, outputWindow * ow)
{
	plainTextEdit = pte;
	listWidget = lw;
	output = ow;
}
void searchMistakesThread::setKindOfSearch(int kos)
{
	kindOfSearch = kos;
}
void searchMistakesThread::getText(QString txt)
{
	text = txt;
}