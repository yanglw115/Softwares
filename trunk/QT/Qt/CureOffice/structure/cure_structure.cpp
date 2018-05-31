#include "cure_structure.h"
#include "msvs_charset.h"
#include "cure_global.h"

#include <QtDebug>

const QString g_strTableNameStructure = "structure";

CureStructure::CureStructure(QWidget *parent)
    : QWidget(parent)
    , m_pTreeView(NULL)
{
    m_pTreeView = new QTreeView(this);
    CureStructureModel *pModel = new CureStructureModel("", this);
    m_pTreeView->setModel(pModel);
    m_pVLayoutMain = new QVBoxLayout;
    m_pVLayoutMain->addWidget(m_pTreeView);
    this->setLayout(m_pVLayoutMain);
}

CureStructure::~CureStructure()
{

}

void CureStructure::checkStructureDBTable(QSqlDatabase &db)
{
    /* Must open database before outside */
    if (db.isOpen()) {
        QStringList listTable = db.tables();
        if (listTable.contains(g_strTableNameStructure)) {
            qDebug() << "Database has structure table.";
        } else {
            qDebug() << "Database does not have structure table, create it!";
            QSqlQuery query(db);
            if (db.driver()->hasFeature(QSqlDriver::Transactions)) {
                /* 这里通过事务处理，添加数据 */
                if (db.transaction()) {
                    bool bOK = true;
                    QString strExec = QString("CREATE TABLE %1(grade int, name varchar(255), parentName varchar(255), "
                                              "leader varchar(255), job varchar(255), numbers int)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);

                    /* 第一级 */
                    strExec = QString("INSERT INTO %1(grade, name, parentName, leader, "
                                      "job, numbers) VALUES(1, '总经办', '', '李亚秋', '总经理', 5)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);
                    strExec = QString("INSERT INTO %1(grade, name, parentName, leader, "
                                      "job, numbers) VALUES(1, '营销中心', '', '李松', '总监', 13)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);
                    strExec = QString("INSERT INTO %1(grade, name, parentName, "
                                      "leader, job, numbers) VALUES(1, '产品研发中心', '', '彭鹏', '研发总监', 35)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);
                    strExec = QString("INSERT INTO %1(grade, name, parentName, "
                                      "leader, job, numbers) VALUES(1, '财务部', '', '王小芹', '财务部长', 4)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);
                    strExec = QString("INSERT INTO %1(grade, name, parentName, "
                                      "leader, job, numbers) VALUES(1, '综合管理中心', '', '何维', '总经理助理', 6)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);

                    /* 第二级 */
                    strExec = QString("INSERT INTO %1(grade, name, parentName, "
                                      "leader, job, numbers) VALUES(2, '商务拓展部', '营销中心', '刘庆威', '', 1)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);
                    strExec = QString("INSERT INTO %1(grade, name, parentName, "
                                      "leader, job, numbers) VALUES(2, '市场拓展部', '营销中心', '叶影飞', '主管', 3)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);
                    strExec = QString("INSERT INTO %1(grade, name, parentName, "
                                      "leader, job, numbers) VALUES(2, '安装售后部', '营销中心', '', '', 2)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);
                    strExec = QString("INSERT INTO %1(grade, name, parentName, "
                                      "leader, job, numbers) VALUES(2, '营销支持部', '营销中心', '', '', 3)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);

                    /* UDC */
                    strExec = QString("INSERT INTO %1(grade, name, parentName, "
                                      "leader, job, numbers) VALUES(2, 'UDC', '产品研发中心', '石涌', '总监', 5)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);
                    strExec = QString("INSERT INTO %1(grade, name, parentName, "
                                      "leader, job, numbers) VALUES(3, 'UI组', 'UDC', '', '', 2)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);
                    strExec = QString("INSERT INTO %1(grade, name, parentName, "
                                      "leader, job, numbers) VALUES(3, '平面组', 'UDC', '', '', 2)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);

                    /* 产品部 */
                    strExec = QString("INSERT INTO %1(grade, name, parentName, "
                                      "leader, job, numbers) VALUES(2, '产品部', '产品研发中心', '', '', 2)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);

                    /* 软件研发部 */
                    strExec = QString("INSERT INTO %1(grade, name, parentName, "
                                      "leader, job, numbers) VALUES(2, '软件研发部', '产品研发中心', '朱冲', '部长', 12)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);
                    strExec = QString("INSERT INTO %1(grade, name, parentName, "
                                      "leader, job, numbers) VALUES(3, '前端', '软件研发部', '施兰兰', '组长', 2)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);
                    strExec = QString("INSERT INTO %1(grade, name, parentName, "
                                      "leader, job, numbers) VALUES(3, 'JAVA组', '软件研发部', '邓强', '组长', 2)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);
                    strExec = QString("INSERT INTO %1(grade, name, parentName, "
                                      "leader, job, numbers) VALUES(3, '测试组', '软件研发部', '刘永程', '组长', 2)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);
                    strExec = QString("INSERT INTO %1(grade, name, parentName, "
                                      "leader, job, numbers) VALUES(3, 'Android组', '软件研发部', '淘军', '组长', 2)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);
                    strExec = QString("INSERT INTO %1(grade, name, parentName, "
                                      "leader, job, numbers) VALUES(3, 'C++组', '软件研发部', '杨利伟', '组长', 2)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);
                    strExec = QString("INSERT INTO %1(grade, name, parentName, "
                                      "leader, job, numbers) VALUES(3, 'IOS组', '软件研发部', '陈竹', '组长', 1)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);


                    /* 运营策划部 */
                    strExec = QString("INSERT INTO %1(grade, name, parentName, "
                                      "leader, job, numbers) VALUES(2, '运营策划部', '产品研发中心', '刘庆威', '部长', 3)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);
                    strExec = QString("INSERT INTO %1(grade, name, parentName, "
                                      "leader, job, numbers) VALUES(2, '硬件研发部', '产品研发中心', '王希', '部长', 4)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);
                    strExec = QString("INSERT INTO %1(grade, name, parentName, "
                                      "leader, job, numbers) VALUES(2, '深圳研发部', '产品研发中心', '石萍', '生产副总监&产品经理', 8)").arg(g_strTableNameStructure);
                    bOK &= query.exec(strExec);

                    if (bOK && db.commit()) {
                        qDebug() << "Add structure data to table success!";
                        return;
                    }
                    qWarning() << "Query error: " << db.lastError().text();
                } else {
                    qWarning() << "Driver not support transactions!";
                }
            }
        }
    } else {
        qWarning() << "Cannot check structure table, database is not open!";
    }
}

