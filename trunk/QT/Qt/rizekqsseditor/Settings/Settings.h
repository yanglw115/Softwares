#ifndef SETTINGS
#define SETTINGS

#include "ui_Settings.h"
#include <QDialog>
#include "QString"

class settings : public QDialog, public Ui::Settings_Window
{
	Q_OBJECT
public:
    settings(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~settings();
private:
	void loadSettings();
	//--------Colors
	QColor colorOfLocation;
	QColor colorOfObject;
	QColor colorOfFunction;
	QColor colorOfClasses;
	QColor colorOfValues;
	QColor colorOfPseudoStates;
	QColor colorOfSubControls;
	QColor colorOfColors;
	QColor colorOfComment;
	QColor colorOfHC;
	QColor colorOfHTC;
	//--------Bools
	bool isSaveWhenUserExit;
	bool isAutosave;
private slots:
	void setSettings();
	void getFont();
	void getHC();
	void getHTC();
	//
	void getColorOfLocationFormat();
	void getColorOfObjectFormat();
	void getColorOfFunctionFormat();
	void getColorOfClassesFormat();
	void getColorOfValuesFormat();
	void getColorOfPseudoStatesFormat();
	void getColorOfSubControlsFormat();
	void getColorOfColorsFormat();
	void getColorOfCommentFormat();
	//--------
	void getSaveWhenUserExit(int is);
	void getAutosave(int is);
};
#endif
