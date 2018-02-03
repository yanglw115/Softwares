#ifndef CLABELIMAGEDRAG_H
#define CLABELIMAGEDRAG_H

#include <QObject>
#include <QLabel>
#include <QDropEvent>
#include <QDragEnterEvent>

class CLabelImageDrag : public QLabel
{
    Q_OBJECT
public:
    CLabelImageDrag(QWidget * parent = 0, Qt::WindowFlags f = 0);
    CLabelImageDrag(const QString & text, QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~CLabelImageDrag();
    void changePixmap(const QString &strPath);
signals:
    void sigPixmapChanged(const QString &strPath);
protected:
    virtual void dragEnterEvent(QDragEnterEvent * event);
    virtual void dropEvent(QDropEvent * event);
};

#endif // CLABELIMAGEDRAG_H
