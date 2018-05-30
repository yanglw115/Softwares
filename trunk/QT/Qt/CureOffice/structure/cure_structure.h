#ifndef CURESTRUCTURE_H
#define CURESTRUCTURE_H

#include <QObject>
#include <QWidget>
#include <QTreeView>
#include <QLayout>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "cure_structure_model.h"
#include "cure_tree_item.h"

class CureStructure : public QWidget
{
    Q_OBJECT
public:
    explicit CureStructure(QWidget *parent = 0);
    virtual ~CureStructure();
    static void checkStructureDBTable(QSqlDatabase &db);
signals:

public slots:
private:
    QTreeView *m_pTreeView;
    QVBoxLayout *m_pVLayoutMain;
};

#endif // CURESTRUCTURE_H
