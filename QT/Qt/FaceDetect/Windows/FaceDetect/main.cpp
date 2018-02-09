
#include <QApplication>
#include <QFile>
#include <QtDebug>

#include "mainwindow.h"
#include "CFaceDetecter.h"
#include "CResultDetail.h"
#include "vs_charset.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if 0
    QFile file(":/qss/darkOrange.qss");
    //QFile file(":/qss/material-blue.qss");
    //QFile file(":/qss/Style_Gray.qss");
    //QFile file(":/qss/QTDark.qss");
    //QFile file(":/darkstyle/darkstyle.qss");
    if (file.open(QFile::ReadOnly)) {
        QString strStyleSheet = QLatin1String(file.readAll());
        a.setStyleSheet(strStyleSheet);
        file.close();
    } else {
        qWarning() << "Open qss file failed!";
    }
#endif

#if 1
    MainWindow w;
    QString strQssFile = "../FaceDetect/resources/styles/myStyle.qss";
#if 1
    //QString strQssFile = "../FaceDetect/resources/styles/QUIStyles/qss/bf.css";
    //QString strQssFile = "../FaceDetect/resources/styles/QUIStyles/qss/black.css";
    //QString strQssFile = "../FaceDetect/resources/styles/QUIStyles/qss/blue.css";
    //QString strQssFile = "../FaceDetect/resources/styles/QUIStyles/qss/darkblack.css";
    //QString strQssFile = "../FaceDetect/resources/styles/QUIStyles/qss/darkgray.css";
    //QString strQssFile = "../FaceDetect/resources/styles/QUIStyles/qss/flatblack.css";
    //QString strQssFile = "../FaceDetect/resources/styles/QUIStyles/qss/flatwhite.css";
    //QString strQssFile = "../FaceDetect/resources/styles/QUIStyles/qss/gray.css";
    //QString strQssFile = "../FaceDetect/resources/styles/QUIStyles/qss/lightblack.css";
    //QString strQssFile = "../FaceDetect/resources/styles/QUIStyles/qss/lightblue.css";
    //QString strQssFile = "../FaceDetect/resources/styles/QUIStyles/qss/psblack.css";
    //QString strQssFile = "../FaceDetect/resources/styles/QUIStyles/qss/silvery.css";
    //QString strQssFile = "../FaceDetect/resources/styles/QUIStyles/qss/test.css";
#else
    //QString strQssFile = ":/qss/bf.css";
    //QString strQssFile = ":/qss/black.css";
    //QString strQssFile = ":/qss/blue.css";
    //QString strQssFile = ":/qss/darkblack.css";
    //QString strQssFile = ":/qss/darkgray.css";
    //QString strQssFile = ":/qss/flatblack.css";
    //QString strQssFile = ":/qss/flatwhite.css";
    //QString strQssFile = ":/qss/gray.css";
    //QString strQssFile = ":/qss/lightblack.css";
    //QString strQssFile = ":/qss/lightblue.css";
    //QString strQssFile = ":/qss/psblack.css";
    //QString strQssFile = ":/qss/silvery.css";
    //QString strQssFile = ":/qss/test.css";
#endif
    w.registerFileWatcher(strQssFile);
    w.slotWatchedFileChanged(strQssFile);
    w.show();
#endif

#if 0
#include "quiwidget.h"
    QUIWidget::setCode();
    QUIWidget::setTranslator(":/image/qt_zh_CN.qm");
    QUIWidget::setTranslator(":/image/widgets.qm");
    MainWindow *pMainWindow = new MainWindow;

    QUIWidget qui;
    //设置主窗体
    qui.setMainWidget(pMainWindow);
    QObject::connect(&qui, SIGNAL(changeStyle(QString)), &a, SLOT(setStyleSheet(QString)));

    //设置标题
    qui.setTitle("iFace tools");

    //设置标题文本居中
    qui.setAlignment(Qt::AlignCenter);

    //设置窗体可拖动大小
    qui.setSizeGripEnabled(true);

    //设置换肤下拉菜单可见
    qui.setVisible(QUIWidget::BtnMenu, true);

    //设置标题栏高度
    //qui.setTitleHeight(50);

    //设置按钮宽度
    //qui.setBtnWidth(50);

    //设置左上角图标-图形字体
    //qui.setIconMain(QChar(0xf099), 11);

    //设置左上角图标-图片文件
    //qui.setPixmap(QUIWidget::Lab_Ico, ":/main.ico");

    qui.show();
#endif

    return a.exec();
}
