#include "programCompleter.h"
#include "QTextCursor"
#include "QString"
#include "QObject"
#include "QDebug"
#include "QTextBlock"

programCompleter::programCompleter()
{

}
programCompleter::~programCompleter()
{

}
void programCompleter::setTextEdit(QPlainTextEdit * textEdit)
{
	plainTextEdit = textEdit;
	QObject::connect(plainTextEdit, SIGNAL(makeSignCompletion()), this, SLOT(complete()));
	QObject::connect(plainTextEdit, SIGNAL(makeIndent()), this, SLOT(indent()));
}
void programCompleter::complete()
{
	setHalfRoundBrackets();
	setRoundBrackets();
	setSquareBrackets();
	setQuotes();
	setNormalComment();
}
void programCompleter::indent()
{
	setIndent();
}
void programCompleter::setHalfRoundBrackets()
{
	bool isConnect = true;
	int currentPosition = plainTextEdit->textCursor().anchor();
	if (QString(plainTextEdit->toPlainText()[currentPosition-1])==QString("{")
		&& QString(plainTextEdit->toPlainText()[currentPosition])!=QString("}"))
	{
		QObject::disconnect(plainTextEdit, SIGNAL(makeSignCompletion()), this, SLOT(complete()));
		isConnect = false;
		plainTextEdit->textCursor().insertText("\n	\n}");
		QTextCursor cursor = plainTextEdit->textCursor();
		cursor.setPosition(currentPosition+2);
		plainTextEdit->setTextCursor(cursor);
	}
	if (!isConnect)
		QObject::connect(plainTextEdit, SIGNAL(makeSignCompletion()), this, SLOT(complete()));
}
void programCompleter::setRoundBrackets()
{
	bool isConnect = true;
	int currentPosition = plainTextEdit->textCursor().anchor();
	if (QString(plainTextEdit->toPlainText()[currentPosition-1])==QString("(")
		&& QString(plainTextEdit->toPlainText()[currentPosition])!=QString(")"))
	{
		QObject::disconnect(plainTextEdit, SIGNAL(makeSignCompletion()), this, SLOT(complete()));
		isConnect = false;
		plainTextEdit->textCursor().insertText(")");
		QTextCursor cursor = plainTextEdit->textCursor();
		cursor.setPosition(currentPosition);
		plainTextEdit->setTextCursor(cursor);
	}
	if (!isConnect)
		QObject::connect(plainTextEdit, SIGNAL(makeSignCompletion()), this, SLOT(complete()));
}
void programCompleter::setSquareBrackets()
{
	bool isConnect = true;
	int currentPosition = plainTextEdit->textCursor().anchor();
	if (QString(plainTextEdit->toPlainText()[currentPosition-1])==QString("[")
		&& QString(plainTextEdit->toPlainText()[currentPosition])!=QString("]"))
	{
		QObject::disconnect(plainTextEdit, SIGNAL(makeSignCompletion()), this, SLOT(complete()));
		isConnect = false;
		plainTextEdit->textCursor().insertText("]");
		QTextCursor cursor = plainTextEdit->textCursor();
		cursor.setPosition(currentPosition);
		plainTextEdit->setTextCursor(cursor);
	}
	if (!isConnect)
		QObject::connect(plainTextEdit, SIGNAL(makeSignCompletion()), this, SLOT(complete()));
}
void programCompleter::setQuotes()
{
	bool isConnect = true;
	int currentPosition = plainTextEdit->textCursor().anchor();
	if (QString(plainTextEdit->toPlainText()[currentPosition-1])==QString("\"")
		&& QString(plainTextEdit->toPlainText()[currentPosition])!=QString("\""))
	{
		QObject::disconnect(plainTextEdit, SIGNAL(makeSignCompletion()), this, SLOT(complete()));
		isConnect = false;
		plainTextEdit->textCursor().insertText("\"");
		QTextCursor cursor = plainTextEdit->textCursor();
		cursor.setPosition(currentPosition);
		plainTextEdit->setTextCursor(cursor);
	}
	if (!isConnect)
		QObject::connect(plainTextEdit, SIGNAL(makeSignCompletion()), this, SLOT(complete()));
}
void programCompleter::setIndent()
{
	QObject::disconnect(plainTextEdit, SIGNAL(makeIndent()), this, SLOT(indent()));
	bool isClass = false;
	bool cont = true;
	bool isComment = false;
	for(int a=plainTextEdit->textCursor().position();a>=0;--a)
	{
		if (a!=plainTextEdit->textCursor().position())
		{
			if (plainTextEdit->toPlainText()[a]=='*' && plainTextEdit->toPlainText()[a+1]=='/')
				isComment=true;
			if (plainTextEdit->toPlainText()[a]=='/' && plainTextEdit->toPlainText()[a+1]=='*')
				isComment=false;
		}
		if(plainTextEdit->toPlainText()[a]==QChar('}') && !isComment)//вне класса
		{
			isClass = true;
			break;
		}
		else
			if(plainTextEdit->toPlainText()[a]==QChar(';') || plainTextEdit->toPlainText()[a]==QChar('{') && !isComment) //внутри класса
			{
				isClass=false;
				break;
			}
		if(a==0)//Если текст первый то отступ не нужен
			{
				isClass=true;
			}
	}
	if (!isClass)
		plainTextEdit->textCursor().insertText("	");
	QObject::connect(plainTextEdit, SIGNAL(makeIndent()), this, SLOT(indent()));
}
void programCompleter::setNormalComment()
{
	QString text = plainTextEdit->toPlainText();
	bool isConnect = true;
	int currentPosition = plainTextEdit->textCursor().anchor();
	if (text[currentPosition-2]=='\/' && text[currentPosition-1]=='\/' && currentPosition!=text.size())
	{
		QObject::disconnect(plainTextEdit, SIGNAL(makeSignCompletion()), this, SLOT(complete()));
		isConnect = false;
		int endOfLinePosition;
		for(int a = currentPosition;text[a]!='\n';a++)
		{
			if(text[a]!='\n')
				endOfLinePosition = a;
		}
		QTextCursor cursor = plainTextEdit->textCursor();
		cursor.setPosition(endOfLinePosition+1);
		plainTextEdit->setTextCursor(cursor);
		plainTextEdit->textCursor().insertText("*\/");
		cursor.setPosition(currentPosition);
		plainTextEdit->setTextCursor(cursor);
		cursor.deletePreviousChar();
		plainTextEdit->textCursor().insertText("*");
	}
	if (!isConnect)
		QObject::connect(plainTextEdit, SIGNAL(makeSignCompletion()), this, SLOT(complete()));
}