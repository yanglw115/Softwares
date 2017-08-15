#include <QFileInfo>
#include <QtDebug>

#include "YLW_file_line_edit.h"

FileLineEdit::FileLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    this->setAcceptDrops(true);
}


FileLineEdit::~FileLineEdit()
{

}

void FileLineEdit::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "FileLineEdit: drag enter event...";
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
//        QLineEdit::dragEnterEvent(event);
    }
}

void FileLineEdit::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        QList<QUrl> listUrls = event->mimeData()->urls();
        QFileInfo *pFileInfo = new QFileInfo(listUrls.at(0).toLocalFile());
        setText(pFileInfo->absoluteFilePath());
        event->acceptProposedAction();
    }
}
