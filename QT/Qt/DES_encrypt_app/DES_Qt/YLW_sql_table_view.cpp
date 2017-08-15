#include <QFile>
#include <QtDebug>
#include <QHeaderView>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlIndex>
#include <QDateTime>

#include "YLW_sql_table_view.h"

const QString g_strSqlDriverName                    = "QSQLITE";
const QString g_strConnectName                      = "HWAPP";
static const QString g_strADInfoTableName           = "advinfo";
static const QString g_strVersionTableName          = "version";

SQLOperateWidget::SQLOperateWidget(QWidget *parent)
    : QWidget(parent)
    , m_strSqlFilePath(QString(""))
    , m_strCurrentTable(QString(""))
    , m_pSqlTableModel(NULL)
    , m_bOpenDatabase(false)
{
    this->setAcceptDrops(true);

    m_pTableView = new MyTableView(this);
    this->setAcceptDrops(true);

    /* 数据库操作部分 */
    m_pLabelSqlFile = new QLabel(tr("数据库文件:"), this);

    m_pLineEditSqlFile = new FileLineEdit(this);
    connect(m_pLineEditSqlFile, SIGNAL(textChanged(QString)), this, SLOT(slotSetSqlDatabaseFile(QString)));
    /* 除了QLineEdit自身接受文件拖入以外，也可以将拖入QTableView的文件设置到QLineEdit */
    connect(m_pTableView, SIGNAL(sigFileIn(QString)), m_pLineEditSqlFile, SLOT(setText(QString)));

    m_pButtonSqlFileBrowse = new QPushButton(tr("浏览"), this);
    connect(m_pButtonSqlFileBrowse, SIGNAL(clicked(bool)), this, SLOT(slotBrowseSqlFile()));

    m_pLabelDataTable = new QLabel(tr("表:"), this);
    m_pComboBoxDataTable = new QComboBox(this);
    connect(m_pComboBoxDataTable, SIGNAL(activated(QString)), this, SLOT(slotSetCurrentTable(QString)));

    m_pButtonRefresh = new QPushButton(QIcon(":/refresh.png"), "", this);
    m_pButtonRefresh->setShortcut(tr("F5"));
//    m_pButtonRefresh->setStatusTip(tr("刷新表数据"));
    /* statusTip应该是在状态栏的提示信息，而toolTip才是在部件上面的提示信息 */
    m_pButtonRefresh->setToolTip(tr("刷新表数据"));
    m_pButtonRefresh->setEnabled(false);
    connect(m_pButtonRefresh, SIGNAL(clicked(bool)), this, SLOT(slotRereshCurrentTable()));

    m_pButtonSaveChanges = new QPushButton(QIcon(":/save.png"), "", this);
    m_pButtonSaveChanges->setShortcut(tr("Ctrl+s"));
    m_pButtonSaveChanges->setToolTip(tr("保存更改"));
    m_pButtonSaveChanges->setEnabled(false);
    connect(m_pButtonSaveChanges, SIGNAL(clicked(bool)), this, SLOT(slotSaveChanges()));

    m_pButtonNewRecord = new QPushButton(tr("新建记录"), this);
    m_pButtonNewRecord->setEnabled(false);
    connect(m_pButtonNewRecord, SIGNAL(clicked(bool)), this, SLOT(slotAddNewRowRecord()));

    m_pButtonDeleteRecord = new QPushButton(tr("删除记录"), this);
    m_pButtonDeleteRecord->setEnabled(false);
    connect(m_pButtonDeleteRecord, SIGNAL(clicked(bool)), this, SLOT(slotDeleteRowRecord()));


    /* 数据库操作部分 */
    QHBoxLayout *pHLayoutSqlFile = new QHBoxLayout;
    pHLayoutSqlFile->addWidget(m_pLabelSqlFile, 0);
    pHLayoutSqlFile->addWidget(m_pLineEditSqlFile, 2);
    pHLayoutSqlFile->addWidget(m_pButtonSqlFileBrowse, 1, Qt::AlignLeft);
    pHLayoutSqlFile->addStretch(1);

    QHBoxLayout *pHLayoutTable = new QHBoxLayout;
    pHLayoutTable->addWidget(m_pLabelDataTable, 0, Qt::AlignLeft);
    pHLayoutTable->addWidget(m_pComboBoxDataTable, 3);
    pHLayoutTable->addWidget(m_pButtonRefresh, 0);
    pHLayoutTable->addWidget(m_pButtonSaveChanges, 0);
    /* addStretch是按照比例进行添加空白部分，作用类似于Spacer; addSpacing则是按照像素大小进行添加，窗口大小变化时不好控制 */
    pHLayoutTable->addStretch(4);
    pHLayoutTable->addWidget(m_pButtonNewRecord, 0, Qt::AlignRight);
    pHLayoutTable->addWidget(m_pButtonDeleteRecord, 0, Qt::AlignRight);

    /* 该类的主Layout */
    m_pVLayoutSqlOper = new QVBoxLayout(this);
    m_pVLayoutSqlOper->addLayout(pHLayoutSqlFile);
    m_pVLayoutSqlOper->addLayout(pHLayoutTable);
    m_pVLayoutSqlOper->addWidget(m_pTableView);
}


