#ifndef CURESTRUCTUREMODEL_H
#define CURESTRUCTUREMODEL_H

#include <QObject>
#include <QAbstractItemModel>

#include "cure_tree_item.h"


class CureStructureModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit CureStructureModel(const QString &data, QObject *parent = 0);
    virtual ~CureStructureModel();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

private:
    void setupModelData(const QStringList &lines, CureTreeItem *parent);

    CureTreeItem *rootItem;
};

#endif // CURESTRUCTUREMODEL_H
