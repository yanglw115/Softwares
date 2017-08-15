#ifndef FILELINEEDIT_H
#define FILELINEEDIT_H

#include <QLineEdit>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

class FileLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit FileLineEdit(QWidget *parent = 0);
    ~FileLineEdit();
protected:
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);
};

#endif // FILELINEEDIT_H
