#include "cure_structure_model.h"
#include "msvs_charset.h"


CureStructureModel::CureStructureModel(const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "partment" << "leader";
    rootItem = new CureTreeItem(rootData);
    setupModelData(data.split(QString("\n")), rootItem);
}

CureStructureModel::~CureStructureModel()
{
    delete rootItem;
}

QVariant CureStructureModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    CureTreeItem *item = static_cast<CureTreeItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags CureStructureModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant CureStructureModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex CureStructureModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    CureTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<CureTreeItem*>(parent.internalPointer());

    CureTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex CureStructureModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    CureTreeItem *childItem = static_cast<CureTreeItem*>(index.internalPointer());
    CureTreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int CureStructureModel::rowCount(const QModelIndex &parent) const
{
    CureTreeItem *parentItem = NULL;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<CureTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int CureStructureModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<CureTreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

void CureStructureModel::setupModelData(const QStringList &lines, CureTreeItem *parent)
{
    QList<CureTreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].mid(position, 1) != " ")
                break;
            position++;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QList<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            parents.last()->appendChild(new CureTreeItem(columnData, parents.last()));
            parents.last()->child(0)->appendChild(new CureTreeItem(columnData, parents.last()));
            parents.last()->child(0)->appendChild(new CureTreeItem(columnData, parents.last()));
        }

        ++number;
    }
}
