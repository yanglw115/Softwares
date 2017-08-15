#include "YLW_image_processor.h"
#include <QApplication>


/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 * 参考<<Qt5开发及实例>>5.2 Qt5文件操作功能
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImgProcessor w;
    w.show();

    return a.exec();
}
