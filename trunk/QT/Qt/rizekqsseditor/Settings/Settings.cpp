#include "Settings.h"
#include "QString"
#include "QFile"
#include "QStyleFactory"
#include "QSettings"
#include "QFontDialog"
#include "QVariant"
#include "QColor"
#include "QColorDialog"
#include "QMessageBox"

settings::settings(QWidget *parent, Qt::WindowFlags flags)
: QDialog(parent, flags)
{
	setupUi(this);
	QSettings settings("Rizek", "Rizek Qss Editor");
	if (settings.value("this\UseStyleSheet").toBool())
	{
		QFile file(":/Other/qss/default.qss");
		file.open(QFile::ReadOnly);
		QString styleSheet = QLatin1String(file.readAll());
		this->setStyleSheet(styleSheet);
	}
	//
	comboBox_3->addItems(QStyleFactory::keys());
	loadSettings();

	QIcon ic;
	ic.addFile(":/ico/icos/settings.png");
	this->setWindowIcon(ic);

	connect(pushButton_2, SIGNAL(clicked()), this, SLOT(setSettings()));
	connect(pushButton_2, SIGNAL(clicked()), this, SLOT(close()));
	connect(pushButton, SIGNAL(clicked()), this, SLOT(getFont()));
	connect(pushButton_4, SIGNAL(clicked()), this, SLOT(getHC()));
	connect(pushButton_5, SIGNAL(clicked()), this, SLOT(getHTC()));
	//
	connect(pushButton_6 , SIGNAL(clicked()), this, SLOT(getColorOfLocationFormat()));
	connect(pushButton_7 , SIGNAL(clicked()), this, SLOT(getColorOfObjectFormat()));
	connect(pushButton_8 , SIGNAL(clicked()), this, SLOT(getColorOfFunctionFormat()));
	connect(pushButton_9 , SIGNAL(clicked()), this, SLOT(getColorOfClassesFormat()));
	connect(pushButton_10 , SIGNAL(clicked()), this, SLOT(getColorOfValuesFormat()));
	connect(pushButton_11 , SIGNAL(clicked()), this, SLOT(getColorOfPseudoStatesFormat()));
	connect(pushButton_12 , SIGNAL(clicked()), this, SLOT(getColorOfSubControlsFormat()));
	connect(pushButton_13 , SIGNAL(clicked()), this, SLOT(getColorOfColorsFormat()));
	connect(pushButton_14 , SIGNAL(clicked()), this, SLOT(getColorOfCommentFormat()));
	//------------
	connect(checkBox , SIGNAL(stateChanged(int)), this, SLOT(getSaveWhenUserExit(int)));
	connect(checkBox_2 , SIGNAL(stateChanged(int)), this, SLOT(getAutosave(int)));
}

