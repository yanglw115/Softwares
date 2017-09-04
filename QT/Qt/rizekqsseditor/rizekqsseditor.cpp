// Modules
#include "QtScript"
// Qt headers
#include "QFile"
#include "QSyntaxHighlighter"
#include "QPushButton"
#include "QLineEdit"
#include "QLabel"
#include "QVariant"
#include "QPalette"
#include "QUrl"
#include "QSystemTrayIcon"
#include "QAbstractItemModel"
#include "QLayout"
#include "QFontDialog"
#include "QPrintDialog"
#include "QStringListModel"
#include "QMessageBox"
#include "QColorDialog"
#include "QPrinter"
#include "QDesktopServices"
#include "QFileDialog"
#include "QStyleFactory"
//My headers
#include "rizekqsseditor.h"
#include "searchMistakes/searchMistakeThread/searchMistakeThread.h"
#include "replace/replace.h"
#include "search/search.h"
#include "RScript/RScript.h"
#include "prevnextWidget/prevnextWidget.h"
#include "About_program/aboutProgram.h"
#include "Help/Help.h"
#include "Settings/Settings.h"
#include "qssTextEdit/qssTextEdit.h"
#include "Preview/Preview.h"
#include "programCompleter/programCompleter.h"
#include "searchMistakes/searchMistakes.h"
#include "Output/Output.h"
#include "ChekForUpdate/updater.h"

