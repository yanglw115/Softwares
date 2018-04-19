#include "mainwindow.h"
#include "msvs_charset.h"

#include <QListWidgetItem>

static QString g_strTitle = "CureOffice V1.0";
static int LIST_ICON_SIZE_RATIO = 25;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle(g_strTitle);
    this->setMinimumSize(800, 600);

    initMainWindow();
}

MainWindow::~MainWindow()
{

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

    m_pStackedOffice = new QStackedWidget(this);

    m_pStackedOffice->addWidget(m_pWidgetSalary);
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

void MainWindow::resizeEvent(QResizeEvent *event)
{
    m_pListOffice->setIconSize(event->size() / LIST_ICON_SIZE_RATIO);
    m_pListOffice->setSpacing(this->height() / (LIST_ICON_SIZE_RATIO * 2));
    //m_pListOffice->setFixedWidth(this->width() / 10);
}
