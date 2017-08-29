#include <QList>
#include <QUrl>
#include <QFileInfo>
#include <QMimeData>
#include <QtDebug>

#include "YLW_my_table_view.h"
#include "YLW_VS_char_set.h"

MyTableView::MyTableView(QWidget *parent)
    : QTableView(parent)
{
    this->setAcceptDrops(true);
    this->setDragDropMode(QAbstractItemView::DragDrop);
    this->setDragEnabled(true);
    this->setDropIndicatorShown(true);
    this->setSortingEnabled(true);
}


MyTableView::~MyTableView()
{

}

/* 此方法获取到的模型索引，包含所有已选列，而不是我们想要的只是行；使用QModelIndex::row()将得到很多重复的行，
 * 为此，我们可以直接使用QTableView::selectionModel::selectedRows(int column)来只获取1行的所有行 */
QModelIndexList MyTableView::getSelectedIndexes()
{
    QModelIndexList list = this->selectedIndexes();
    return list;
}

void MyTableView::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "MyTableView drag enter event...";
    if (event->mimeData()->hasUrls()) { //hasFormat("text/uri-list")) {
        event->acceptProposedAction();
        /* 通过继承的方式重新实现dragEnterEvent之后，下面还需要再调用父类的方法，不然程序运行之后表现出来拖动还是被禁止的 */
        QTableView::dragEnterEvent(event);
    }
}

void MyTableView::dropEvent(QDropEvent *event)
{
    qDebug() << "MyTableView drop enter event...";
    if (event->mimeData()->hasUrls())
    {
        QList<QUrl> listUrls = event->mimeData()->urls();
        QFileInfo *pFileInfo = new QFileInfo(listUrls.at(0).toLocalFile());
        emit sigFileIn(pFileInfo->absoluteFilePath());
        event->acceptProposedAction();
    }
}
