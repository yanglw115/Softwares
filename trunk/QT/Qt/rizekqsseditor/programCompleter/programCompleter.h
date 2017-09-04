#ifndef PROGRAMCOMPLETER
#define PROGRAMCOMPLETER

#include "QPlainTextEdit"
#include "QObject"

class programCompleter: public QObject
{
	Q_OBJECT
public:
	programCompleter();
	~programCompleter();
	void setTextEdit(QPlainTextEdit * textEdit);
private:
	QPlainTextEdit * plainTextEdit;
	//------Дополнить
	void setHalfRoundBrackets();
	void setRoundBrackets();
	void setSquareBrackets();
	void setQuotes();
	void setNormalComment();
	//--------------
	void setIndent();
protected slots:
	void complete();
	void indent();
};
#endif