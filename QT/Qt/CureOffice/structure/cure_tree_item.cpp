#include "cure_tree_item.h"

#include <QtDebug>

CureTreeItem::CureTreeItem(const QList<QVariant> &data, CureTreeItem *parentItem)
    : m_listData(data)
    , m_pParent(parentItem)
{

}

CureTreeItem::~CureTreeItem()
{
    qDeleteAll(m_listChild);
}

void CureTreeItem::appendChild(CureTreeItem *child)
{
    if (child) {
        m_listChild.append(child);
    } else {
        qWarning() << "Input child is NULL!";
    }
}

CureTreeItem *CureTreeItem::child(int row)
{
    if (row <= m_listChild.size() -1) {
        return m_listChild.at(row);
    }
    qWarning() << "Invalid row index for tree item's child!";
}

int CureTreeItem::childCount() const
{
    return m_listChild.size();
}

int CureTreeItem::columnCount() const
{
    return m_listData.count(); // the same with size()
}

QVariant CureTreeItem::data(int column) const
{
    if (column <= m_listData.size() - 1) {
        return m_listData.at(column);
    }
    qWarning() << "Invalid column index for tree item data!";
}

int CureTreeItem::row() const
{
    if (m_pParent) {
        return m_pParent->m_listChild.indexOf(const_cast<CureTreeItem*>(this));
    }
    return 0;
}

CureTreeItem *CureTreeItem::parentItem()
{
    return m_pParent;
}