settings::~settings()
{

}
void settings::loadSettings()
{
	QSettings set("Rizek","Rizek Qss Editor");
	comboBox_3->setCurrentIndex(comboBox_3->findText(set.value("style").toString()));
	checkBox_4->setChecked(set.value("this\UseStyleSheet", true).toBool());
	comboBox_5->setCurrentIndex(set.value("this\WhenClosing", 0).toInt());
	label_5->setText(set.value("font").value<QFont>().rawName());
	label_7->setText(QString::number(set.value("font").value<QFont>().pointSize()));

	comboBox_2->setCurrentIndex(set.value("Completer\CompletionMode", 0).toInt());
	comboBox_4->setCurrentIndex(set.value("Completer\CaseSensitivity", 1).toInt());
	checkBox_3->setChecked(set.value("Completer\WrapAround", false).toBool());
    label_10->setText(set.value("TextEdit\HC", QColor("#468dd4")).toString());
    label_12->setText(set.value("TextEdit\HTC", int(Qt::white)).toString());
	//
    label_14->setText(set.value("Highlighter\ColorOfLocationFormat", int(Qt::darkYellow)).toString());
    label_16->setText(set.value("Highlighter\ColorOfObjectFormat", int(Qt::blue)).toString());
    label_18->setText(set.value("Highlighter\ColorOfFunctionFormat", int(Qt::darkRed)).toString());
    label_20->setText(set.value("Highlighter\ColorOfClassesFormat", int(Qt::darkCyan)).toString());
    label_22->setText(set.value("Highlighter\ColorOfValuesFormat", int(Qt::blue)).toString());
    label_24->setText(set.value("Highlighter\ColorOfPseudoStatesFormat", int(Qt::darkMagenta)).toString());
    label_26->setText(set.value("Highlighter\ColorOfSubControlsFormat", int(Qt::darkRed)).toString());
	label_28->setText(set.value("Highlighter\ColorOfColorsFormat", QColor::fromRgb(85, 170, 255)).toString());
    label_30->setText(set.value("Highlighter\ColorOfCommentFormat", int(Qt::red)).toString());
	//
    colorOfLocation.setNamedColor(set.value("Highlighter\ColorOfLocationFormat", int(Qt::darkYellow)).toString());
    colorOfObject.setNamedColor(set.value("Highlighter\ColorOfObjectFormat", int(Qt::blue)).toString());
    colorOfFunction.setNamedColor(set.value("Highlighter\ColorOfFunctionFormat", int(Qt::darkRed)).toString());
    colorOfClasses.setNamedColor(set.value("Highlighter\ColorOfClassesFormat", int(Qt::darkCyan)).toString());
    colorOfValues.setNamedColor(set.value("Highlighter\ColorOfValuesFormat", int(Qt::blue)).toString());
    colorOfPseudoStates.setNamedColor(set.value("Highlighter\ColorOfPseudoStatesFormat", int(Qt::darkMagenta)).toString());
    colorOfSubControls.setNamedColor(set.value("Highlighter\ColorOfSubControlsFormat", int(Qt::darkRed)).toString());
	colorOfColors.setNamedColor(set.value("Highlighter\ColorOfColorsFormat", QColor::fromRgb(85, 170, 255)).toString());
    colorOfComment.setNamedColor(set.value("Highlighter\ColorOfCommentFormat", int(Qt::red)).toString());
	//
	colorOfHC.setNamedColor(set.value("TextEdit\HC").toString());
	colorOfHTC.setNamedColor(set.value("TextEdit\HTC").toString());
	//
	isSaveWhenUserExit = set.value("SaveWhenUserExit", false).toBool();
	isAutosave = set.value("Autosave", true).toBool();
	spinBox->setValue(set.value("Autosave\Time", 180).toInt());
	checkBox->setChecked(isSaveWhenUserExit);
	checkBox_2->setChecked(isAutosave);
}
void settings::setSettings()
{
	QSettings set("Rizek","Rizek Qss Editor");
	set.setValue("style",comboBox_3->currentText());
	set.setValue("this\UseStyleSheet", checkBox_4->isChecked());

	set.setValue("Completer\CompletionMode",comboBox_2->currentIndex());
	set.setValue("Completer\CaseSensitivity",comboBox_4->currentIndex());
	set.setValue("Completer\WrapAround", checkBox_3->isChecked());

	set.setValue("this\WhenClosing", comboBox_5->currentIndex());
	set.setValue("TextEdit\HC", colorOfHC);
	set.setValue("TextEdit\HTC", colorOfHTC);
	//
	set.setValue("Highlighter\ColorOfLocationFormat", colorOfLocation);
	set.setValue("Highlighter\ColorOfObjectFormat", colorOfObject);
	set.setValue("Highlighter\ColorOfFunctionFormat", colorOfFunction);
	set.setValue("Highlighter\ColorOfClassesFormat", colorOfClasses);
	set.setValue("Highlighter\ColorOfValuesFormat", colorOfValues);
	set.setValue("Highlighter\ColorOfPseudoStatesFormat", colorOfPseudoStates);
	set.setValue("Highlighter\ColorOfSubControlsFormat", colorOfSubControls);
	set.setValue("Highlighter\ColorOfColorsFormat", colorOfColors);
	set.setValue("Highlighter\ColorOfCommentFormat", colorOfComment);
	//
	set.setValue("SaveWhenUserExit", isSaveWhenUserExit);
	set.setValue("Autosave", isAutosave);
	set.setValue("Autosave\Time", spinBox->value());
}
void settings::getFont()
{
	QSettings set("Rizek","Rizek Qss Editor");
	if (set.value("font").toString().isEmpty())
		set.setValue("font",QFontDialog::getFont(0, this->font()));
	else 
		set.setValue("font",QFontDialog::getFont(0,set.value("font").value<QFont>())); 
	label_5->setText(set.value("font").value<QFont>().rawName());
	label_7->setText(QString::number(set.value("font").value<QFont>().pointSize()));
}
void settings::getHC()
{
	QSettings set("Rizek","Rizek Qss Editor");
	QColor c = QColorDialog::getColor();
	label_10->setText(c.name());
	colorOfHC = c;
	//set.setValue("TextEdit\HC", c.name());
}
void settings::getHTC()
{
	QSettings set("Rizek","Rizek Qss Editor");
	QColor c = QColorDialog::getColor();
	label_12->setText(c.name());
	colorOfHTC = c;
	//set.setValue("TextEdit\HTC", c.name());
}
void settings::getColorOfLocationFormat()
{
	QSettings set("Rizek","Rizek Qss Editor");
	QColor c = QColorDialog::getColor();
	label_14->setText(c.name());
	colorOfLocation = c;
	//set.setValue("Highlighter\ColorOfLocationFormat", c.name());
}
void settings::getColorOfObjectFormat()
{
	QSettings set("Rizek","Rizek Qss Editor");
	QColor c = QColorDialog::getColor();
	label_16->setText(c.name());
	colorOfObject = c;
	//set.setValue("Highlighter\ColorOfObjectFormat", c.name());
}
void settings::getColorOfFunctionFormat()
{
	QSettings set("Rizek","Rizek Qss Editor");
	QColor c = QColorDialog::getColor();
	label_18->setText(c.name());
	colorOfFunction = c;
	//set.setValue("Highlighter\ColorOfFunctionFormat", c.name());
}
void settings::getColorOfClassesFormat()
{
	QSettings set("Rizek","Rizek Qss Editor");
	QColor c = QColorDialog::getColor();
	label_20->setText(c.name());
	colorOfClasses = c;
	//set.setValue("Highlighter\ColorOfClassesFormat", c.name());
}
void settings::getColorOfValuesFormat()
{ 
	QSettings set("Rizek","Rizek Qss Editor");
	QColor c = QColorDialog::getColor();
	label_22->setText(c.name());
	colorOfValues = c;
	//set.setValue("Highlighter\ColorOfValuesFormat", c.name());
}
void settings::getColorOfPseudoStatesFormat()
{ 
	QSettings set("Rizek","Rizek Qss Editor");
	QColor c = QColorDialog::getColor();
	label_24->setText(c.name());
	colorOfPseudoStates = c;
	//set.setValue("Highlighter\ColorOfPseudoStatesFormat", c.name());
}
void settings::getColorOfSubControlsFormat()
{ 
	QSettings set("Rizek","Rizek Qss Editor");
	QColor c = QColorDialog::getColor();
	label_26->setText(c.name());
	colorOfSubControls = c;
	//set.setValue("Highlighter\ColorOfSubControlsFormat", c.name());
}
void settings::getColorOfColorsFormat()
{
	QSettings set("Rizek","Rizek Qss Editor");
	QColor c = QColorDialog::getColor();
	label_28->setText(c.name());
	colorOfColors = c;
	//set.setValue("Highlighter\ColorOfColorsFormat", c.name());
}
void settings::getColorOfCommentFormat()
{
	QSettings set("Rizek","Rizek Qss Editor");
	QColor c = QColorDialog::getColor();
	label_30->setText(c.name());
	colorOfComment = c;
	//set.setValue("Highlighter\ColorOfCommentFormat", c.name());
}
void settings::getSaveWhenUserExit(int is)
{
	QSettings set("Rizek","Rizek Qss Editor");
	isSaveWhenUserExit = checkBox->isChecked();
}
void settings::getAutosave(int is)
{
	QSettings set("Rizek","Rizek Qss Editor");
	isAutosave = checkBox_2->isChecked();
}
