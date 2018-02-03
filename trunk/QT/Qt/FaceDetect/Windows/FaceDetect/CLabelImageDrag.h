#ifndef CLABELIMAGEDRAG_H
#define CLABELIMAGEDRAG_H

#include <QObject>
#include <QLabel>
#include <QDropEvent>
#include <QDragEnterEvent>

class CLabelImageDrag : public QLabel
{
public:
    CLabelImageDrag(QWidget * parent = 0, Qt::WindowFlags f = 0);
    CLabelImageDrag(const QString & text, QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~CLabelImageDrag();

protected:
    virtual void dragEnterEvent(QDragEnterEvent * event);
    virtual void dropEvent(QDropEvent * event);
};

#endif // CLABELIMAGEDRAG_H
