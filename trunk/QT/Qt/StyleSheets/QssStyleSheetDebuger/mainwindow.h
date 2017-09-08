#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QStringListModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void slotChangeStacked(const QModelIndex &index);
private:
    Ui::MainWindow *ui;
    QFileSystemModel *m_pFileSystemModel;
    QStringListModel *m_pStringListModel;
};

#endif // MAINWINDOW_H