RizekQssEditor::RizekQssEditor(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{
	//QApplication::setStyle(new QCleanlooksStyle);
	setupUi(this);
	//assotiate();

	currentVersion = "1.13";
	isIconInTray = false;
	//
	syntax = new Highlighter(plainTextEdit->document());	
	//Style
	QSettings settings("Rizek", "Rizek Qss Editor");
	if (settings.value("style").toString()!="")
	{
		QApplication::setStyle(QStyleFactory::create(settings.value("style").toString()));
	}
	//-QSS
	if (settings.value("this\UseStyleSheet",true).toBool())
	{
		QFile file(":/Other/qss/default.qss");
		file.open(QFile::ReadOnly);
		QString styleSheet = QLatin1String(file.readAll());
		this->setStyleSheet(styleSheet);
	}

	programCompleter * PC = new programCompleter;
	PC->setTextEdit(plainTextEdit);

	pSearchMistakes = new searchMistakes;
	pSearchMistakes->setVisible(false);
	this->addDockWidget(Qt::LeftDockWidgetArea ,pSearchMistakes ,Qt::Horizontal);

	pOutput = new outputWindow;
	this->addDockWidget(Qt::BottomDockWidgetArea ,pOutput ,Qt::Horizontal);

	pConnectedResource = new RConnectedResource;
	this->addDockWidget(Qt::LeftDockWidgetArea ,pConnectedResource ,Qt::Horizontal);
	this->tabifyDockWidget(pSearchMistakes,pConnectedResource);
	//Scripts
	pScript = new scriptWindow;
	this->addDockWidget(Qt::BottomDockWidgetArea ,pScript ,Qt::Horizontal);
	this->tabifyDockWidget(pOutput,pScript);
	pScript->configure();
	//StatusBarLabel
	statusBarLabel = new QLabel;
	QMainWindow::statusBar()->addPermanentWidget(statusBarLabel);

	buildToolBar();
	setSettings();
	loadRecentProjects();

	QTextCursor c = plainTextEdit->textCursor();
	c.setVisualNavigation(true);
	plainTextEdit->setTextCursor(c);
	pSearchMistakes->setTextEdit(plainTextEdit);

	//This
	this->restoreGeometry(settings.value("this\geometry", this->geometry()).toByteArray());
	if (!settings.value("this\windowState").isNull())
		this->restoreState(settings.value("this\windowState").toByteArray());

	//Connects
	connect(actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(actionSave_as, SIGNAL(triggered()), this, SLOT(saveAsInFile()));
	connect(actionSave, SIGNAL(triggered()), this, SLOT(saveInFile()));
	connect(actionReplace, SIGNAL(triggered()), this, SLOT(openReplaceWindow()));
	connect(actionDocumentation, SIGNAL(triggered()), this, SLOT(showOnlineDocumentation()));
	connect(actionExamples, SIGNAL(triggered()), this, SLOT(showOnlineExamples()));
	connect(actionSearch, SIGNAL(triggered()), this, SLOT(openSearchWindow()));
	connect(actionAbout_program, SIGNAL(triggered()), this, SLOT(openAboutProgramWindow()));
	connect(actionHelp, SIGNAL(triggered()), this, SLOT(openHelpWindow()));
	connect(actionNew, SIGNAL(triggered()), this, SLOT(createNewDocument()));
	connect(actionPreview, SIGNAL(triggered()), this, SLOT(openPreviewWindow()));
	connect(actionSettings, SIGNAL(triggered()), this, SLOT(openSettingsWindow()));
	connect(actionPrint, SIGNAL(triggered()), this, SLOT(printFile()));
	connect(actionColor, SIGNAL(triggered()), this, SLOT(insertColor16bit()));
	connect(actionFont_name, SIGNAL(triggered()), this, SLOT(insertFontName()));
	connect(actionReopen, SIGNAL(triggered()), this, SLOT(reopenCurrentFile()));
	connect(actionOpen_dir_of_current_file, SIGNAL(triggered()), this, SLOT(openDirOfCurrentFile()));
	connect(actionOpen_file_in_standart_note, SIGNAL(triggered()), this, SLOT(openFileThroughSystem()));
	connect(actionHide_to_tray, SIGNAL(triggered()), this, SLOT(hideToTray()));
	connect(actionSearch_mistakes, SIGNAL(triggered()), this, SLOT(showSearchMistakesWindow()));
	connect(actionResources, SIGNAL(triggered()), this, SLOT(showResourcesWindow()));
	connect(actionScript, SIGNAL(triggered()), this, SLOT(showScriptWindow()));
	connect(actionOutput, SIGNAL(triggered()), this, SLOT(showOutput()));
	connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(actionDonate, SIGNAL(triggered()), this, SLOT(donate()));
    connect(actionCheck_for_updates, SIGNAL(triggered()), this, SLOT(chekForUpdates()));

	connect(plainTextEdit, SIGNAL(textChanged()), this, SLOT(setPlainTextEditModified()));
	
	connect(plainTextEdit, SIGNAL(cursorPositionChanged()), this, SLOT(showCursorXY()));
	connect(prevNextButtons->prev, SIGNAL(clicked()), plainTextEdit, SLOT(undo())); //previous
	connect(prevNextButtons->next, SIGNAL(clicked()), plainTextEdit, SLOT(redo())); //next
	connect(searchLineEdit, SIGNAL(textChanged(QString)), this, SLOT(findInTheText(QString)));
	connect(searchButton, SIGNAL(clicked()), this, SLOT(findNext()));
	connect(searchLineEdit, SIGNAL(returnPressed()), this, SLOT(findNext()));

	connect(actionStandart, SIGNAL(triggered()), this, SLOT(setViewStandart()));
	connect(actionLeft_bottom, SIGNAL(triggered()), this, SLOT(setViewLeftBottom()));
	connect(actionRight_bottom, SIGNAL(triggered()), this, SLOT(setViewRightBottom()));
	connect(actionClear, SIGNAL(triggered()), this, SLOT(setViewClear()));
}

RizekQssEditor::~RizekQssEditor()
{
	delete pSearchMistakes;
}
void RizekQssEditor::resizeEvent( QResizeEvent* )
{
	//plainTextEdit->setFixedHeight(this->height()-80);
	//plainTextEdit->setFixedWidth(this->width()-6);
}
void RizekQssEditor::findInTheText(const QString & text)
{
	plainTextEdit->moveCursor(QTextCursor::End);
	plainTextEdit->find(searchLineEdit->text(), QTextDocument::FindBackward);
}
void RizekQssEditor::findNext()
{
	bool is = true;
	is = plainTextEdit->find(searchLineEdit->text(), QTextDocument::FindBackward);
	if (!is)
	{
		plainTextEdit->moveCursor(QTextCursor::End);
		plainTextEdit->find(searchLineEdit->text(), QTextDocument::FindBackward);
	}
}
void RizekQssEditor::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, QObject::trUtf8("QSS File"), "/home/" , "QSS File (*.qss)");
	openFileFromString(fileName);
}
void RizekQssEditor::saveAsInFile()
{
	QString fileName = QFileDialog::getSaveFileName(this, QObject::trUtf8("QSS File"), "/home/" , "QSS File (*.qss)");
	if (!fileName.isEmpty()) {
		QFile file(fileName);
		if (!file.open(QFile::WriteOnly | QFile::Text)) {
			QMessageBox::warning(this, tr("Warning!"),
				tr("Cannot write file %1:\n%2")
				.arg(fileName)
				.arg(file.errorString()));
			return;
		}
		changeAddressOfCurrentFile(fileName);
		rewriteRecentProjects();
		pOutput->addItem("File save as " + fileName);
		saveResources();
		QTextStream out(&file);
		out << plainTextEdit->toPlainText();
		isModified = false;
		removeStarsFromTitle();
	}
}
void RizekQssEditor::changeAddressOfCurrentFile(QString file)
{
	addressOfCurrenFile = file;
	QFile f(addressOfCurrenFile);
	const QString name = QFileInfo(f).fileName ();
	this->setWindowTitle(tr("%1 - Rizek Qss Editor").arg(name));
	QSettings settings("Rizek", "Rizek Qss Editor");
	settings.setValue("lastProject", addressOfCurrenFile);
}
void RizekQssEditor::saveInFile()
{
	QString fileName = addressOfCurrenFile;
	if (fileName == "New style sheet")
		saveAsInFile();
	else
	{
		if (!fileName.isEmpty()) {
		QFile file(fileName);
		if (!file.open(QFile::WriteOnly | QFile::Text)) {
			QMessageBox::warning(this, tr("Warning!"),
				tr("Cannot write file %1:\n%2")
				.arg(fileName)
				.arg(file.errorString()));
			return;
		}
		changeAddressOfCurrentFile(fileName);
		rewriteRecentProjects();
		pOutput->addItem("File save");
		saveResources();
		QTextStream out(&file);
		out << plainTextEdit->toPlainText();
	}
	}
	isModified = false;
	removeStarsFromTitle();
}
void RizekQssEditor::openReplaceWindow()
{
	replace * rep = new replace;
	rep->setPlaintTextEdit(plainTextEdit);
	rep->show();
}
void RizekQssEditor::openSearchWindow()
{
	search * s = new search;
	s->setPlaintTextEdit(plainTextEdit);

	s->show();
}
void RizekQssEditor::openAboutProgramWindow()
{
	aboutProgram * about = new aboutProgram;
	about->show();
}
void RizekQssEditor::openHelpWindow()
{
	RHelp * help = new RHelp;
	help->show();
}
void RizekQssEditor::openPreviewWindow()
{
	preview * previewWindow = new preview;
	previewWindow->setTextOfEditor(plainTextEdit->toPlainText());
	previewWindow->setOutputWindow(pOutput);
	previewWindow->findClassesAndAddIt();

	QStringList resources;
	for(int a=0;a<pConnectedResource->listWidget->count();a++)
		resources << pConnectedResource->listWidget->item(a)->text();
	previewWindow->setResorces(resources);
	previewWindow->show();
}
void RizekQssEditor::openSettingsWindow()
{
	settings * s = new settings;
	connect(s, SIGNAL(rejected()), this, SLOT(setSettings()));
	s->show();
}
void RizekQssEditor::buildToolBar()
{
	mainToolBar->layout()->setSpacing(5);
	//PrevNextButtons
	prevNextButtons = new prevnextWidget;
	mainToolBar->addWidget(prevNextButtons);
	//--SearchLabel
	searchLabel = new QLabel;
	searchLabel->setText(tr("Search:"));
	mainToolBar->addWidget(searchLabel);
	//--SerchLineEdit
	searchLineEdit = new QLineEdit;
	searchLineEdit->setFixedHeight(20);
	searchLineEdit->setPlaceholderText(tr("Enter search text(pressed \"Enter\" to find next)"));
	mainToolBar->addWidget(searchLineEdit);
	//Search Button
	searchButton = new QPushButton;
	searchButton->setFixedHeight(20);
	searchButton->setFixedWidth(20);
	searchButton->setToolTip(tr("Next"));
	searchButton->setProperty("isSearchButton", true);
	  //Style sheet for search button
	//QSS
	QFile file(":/Other/qss/searchButtonStyleSheet.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	searchButton->setStyleSheet(styleSheet);

	mainToolBar->addWidget(searchButton);
	//--MainToolBar
	this->addToolBar(mainToolBar);
	mainToolBar->setToolTip(tr("Main tool bar"));
}
void RizekQssEditor::showOnlineDocumentation()
{
	QUrl url;
	url.setUrl("http://doc.qt.nokia.com/4.7/stylesheet-reference.html");
	QDesktopServices::openUrl(url);
}
void RizekQssEditor::showOnlineExamples()
{
	QUrl url;
	url.setUrl("http://doc.qt.nokia.com/4.7/stylesheet-examples.html");
	QDesktopServices::openUrl(url);
}
QAbstractItemModel *RizekQssEditor::modelFromFile(const QString& fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly))
		return new QStringListModel(completer);

