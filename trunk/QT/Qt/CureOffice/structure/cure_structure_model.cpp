#include <QtDebug>

#include "cure_structure_model.h"
#include "msvs_charset.h"
#include "cure_global.h"

CureStructureModel::CureStructureModel(const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "部门" << "人数" << "负责人";
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
    parents << parent;

#if 0
    QList<int> indentations;

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
#else
    Q_UNUSED(lines)
    if (g_db.isOpen()) {
        QStringList strTables = g_db.tables();
        if (strTables.contains(g_strTableNameStructure)) {
            if (getChildDataFromDB(1, "", parent)) {
                qDebug() << "Read structure data from database success!";
                return;
            }
            /* 查询失败 */
            /* 因为失败,所以要回收根结点以外的所有数据,因为数据不完整了 */
            for (int i = 0; i < parents[0]->childCount(); ++i) {
                delete parents[0]->child(i);
            }

//            QMessageBox::warning(this, tr("数据库操作"), tr("从数据库中读取公司结构表数据失败!"));
            qWarning() << "Run sql query failed when read data from table: " << g_strTableNameStructure;
            qWarning() << "" << g_db.lastError().text();
        } else {
            qWarning() << "Read structure table from database failed, cannot find the table!";
//            QMessageBox::critical(this, tr("数据库操作"), tr("未在本地数据库中找到公司结构表!"));
        }
    }
#endif
}

bool CureStructureModel::getChildDataFromDB(const int nGrade, const QString &strParent, CureTreeItem *pParent)
{
    QSqlQuery query(g_db);
    QList<QVariant> columnData;

    QString strExec = QString("select * from %1 where grade=%2 and parentName='%3'").arg(g_strTableNameStructure).arg(nGrade).arg(strParent);
    if (query.exec(strExec)) {
        bool bHasResult = false;
        while (query.next()) {
            bHasResult = true;
            int nGrade = query.value("grade").toInt();
            QString strName = query.value("name").toString();
            QString strParentName = query.value("parentName").toString();
            QString strLeader = query.value("leader").toString();
            QString strJob = query.value("job").toString();
            int numbers = query.value("numbers").toInt();

            columnData.clear();
            columnData << strName << numbers << strLeader;
            CureTreeItem *pChild = new CureTreeItem(columnData, pParent);
            if (getChildDataFromDB(nGrade + 1, strName, pChild)) {
                pParent->appendChild(pChild);
            } else {
                return false;
            }
        }
        return true;
    }
    qWarning() << "Read data from database failed: " << g_db.lastError().text();
    return false;
}