SQLOperateWidget::~SQLOperateWidget()
{

}

void SQLOperateWidget::setSqlDatabaseFile(const QString &strFilePath)
{
    if (!m_strSqlFilePath.isEmpty()) {
        closeDatabase();
        /* 这里需要删除代理 */
        m_pSqlTableModel->deleteLater();
        m_pSqlTableModel = NULL;
        m_pButtonNewRecord->setEnabled(false);
        m_pButtonDeleteRecord->setEnabled(false);
    }
    m_strSqlFilePath = strFilePath;
    initDatabase();
    if (openDatabase()) {
        /* 只有成功打开了数据库，才能进行初始化的显示 */
        initTableViewShow();
    }
}

void SQLOperateWidget::slotSetCurrentTable(const QString &strTableName)
{
    if (strTableName.isEmpty()) {
        qWarning() << "当前所选择的表名为空!";
        return;
    }

    m_strCurrentTable = strTableName;
    qDebug() << "当前所选表名:" << m_strCurrentTable;
    if (m_pSqlTableModel) {
        m_pSqlTableModel->clear();
        m_pSqlTableModel->deleteLater();
        m_pSqlTableModel = NULL;
    }
    m_pSqlTableModel = new QSqlTableModel(this, m_database);
    m_pSqlTableModel->setTable(m_strCurrentTable);
    connect(m_pSqlTableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(slotSqlModelDataChanged()));

    /* 字段改变后同步到数据库的机制 */
    m_pSqlTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_pSqlTableModel->select();

    /* 有的数据库或平台不支持直接查询当前数据库的大小，则需要加上下面几句，否则不能通过model.rowCount()获取到行数 */
    while (m_pSqlTableModel->canFetchMore()) {
        m_pSqlTableModel->fetchMore();
    }

    m_pButtonRefresh->setEnabled(false);
    m_pButtonSaveChanges->setEnabled(false);
    m_pButtonNewRecord->setEnabled(true);
    setButtonDeleteEnable();

    m_pTableView->setModel(m_pSqlTableModel);

    m_pTableView->show();
}

void SQLOperateWidget::slotBrowseSqlFile()
{
#ifdef Q_OS_UNIX
    QString strFilePath = QFileDialog::getOpenFileName(this, tr("选择一个将计算其MD5值的文件"), "/", "file(*.*)");
#else
    QString strFilePath = QFileDialog::getOpenFileName(this, tr("选择一个将计算其MD5值的文件"), "Desktop", "file(*.*)");
#endif
    if (!strFilePath.isEmpty()) {
        qDebug() << "打开的文件为:" << strFilePath;
        m_pLineEditSqlFile->setText(strFilePath);
    }
}

void SQLOperateWidget::slotSetSqlDatabaseFile(const QString &strFilePath)
{
    if (QFile::exists(strFilePath)) {
        setSqlDatabaseFile(strFilePath);
    } else {
        qWarning() << "Sql database file is not exist:" << strFilePath;
    }
}

void SQLOperateWidget::slotRereshCurrentTable()
{
    qDebug() << "刷新当前表数据...";
    slotSetCurrentTable(m_strCurrentTable);
}

void SQLOperateWidget::slotSaveChanges()
{
    if (m_pSqlTableModel) {
        if (!m_pSqlTableModel->submitAll()) {
            qWarning() << "数据提交失败:" << m_pSqlTableModel->lastError();
        } else {
            m_pButtonSaveChanges->setEnabled(false);
            m_pButtonRefresh->setEnabled(false);
        }
    }
}

void SQLOperateWidget::slotAddNewRowRecord()
{
    if (m_pSqlTableModel) {
        int rowCount = m_pSqlTableModel->rowCount();
        m_pSqlTableModel->insertRow(rowCount);

        QSqlIndex sqlIndex = m_pSqlTableModel->primaryKey();
        if (!sqlIndex.isEmpty()) {
            QString strPrimaryKeyName = sqlIndex.fieldName(0);
            QString strData = m_pSqlTableModel->data(m_pSqlTableModel->index(rowCount - 1, 0)).toString();
            qDebug() << "Primary key name:" << strPrimaryKeyName;
            qDebug() << "Last primary key value:" << strData;

            m_pSqlTableModel->setData(m_pSqlTableModel->index(rowCount, 0), QString("%1").arg(QDateTime::currentMSecsSinceEpoch()));
        }
    }
}

