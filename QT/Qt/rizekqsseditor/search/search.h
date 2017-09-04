#ifndef SEARCH
#define SEARCH

#include "ui_search.h"
#include "qssTextEdit/qssTextEdit.h"
#include "QDialog"

class search : public QDialog, public Ui::searchDialog
{
	Q_OBJECT
public:
    search(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~search();
	void setPlaintTextEdit(qssTextEdit * textEdit);
protected:
	qssTextEdit * plainTextEdit;
protected slots:
	void findNext();
};
#endif
