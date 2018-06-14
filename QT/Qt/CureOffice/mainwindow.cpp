#include "mainwindow.h"
#include "msvs_charset.h"
#include "cure_global.h"
#include "cure_utils.h"

#include <QListWidgetItem>
#include <QThread>
#include <QMessageBox>

//QSqlDatabase g_db = QSqlDatabase::addDatabase("SQLITECIPHER");
QSqlDatabase g_db = QSqlDatabase::addDatabase("QSQLITE");

static QString g_strTitle = "CureOffice V1.0";
static int LIST_ICON_SIZE_RATIO = 25;

const QString g_strDataDir = "data";
const QString g_strDatabaseFile = "CureOffice.db";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle(g_strTitle);
    this->setMinimumSize(800, 600);

    initDatabase();
    initMainWindow();
    this->showMaximized();
}

MainWindow::~MainWindow()
{
    if (g_db.isOpen()) {
        g_db.close();
    }

    QThread::msleep(300);
    /* Avoid run warning "connection is still inuse" */
    QString name;
    {
        name = QSqlDatabase::database().connectionName();
    }
    /* QSqlDatabase::database() will be deleted */
    QSqlDatabase::removeDatabase(name);
}

void MainWindow::initMainWindow()
{
    m_pListOffice = new QListWidget(this);
    m_pListOffice->setIconSize(this->size() / LIST_ICON_SIZE_RATIO);
    m_pListOffice->setViewMode(QListView::ListMode);
    m_pListOffice->setSpacing(this->height() / (LIST_ICON_SIZE_RATIO * 2));
    //m_pListOffice->setMinimumWidth(140);
    m_pListOffice->setMaximumWidth(150);
    //m_pListOffice->setFixedWidth(this->width() / 10);
    //m_pListOffice->setResizeMode(QListView::Adjust);

    m_pWidgetSalary = new CureSalary(this);
    //m_pWidgetStructure = new CureStructure(this);

    m_pStackedOffice = new QStackedWidget(this);

    m_pStackedOffice->addWidget(m_pWidgetSalary);
    //m_pStackedOffice->addWidget(m_pWidgetStructure);
    m_pStackedOffice->addWidget(new QWidget(this));
    m_pStackedOffice->addWidget(new QWidget(this));
    m_pStackedOffice->addWidget(new QWidget(this));
    m_pStackedOffice->addWidget(new QWidget(this));
    m_pStackedOffice->addWidget(new QWidget(this));
    m_pStackedOffice->addWidget(new QWidget(this));
    m_pStackedOffice->addWidget(new QWidget(this));
    m_pStackedOffice->addWidget(new QWidget(this));

    connect(m_pListOffice, SIGNAL(currentRowChanged(int)), m_pStackedOffice, SLOT(setCurrentIndex(int)));
    m_pHLayoutOffice = new QHBoxLayout;
    m_pWidgetOffice = new QWidget;

    QListWidgetItem *pItemFirst = new QListWidgetItem(m_pListOffice);
    pItemFirst->setIcon(QIcon(":/images/FirstPage.png"));
    pItemFirst->setText(tr("首页"));
    pItemFirst->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QListWidgetItem *pItemStructure = new QListWidgetItem(m_pListOffice);
    pItemStructure->setIcon(QIcon(":/images/Structure.png"));
    pItemStructure->setText(tr("组织"));
    pItemStructure->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QListWidgetItem *pItemStaff = new QListWidgetItem(m_pListOffice);
    pItemStaff->setIcon(QIcon(":/images/Staff.png"));
    pItemStaff->setText(tr("员工"));
    pItemStaff->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QListWidgetItem *pItemSalary = new QListWidgetItem(m_pListOffice);
    pItemSalary->setIcon(QIcon(":/images/Salary.png"));
    pItemSalary->setText(tr("工资"));
    pItemSalary->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QListWidgetItem *pItemSocialSecurity = new QListWidgetItem(m_pListOffice);
    pItemSocialSecurity->setIcon(QIcon(":/images/SocialSecurity.png"));
    pItemSocialSecurity->setText(tr("社保"));
    pItemSocialSecurity->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QListWidgetItem *pItemAccumulationFund = new QListWidgetItem(m_pListOffice);
    pItemAccumulationFund->setIcon(QIcon(":/images/AccumulationFund.png"));
    pItemAccumulationFund->setText(tr("公积金"));
    pItemAccumulationFund->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QListWidgetItem *pItemClockingIn = new QListWidgetItem(m_pListOffice);
    pItemClockingIn->setIcon(QIcon(":/images/Clocking-in.png"));
    pItemClockingIn->setText(tr("考勤"));
    pItemClockingIn->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QListWidgetItem *pItemEmploy = new QListWidgetItem(m_pListOffice);
    pItemEmploy->setIcon(QIcon(":/images/Employ.png"));
    pItemEmploy->setText(tr("招聘"));
    pItemEmploy->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QListWidgetItem *pItemStatics = new QListWidgetItem(m_pListOffice);
    pItemStatics->setIcon(QIcon(":/images/Statics.png"));
    pItemStatics->setText(tr("统计"));
    pItemStatics->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);


    m_pHLayoutOffice->addWidget(m_pListOffice, 0);
    m_pHLayoutOffice->addWidget(m_pStackedOffice, 4);
    m_pWidgetOffice->setLayout(m_pHLayoutOffice);
    this->setCentralWidget(m_pWidgetOffice);
}

void MainWindow::initDatabase()
{
    /* 目前没有找到能够打开我们实现的加密的sqlite数据库,后续还需要自己实现一个. */
    QString strDBFilePath = CUtils::getFileFullPath(g_strDataDir + "/" + g_strDatabaseFile);
    /** FixMe: 这里需要增加判断，如果数据库文件不存在的话，则需要创建一个数据库文件，并且提示用户输入用户名与密码 **/
    g_db.setDatabaseName(strDBFilePath);
    bool bOK = false;
    if (QFile::exists(strDBFilePath)) {
        bOK = g_db.open("root", "admin");
    } else {
        g_db.setUserName("root");
        g_db.setPassword("admin");
//        g_db.setConnectOptions("QSQLITE_CREATE_KEY");
        bOK = g_db.open();
    }

    if (bOK) {
        qDebug() << "Open database success!";
        /** do something others. */
        CureStructure::checkStructureDBTable(g_db);
    } else {
        qWarning() << "Open database failed: " << g_db.lastError().text();
        QMessageBox::critical(this, tr("本地数据库操作"), tr("本地数据库打开失败,将影响后续所有数据的正常展示,建议先修复此问题!"));
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    m_pListOffice->setIconSize(event->size() / LIST_ICON_SIZE_RATIO);
    m_pListOffice->setSpacing(this->height() / (LIST_ICON_SIZE_RATIO * 2));
    //m_pListOffice->setFixedWidth(this->width() / 10);
}