#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
	QStringList words;

	while (!file.atEnd()) {
		QByteArray line = file.readLine();
		if (!line.isEmpty())
			words << line.trimmed();
	}

#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif
	return new QStringListModel(words, completer);
}
void RizekQssEditor::setSettings()
{
	QSettings settings("Rizek", "Rizek Qss Editor");
	//Open last project
	if (addressOfCurrenFile!=settings.value("lastProject").toString())
		openFileFromString(settings.value("lastProject").toString());
	//Completer of QPlainTextEdit and lineEdit
	completer = new QCompleter(this);
	completer->setModel(modelFromFile(":/Other/wordlist.txt"));
	completer->setModelSorting(QCompleter::UnsortedModel);
	completer->setCaseSensitivity(Qt::CaseSensitivity(settings.value("Completer\CaseSensitivity",Qt::CaseSensitive).toInt()));
	completer->setWrapAround(settings.value("Completer\WrapAround",false).toBool());
	completer->setCompletionMode(QCompleter::CompletionMode(settings.value("Completer\CompletionMode",QCompleter::PopupCompletion).toInt()));
	plainTextEdit->setCompleter(completer);
	searchLineEdit->setCompleter(completer);
	//Font
	QFont f = this->font();
	if (f.pointSize()<10)
		f.setPointSize(10);
	QFont font = settings.value("font", f).value<QFont>();
	plainTextEdit->setFont(font);

	QPalette newPal(plainTextEdit->palette());
	newPal.setColor(QPalette::Highlight, QColor(settings.value("TextEdit\HC",QColor("#468dd4")).toString()));
	newPal.setColor(QPalette::HighlightedText,QColor(settings.value("TextEdit\HTC",QColor("#ffffff")).toString()));
	plainTextEdit->setPalette(newPal);

	if(settings.value("Autosave", true).toBool())
		this->startTimer(settings.value("Autosave\Time", 180).toInt()*1000);
}
void RizekQssEditor::closeEvent( QCloseEvent * event )
{ 
	QSettings set("Rizek","Rizek Qss Editor");
	if (set.value("this\WhenClosing", 0).toInt()==0)
	{
		set.setValue("this\geometry", saveGeometry());
		set.setValue("this\windowState", saveState());
		if (set.value("SaveWhenUserExit", true).toBool())
			saveInFile();
		if (isModified==true)
		{
			int ret = QMessageBox::warning(this, tr("Save your project"),
				tr("The document has been modified.\n"
				"Do you want to save your changes?"),
				QMessageBox::Save | QMessageBox::Discard
				| QMessageBox::Cancel,
				QMessageBox::Save);
			if (ret==QMessageBox::Save)
				saveInFile();
			if (ret==QMessageBox::Discard)
				event->accept();
			if (ret==QMessageBox::Cancel)
				event->ignore();
		}
	}
	else 
	{
		event->ignore();
		hideToTray();
	}
}
void RizekQssEditor::setPlainTextEditModified()
{
	isModified = true;
	this->setWindowTitle("*" + this->windowTitle().replace("*",""));
}
void RizekQssEditor::createNewDocument()
{
	bool isContinue = true;
	if (isModified==true)
	{
		int ret = QMessageBox::warning(this, tr("Save your project"),
			tr("The document has been modified.\n"
			"Do you want to save your changes?"),
			QMessageBox::Save | QMessageBox::Discard
			| QMessageBox::Cancel,
			QMessageBox::Save);
		if (ret==QMessageBox::Save)
			saveInFile();
		if (ret==QMessageBox::Cancel)
			isContinue = false;
	}
	pOutput->addItem("New document was create");
	changeAddressOfCurrentFile("New style sheet");
	plainTextEdit->setPlainText("");
	pConnectedResource->listWidget->clear();
	isModified = false;
	removeStarsFromTitle();
}

