#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pFileSystemModel = new QFileSystemModel(this);
    m_pFileSystemModel->setRootPath("D:\\");
    m_pStringListModel = new QStringListModel(this);
    QStringList strList;
    strList << "Vim" << "Source Insight" << "Android Stdio" << "Eclips" << "QT";
    m_pStringListModel->setStringList(strList);
    ui->listView->setModel(m_pStringListModel);
    ui->treeView->setModel(m_pFileSystemModel);
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(QString("%1, %2").arg(i).arg(j)));
        }
    }
    QStringList strListColumn;
    for (int i = 0; i < 200; ++i) {
        strListColumn << QString("Column %1").arg(i);
    }
    ui->columnView->setModel(new QStringListModel(strListColumn, this));
    connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotChangeStacked(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotChangeStacked(const QModelIndex &index)
{
    ui->stackedWidget->setCurrentIndex(index.row());
}
