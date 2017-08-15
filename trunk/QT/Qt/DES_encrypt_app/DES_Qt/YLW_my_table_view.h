#ifndef MYTABLEVIEW_H
#define MYTABLEVIEW_H

#include <QTableView>
#include <QDragEnterEvent>
#include <QdropEvent>

/* 当时创建这个类是因为，QTableView中的有些方法是protected类型的，不能通过对象实例直接调用，需要重新封装一遍 */

class MyTableView : public QTableView
{
    Q_OBJECT
public:
    explicit MyTableView(QWidget *parent = 0);
    virtual ~MyTableView();

    QModelIndexList getSelectedIndexes();
signals:
    void sigFileIn(const QString &);
protected:
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);
};

#endif // MYTABLEVIEW_H