void SQLOperateWidget::slotDeleteRowRecord()
{
    if (m_pSqlTableModel) {
        /* QTableView自带QItemSelectionModel,不需要再重新new一个，并且也可以共享给其他视图使用 */
        QModelIndexList selectedIndexes = m_pTableView->selectionModel()->selectedRows(0);
        foreach (const QModelIndex modelIndex, selectedIndexes) {
            qDebug() << "Current row about to deleted:" << modelIndex.row();
            m_pSqlTableModel->removeRow(modelIndex.row());
            /* 这里主动发射dataChanged信号，是因为删除行的时候，QT并没有发射此信号，但是增加记录和修改记录时都会发射 */
            emit m_pSqlTableModel->dataChanged(modelIndex, modelIndex);
        }
    }
}

void SQLOperateWidget::slotSqlModelDataChanged()
{
    m_pButtonSaveChanges->setEnabled(true);
    m_pButtonRefresh->setEnabled(true);
}

void SQLOperateWidget::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "SQLOperateWidget drag enter event...";
    if (event->mimeData()->hasUrls()) { //hasFormat("text/uri-list")) {
        event->acceptProposedAction();
    }
}

void SQLOperateWidget::dropEvent(QDropEvent *event)
{
    qDebug() << "SQLOperateWidget drop enter event...";
    if (event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
        QList<QUrl> listUrls = event->mimeData()->urls();
        QFileInfo *pFileInfo = new QFileInfo(listUrls.at(0).toLocalFile());
        m_pLineEditSqlFile->setText(pFileInfo->absoluteFilePath());
    }
}

void SQLOperateWidget::initDatabase()
{
    qDebug() << "初始化数据库:" << m_strSqlFilePath;

    m_database = QSqlDatabase::addDatabase(g_strSqlDriverName, g_strConnectName);
    m_database.setDatabaseName(m_strSqlFilePath);

    /* 检查数据库文件是否存在 */
    if (!QFile::exists(m_strSqlFilePath)) {
        qCritical() << "数据库文件不存在，无法读取数据！";
    }
}

bool SQLOperateWidget::openDatabase()
{
    /* 打开数据库 */
    if (!m_database.open()) {
        qCritical() << "数据库文件打开失败！";
        if (m_database.isValid()) {
            m_database.removeDatabase(g_strConnectName);
        }
        return false;
    }
    m_bOpenDatabase = true;
    qDebug() << "打开连接数据库成功！";
    return true;
}

void SQLOperateWidget::closeDatabase()
{
    qDebug() << "关闭数据库连接！";
    if (m_database.isOpen()) {
        m_database.close();
        /* 网上的方法，避免报connection is still in use的警告 */
        QString name;
        {
            name = QSqlDatabase::database().connectionName();
        } // 超出作用域，隐含对象QSqlDatabase::database()被删除。
        QSqlDatabase::removeDatabase(name);
    }
    m_bOpenDatabase = false;
}

void SQLOperateWidget::initTableViewShow()
{
    QSqlQuery query(m_database);
    QString strExec = QString("%1").arg("SELECT name FROM SQLITE_MASTER WHERE type='table';");
    qDebug() << "准备执行数据查询操作:" << strExec;
    if (query.exec(strExec)) {
        QSqlRecord record = query.record();
        if (record.count() > 0) {
            QStringList strList;
            strList.clear();
            while (query.next()) {
                strList.append(query.value(0).toString());
            }
            if (strList.isEmpty()) {
                qWarning() << "数据库为空!";
            } else {
                foreach (QString str, strList) {
                    qDebug() << str;
                }
                m_pComboBoxDataTable->clear();
                m_pComboBoxDataTable->addItems(strList);
                slotSetCurrentTable(strList[0]);
            }
        } else {
            qWarning() << "数据库表异常!";
        }
    } else {
        qCritical() << "查询数据库所有表名失败!";
    }
}

void SQLOperateWidget::setButtonDeleteEnable()
{
    qDebug() << "QSqlTableModel row count:" << m_pSqlTableModel->rowCount();
    if (m_pSqlTableModel && m_pSqlTableModel->rowCount() > 0) {
        m_pButtonDeleteRecord->setEnabled(true);
    } else {
        m_pButtonDeleteRecord->setEnabled(false);
    }
}
