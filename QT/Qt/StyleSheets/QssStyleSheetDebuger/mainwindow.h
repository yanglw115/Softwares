#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QStringListModel>
#include <QFileSystemWatcher>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void registerFileWatcher(const QString &strFilePath);
public slots:
    void slotWatchedFileChanged(const QString &strFilePath);
private slots:
    void slotChangeStacked(const QModelIndex &index);
private:
    Ui::MainWindow *ui;
    QFileSystemModel *m_pFileSystemModel;
    QStringListModel *m_pStringListModel;
    QFileSystemWatcher *m_pFileSystemWatcher;
};

#endif // MAINWINDOW_H
