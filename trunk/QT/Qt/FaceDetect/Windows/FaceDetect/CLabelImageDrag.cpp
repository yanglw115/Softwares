#include "CLabelImageDrag.h"

#include <QFileInfo>
#include <QUrl>
#include <QList>
#include <QPixmap>
#include <QMessageBox>
#include <QMimeData>
#include <QtDebug>

CLabelImageDrag::CLabelImageDrag(QWidget *parent, Qt::WindowFlags f)
    : QLabel(parent, f)
{
    this->setAcceptDrops(true);
}

CLabelImageDrag::CLabelImageDrag(const QString &text, QWidget *parent, Qt::WindowFlags f)
    : QLabel(text, parent, f)
{
    this->setAcceptDrops(true);
}

CLabelImageDrag::~CLabelImageDrag()
{

}

void CLabelImageDrag::changePixmap(const QString &strPath)
{
    QPixmap pixmap;
    qDebug() << "Selected image path: " << strPath;
    if (pixmap.load(strPath)) {
        pixmap = pixmap.scaled(this->width(), this->height(), Qt::KeepAspectRatio);
        this->setPixmap(pixmap);
        emit sigPixmapChanged(strPath);
    } else {
        QMessageBox::critical(this, "Load image", "Load image failed, please check!");
    }
}

void CLabelImageDrag::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        //event->setDropAction(Qt::LinkAction);
        event->acceptProposedAction();
        //QLabel::dragEnterEvent(event);
    }
}

void CLabelImageDrag::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        QList<QUrl> listUrls = event->mimeData()->urls();
        QFileInfo *pFileInfo = new QFileInfo(listUrls.at(0).toLocalFile());
        changePixmap(pFileInfo->absoluteFilePath());
        event->acceptProposedAction();
    }
}

