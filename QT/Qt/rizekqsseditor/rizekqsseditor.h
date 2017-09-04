#ifndef RIZEKQSSEDITOR_H
#define RIZEKQSSEDITOR_H

#include <QMainWindow>
#include "ui_rizekqsseditor.h"
#include "syntax/highlighter.h"
#include "RScript/RScript.h"
#include "QPushButton"
#include "QLineEdit"
#include "QLabel"
#include "prevnextWidget/prevnextWidget.h"
#include "QCompleter"
#include "searchMistakes/searchMistakes.h"
#include "Output/Output.h"
#include "ConnectedResourcres/ConnectedResourcres.h"
#include "QSystemTrayIcon"
#include "qssTextEdit/qssTextEdit.h"

class RizekQssEditor : public QMainWindow ,Ui::RizekQssEditorClass
{
	Q_OBJECT

public:
    RizekQssEditor(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~RizekQssEditor(); 

	outputWindow* getOutputWindow() const {return pOutput;}
	qssTextEdit* getPlainTextEdit() const {return plainTextEdit;}
private:
	Ui::RizekQssEditorClass ui;
	searchMistakes * pSearchMistakes;
	RConnectedResource * pConnectedResource;
	outputWindow * pOutput;
	scriptWindow * pScript;
	QLabel * statusBarLabel;
	QSystemTrayIcon * systemTrayIcon;
	Highlighter * syntax;

	QString addressOfCurrenFile;
	QString currentVersion;
	bool isIconInTray;
	bool isModified;
	//ToolBar
	QPushButton * next;
	QPushButton * prev;
	QLabel * searchLabel;
	QLineEdit * searchLineEdit;
	QPushButton * searchButton;
	QCompleter * completer;
	prevnextWidget * prevNextButtons;
	//End of ToolBar
	void changeAddressOfCurrentFile(QString file);

	void buildToolBar();

	void loadRecentProjects();
	void rewriteRecentProjects();
	void saveResources();
	void loadResources();

	void removeStarsFromTitle();

	QAbstractItemModel *modelFromFile(const QString& fileName);
	void assotiate(); // Wrong
protected:
	void resizeEvent ( QResizeEvent * );
	void timerEvent  ( QTimerEvent  * );
	void closeEvent  ( QCloseEvent  * );
protected slots:
    void findInTheText(const QString & text);
	void findNext();
	void setSettings();
	void openFile();
	void saveAsInFile();
	void saveInFile();
	void createNewDocument();
	void showCursorXY();

	void openReplaceWindow();
	void openSearchWindow();
	void openPreviewWindow();
	void openSettingsWindow();
	void openHelpWindow();
	void openAboutProgramWindow();

	void showSearchMistakesWindow();
	void showResourcesWindow();
	void showScriptWindow();
	void showOutput();

	void setViewStandart();
	void setViewLeftBottom();
	void setViewRightBottom();
	void setViewClear();

	void showOnlineDocumentation();
	void showOnlineExamples();
	void setPlainTextEditModified();
	void openFileFromString(QString file);
	void openFileByActionClicked();
	void printFile();
	void insertColor16bit();
	void insertFontName();
	void reopenCurrentFile();
	void openFileThroughSystem();
	void openDirOfCurrentFile();

	void hideToTray();
	void showMainWindow() {this->setVisible(!this->isVisible());}
	void showMainWindow(QSystemTrayIcon::ActivationReason actr) { if(actr == QSystemTrayIcon::DoubleClick) this->setVisible(!this->isVisible());}

	void donate();
	void chekForUpdates();
public slots:
	void slotAddToOutput(QString);
};

#endif // RIZEKQSSEDITOR_H
