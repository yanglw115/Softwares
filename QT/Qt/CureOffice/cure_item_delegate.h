#ifndef CUREITEMDELEGATE_H
#define CUREITEMDELEGATE_H

#include <QItemDelegate>
#include <QStyledItemDelegate>

class CureCheckDelegate : public QStyledItemDelegate
{
public:
    explicit CureCheckDelegate(QObject * parent = 0);
    virtual ~CureCheckDelegate();

    virtual QWidget *createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    virtual void setEditorData(QWidget * editor, const QModelIndex & index) const;
    virtual void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;
    virtual void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const;
#if 0
    virtual void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
protected:
    virtual bool editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index);
#endif
};

#endif // CUREITEMDELEGATE_H