void RizekQssEditor::loadRecentProjects()
{
	menuRecent_projects->clear();
	QSettings settings("Rizek", "Rizek Qss Editor");
	for (int a=0;a<=5;a++)
	{
		if (!settings.value("lastProjects/" + QString::number(a)).toString().isEmpty())
		{
			menuRecent_projects->addAction(settings.value("lastProjects/" + QString::number(a)).toString());
			connect(menuRecent_projects->actions()[a], SIGNAL(triggered()), this, SLOT(openFileByActionClicked()));
		}
	}
}
void RizekQssEditor::rewriteRecentProjects()
{
	QSettings settings("Rizek", "Rizek Qss Editor");
	bool isListNotComplete = false;
	for (int a=0;a<6;++a)
	{
		if (settings.value("lastProjects/" + QString::number(a)).toString().isEmpty()) 
		{
			bool isNoCopy = true;
			for (int b=0;b<a;++b)
			{
					if (addressOfCurrenFile==settings.value("lastProjects/" + QString::number(b)).toString())
					{
						isNoCopy = false;
					}
			}
			if(isNoCopy && isListNotComplete)
				{
					settings.setValue("lastProjects/" + QString::number(a), addressOfCurrenFile);
					isListNotComplete = true;
				}
		}
		else
			isListNotComplete = false;
	}
	
	if (isListNotComplete==false)
	{
		bool isNoCopy = true;
		for (int b=0;b<5;++b)
		{
			if (addressOfCurrenFile==settings.value("lastProjects/" + QString::number(b)).toString())
			{
				isNoCopy = false;
			}
		}
		if (isNoCopy==true)
		{
			for(int a=5;a>0;--a)
			{
				settings.setValue("lastProjects/" + QString::number(a), settings.value("lastProjects/" + QString::number(a-1)).toString());
			}
			settings.setValue("lastProjects/" + QString::number(0), addressOfCurrenFile);
		}
	}
	loadRecentProjects();
}
void RizekQssEditor::openFileFromString(QString file)
{
	QSettings settings("Rizek", "Rizek Qss Editor");
	addressOfCurrenFile = file;
	QString fileName = addressOfCurrenFile;
	if (!fileName.isEmpty()) {
		QFile file;
		file.setFileName(fileName);
		file.open(QFile::ReadOnly);
		QString qssFile = QLatin1String(file.readAll());
		changeAddressOfCurrentFile(fileName);
		plainTextEdit->setPlainText(qssFile);
		pOutput->addItem("File " + fileName + " was opened");
		rewriteRecentProjects();
		isModified = false;
		removeStarsFromTitle();
		pConnectedResource->listWidget->clear();
		loadResources();
	}
}
void RizekQssEditor::openFileByActionClicked()
{
	QObject *o = sender();
	QAction * act = qobject_cast<QAction *>(o);
	if (act)
		if(act->text()!=addressOfCurrenFile)
		{
			QSettings settings("Rizek", "Rizek Qss Editor");
			for(int a=1;a<6;++a)
				if (settings.value("lastProjects/" + QString::number(a)).toString()==act->text()) 
					for(int b=a;b>0;--b)
						settings.setValue("lastProjects/" + QString::number(b),settings.value("lastProjects/" + QString::number(b-1)));
			settings.setValue("lastProjects/" + QString::number(0), act->text());
			openFileFromString(act->text());
		}
}
void RizekQssEditor::printFile()
{
#ifndef QT_NO_PRINTER
	QPrinter printer;
	QPrintDialog *dialog = new QPrintDialog(&printer, this);
	dialog->setWindowTitle(tr("Print Document"));
	if (plainTextEdit->textCursor().hasSelection())
		dialog->addEnabledOption(QAbstractPrintDialog::PrintSelection);
	if (dialog->exec() != QDialog::Accepted)
		return;
	plainTextEdit->print(&printer);
	pOutput->addItem("Qss code was print");
#endif
}
void RizekQssEditor::timerEvent(QTimerEvent* e)
{
	if (addressOfCurrenFile!="New style sheet")
		saveInFile();
}
void RizekQssEditor::insertColor16bit()
{
	QColorDialog cd;
	cd.setWindowTitle(tr("Selected color to insert"));
	QIcon ic;
	ic.addFile(":/ico/icos/colors.png");
	cd.setWindowIcon(ic);
	if(cd.exec() == cd.Accepted){
		QString color = cd.currentColor ().name();
		pOutput->addItem("Color " + color + " was insert");
		plainTextEdit->textCursor().insertText(color);
	}
}
void RizekQssEditor::insertFontName()
{
	QFontDialog fd;
	fd.setWindowTitle(tr("Selected font to insert"));
	QIcon ic;
	ic.addFile(":/ico/icos/font.png");
	fd.setWindowIcon(ic);
	if(fd.exec() == fd.Accepted){
		QString font = fd.currentFont().rawName();
		pOutput->addItem("Font " + font + " was insert");
		plainTextEdit->textCursor().insertText(font);
	}
}
void RizekQssEditor::reopenCurrentFile()
{
	openFileFromString(addressOfCurrenFile);
	pOutput->addItem("Current file was reopen");
}
void RizekQssEditor::openFileThroughSystem()
{
	QUrl url;
	url = url.fromLocalFile(addressOfCurrenFile);
	QDesktopServices::openUrl(url);
	pOutput->addItem("Current file was open through system");
}
void RizekQssEditor::showSearchMistakesWindow()
{
	pSearchMistakes->setTextEdit(plainTextEdit);
	pSearchMistakes->show();
	pSearchMistakes->findMistakes();
}
void RizekQssEditor::showScriptWindow()
{
	pScript->show();
}
void RizekQssEditor::saveResources()
{
	bool isResourceListInText = false;
	bool isListWidgetClear = false;
	if (pConnectedResource->listWidget->count()==0)
		isListWidgetClear = true;
	if (plainTextEdit->toPlainText().indexOf("\/\*!Resources:!\*\/")!=-1)
		isResourceListInText = true;
	if (!isResourceListInText && !isListWidgetClear)
	{
		QTextCursor c = plainTextEdit->textCursor();
		c.setPosition(0);
		plainTextEdit->setTextCursor(c);
		plainTextEdit->insertPlainText("/*!Resources:!*/\n");
		for(int a=0;a<pConnectedResource->listWidget->count();a++)
		{
			plainTextEdit->insertPlainText("/*" + pConnectedResource->listWidget->item(a)->text() + "*/\n");
		}
		plainTextEdit->insertPlainText("/*!End of resources!*/\n");
	}
	if (isResourceListInText && !isListWidgetClear)
	{
		int start, finish;
		start = plainTextEdit->toPlainText().indexOf("\/\*!Resources:!\*\/");
		finish = plainTextEdit->toPlainText().indexOf("\/\*!End of resources!\*\/");
		QString text;
		for (int a=start;a<finish-1;a++)
		{
			text += plainTextEdit->toPlainText()[a];
		}
		QStringList recources;
		QString currentPath;
		bool isInside = false;
		text.remove("\/\*!Resources:!\*\/\n");
		for(int a=start+2;a<text.count()-1;a++)
		{
			if (text[a-2]=='/' && text[a-1]=='*')
				isInside = true;
			if (text[a]=='*' && text[a+1]=='/')
			{
				isInside = false;
				recources << currentPath;
				currentPath = "";
			}
			if (isInside)
				currentPath +=text[a];
		}
		
		start = start + QString("\/\*!Resources:!\*\/\n").count()-1;
		for(int a=0;a<pConnectedResource->listWidget->count();a++)
		{
			bool isNoCopy = true;
			for (int b=0;b<recources.count();b++)
			{
				if (pConnectedResource->listWidget->item(a)->text()==recources[b])
				{
					isNoCopy = false;
					break;
				}
			}
			if (isNoCopy)
			{
				QTextCursor c = plainTextEdit->textCursor();
				c.setPosition(start);
				plainTextEdit->setTextCursor(c);
				plainTextEdit->insertPlainText("\n/*" + pConnectedResource->listWidget->item(a)->text() + "*/");
			}
		}
	}
}
void RizekQssEditor::loadResources()
{
	int start, finish;
	start = plainTextEdit->toPlainText().indexOf("\/\*!Resources:!\*\/");
	finish = plainTextEdit->toPlainText().indexOf("\/\*!End of resources!\*\/");
	QString text;
	for (int a=start;a<finish-1;a++)
	{
		text += plainTextEdit->toPlainText()[a];
	}
	text.remove("\/\*!Resources:!\*\/\n");
	bool isInside = false;
	QString currentPath;
	for(int a=start+2;a<text.count()-1;a++)
	{
		if (text[a-2]=='/' && text[a-1]=='*')
			isInside = true;
		if (text[a]=='*' && text[a+1]=='/')
		{
			isInside = false;
			pConnectedResource->listWidget->addItem(currentPath);
			currentPath = "";
		}
		if (isInside)
		{
			currentPath += text[a];
		}
	}
	//qDebug() << text;
}
void RizekQssEditor::setViewStandart()
{
// 	pSearchMistakes->show();
// 	pConnectedResource->show();
// 	pOutput->show();
// 	pScript->show();
	this->addDockWidget(Qt::LeftDockWidgetArea ,pSearchMistakes ,Qt::Horizontal);
	this->addDockWidget(Qt::RightDockWidgetArea ,pConnectedResource ,Qt::Horizontal);
	this->addDockWidget(Qt::BottomDockWidgetArea , pOutput ,Qt::Horizontal);
	this->addDockWidget(Qt::BottomDockWidgetArea , pScript ,Qt::Horizontal);
	this->tabifyDockWidget(pOutput,pScript);
	//this->tabifyDockWidget(pSearchMistakes,pConnectedResource);
}
void RizekQssEditor::setViewLeftBottom()
{
// 	pSearchMistakes->show();
// 	pConnectedResource->show();
// 	pOutput->show();
// 	pScript->show();
	this->addDockWidget(Qt::LeftDockWidgetArea ,pSearchMistakes ,Qt::Horizontal);
	this->addDockWidget(Qt::LeftDockWidgetArea ,pConnectedResource ,Qt::Horizontal);
	this->addDockWidget(Qt::BottomDockWidgetArea , pOutput ,Qt::Horizontal);
	this->addDockWidget(Qt::BottomDockWidgetArea , pScript ,Qt::Horizontal);
	this->tabifyDockWidget(pSearchMistakes,pConnectedResource);
	this->tabifyDockWidget(pOutput,pScript);
}
void RizekQssEditor::setViewRightBottom()
{
// 	pSearchMistakes->show();
// 	pConnectedResource->show();
// 	pOutput->show();
// 	pScript->show();
	this->addDockWidget(Qt::RightDockWidgetArea ,pSearchMistakes ,Qt::Horizontal);
	this->addDockWidget(Qt::RightDockWidgetArea ,pConnectedResource ,Qt::Horizontal);
	this->addDockWidget(Qt::BottomDockWidgetArea , pOutput ,Qt::Horizontal);
	this->addDockWidget(Qt::BottomDockWidgetArea , pScript ,Qt::Horizontal);
	this->tabifyDockWidget(pSearchMistakes,pConnectedResource);
	this->tabifyDockWidget(pOutput,pScript);
}
void RizekQssEditor::setViewClear()
{
	pSearchMistakes->setVisible(false);
	pConnectedResource->setVisible(false);
	pOutput->setVisible(false);
	pScript->setVisible(false);
}
void RizekQssEditor::assotiate()
{
	QSettings settings("HKEY_CLASSES_ROOT\\.qss",
		QSettings::NativeFormat);
	settings.setValue("", "Rizek_Qss_Editor");
}
void RizekQssEditor::openDirOfCurrentFile()
{
	QUrl url;
	url.setPath(QFileInfo(addressOfCurrenFile).dir().absolutePath());
	QDesktopServices::openUrl(url);

}
void RizekQssEditor::hideToTray()
{
	if (!isIconInTray)
	{
		systemTrayIcon = new QSystemTrayIcon;
		QIcon ic;
		ic.addFile(":/ico/icos/package_editors.png");
		systemTrayIcon->setIcon(ic);
		systemTrayIcon->setToolTip("Rizek Color Manager");
		systemTrayIcon->hide();
		this->setVisible(false);
		QMenu * menu = new QMenu;
		QAction * act1 = new QAction(this);
		act1->setText(tr("Show/Hide"));
		menu->addAction(act1);
		connect(act1, SIGNAL(triggered()), this, SLOT(showMainWindow()));
		QAction * act2 = new QAction(this);
		act2->setText(tr("Quit"));
		menu->addAction(act2);
		connect(act2, SIGNAL(triggered()), qApp, SLOT(quit()));
		systemTrayIcon->setContextMenu(menu);
		systemTrayIcon->show();
		connect(systemTrayIcon, SIGNAL(activated( QSystemTrayIcon::ActivationReason)), 
			this, SLOT(showMainWindow( QSystemTrayIcon::ActivationReason)));
		isIconInTray = true;
	}
	else 
		this->setVisible(false);
}
void RizekQssEditor::donate()
{
	QUrl url;
	url.setUrl("https://sites.google.com/site/myrizek/");
	QDesktopServices::openUrl(url);
}
void RizekQssEditor::showResourcesWindow()
{
	pConnectedResource->show();
}
void RizekQssEditor::chekForUpdates()
{
	RUpdater * updater = new RUpdater;
	updater->setCurrentVersion(currentVersion);
	updater->setInformation();
}
void RizekQssEditor::showOutput()
{
	pOutput->show();
}
void RizekQssEditor::removeStarsFromTitle()
{
	this->setWindowTitle(this->windowTitle().replace("*",""));
}
void RizekQssEditor::slotAddToOutput(QString str)
{
	pOutput->addItem(str);
}
void RizekQssEditor::showCursorXY()
{
	QTextCursor cursor = plainTextEdit->textCursor();
	statusBarLabel->setText(QString("String: %1; Sign: %2;").arg(cursor.blockNumber()+1).arg(cursor.position()));
}
