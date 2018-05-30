#ifndef CURETREEITEM_H
#define CURETREEITEM_H

#include <QList>
#include <QVariant>

class CureTreeItem
{
public:
    explicit CureTreeItem(const QList<QVariant> &data, CureTreeItem *parentItem = 0);
    virtual ~CureTreeItem();

    void appendChild(CureTreeItem *child);

    CureTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    CureTreeItem *parentItem();

private:
    QList<CureTreeItem*> m_listChild;
    QList<QVariant> m_listData;
    CureTreeItem *m_pParent;
};

#endif // CURETREEITEM_H
