#include "dialog.h"
#include "ccontent.h"
#include <QApplication>
#include <QListWidget>
#include <QSplitter>

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 * 例子来源于《Qt开发与实例》3.5综合例子
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Dialog w;
//    w.show();
//    CBasicInfo basicInfo;
//    basicInfo.show();

//    CConnectiions connection;
//    connection.show();
//    CDetailInfo detailInfo;
//    detailInfo.show();

//    CContent content;
//    content.show();
    /* 分割窗口，用于将QListWidget和内容分开 */
    QSplitter *pSplitter = new QSplitter;

    QListWidget *pListWidget = new QListWidget;
    CContent *pContent = new CContent;
    /* 通过addWidget的方式得到分割窗口多个部分的内容。这里使用默认的水平方向分割(通过分割器的构造函数) */
    pSplitter->addWidget(pListWidget);
    pSplitter->addWidget(pContent);

    pListWidget->addItem(QObject::tr("基本信息"));
    pListWidget->addItem(QObject::tr("联系方式"));
    pListWidget->addItem(QObject::tr("详细资料"));
    pListWidget->setCurrentRow(0);

    QObject::connect(pListWidget, SIGNAL(currentRowChanged(int)), pContent, SLOT(slotSetCurrentStackWidget(int)));

    pSplitter->show();


    return a.exec();
}
