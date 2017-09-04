#ifndef REPLACE
#define REPLACE

#include "ui_replace.h"
#include "qssTextEdit/qssTextEdit.h"

class replace : public QDialog, public Ui::Dialog
{
	Q_OBJECT
public:
    replace(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~replace();
	void setPlaintTextEdit(qssTextEdit * textEdit);
protected:
	qssTextEdit * plainTextEdit;
protected slots:
	void replaceInTheText();
};
#endif
