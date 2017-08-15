#include <QSplitter>
#include <QTextEdit>
#include <QDockWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>

#include "mainwindow.h"

#define WITH_SPLITTER 1
#define WITH_DOCK 0
#define WITH_STACK 0

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
#if WITH_SPLITTER
    createSplitter();
#endif

#if WITH_DOCK
    setWindowTitle(tr("Dock window"));
    createDockWidget();
#endif

#if WITH_STACK
    createStackWidget();
#endif

}

MainWindow::~MainWindow()
{

}

void MainWindow::createSplitter()
{
#if WITH_SPLITTER
    /* 创建分割窗口 */
    /* 这里定义了主splitter之后，并没有设置其父窗口，因为设置的话，还需要设置其大小之类。这里把splitter当作独立窗口来展示 */
    /* 构造函数中第一个传入的参数代表是水平分割还是垂直分割 */
    QWidget *widget = new QWidget(this);
    this->setCentralWidget(widget);
    QSplitter *splitterMain = new QSplitter(Qt::Horizontal, widget);
    //QTextEdit *leftTextEdit = new QTextEdit(tr("Left widget"), splitterMain);
    /* 这里使用了两种方式，向splitter中添加元素，一种是在构造函数中明示splitterMain为父窗口；另一种是通过addWidget的方式 */
    QTextEdit *leftTextEdit = new QTextEdit(tr("Left widget"));
    splitterMain->addWidget(leftTextEdit);
    /* 设置分割窗口大小变化时分割线是否透明，这个只有看了效果才知道具体是什么样子的 */
    splitterMain->setOpaqueResize(false);

    QSplitter *splitterRight = new QSplitter(Qt::Vertical, splitterMain);
    QTextEdit *righTextEdit = new QTextEdit(tr("Top widget"), splitterRight);
    QTextEdit *downTextEdit = new QTextEdit(tr("Down widget"), splitterRight);
    //QTextEdit *thirdTextEdit = new QTextEdit(tr("third widget"));
    //splitterMain->addWidget(thirdTextEdit);
    QPushButton *pushButton = new QPushButton(tr("hide"), splitterMain);
    /* splitter中的元素可以隐藏，下面是一个例子 */
    connect(pushButton, SIGNAL(clicked()), leftTextEdit, SLOT(hide()));

    /* stretchFactor用于整个窗口的大小被拉伸改变时，splitter中的部分窗口所占的比例。如果splitter中有两个widget，则拉伸时，
        第二个窗口改变，第一个窗口不改变（当然有时也会根据min和max进行变化）；如果有3个及以上widget，则可以对每个进行设置，
        第二个参数就代表了所占的比重(所有第二参数值和所占比重)。*/
    /* 例如下面就是将三部分的拉伸比重设置为一致，即等比例 */
    splitterMain->setStretchFactor(0, 1);
    splitterMain->setStretchFactor(1, 1);
    splitterMain->setStretchFactor(2, 1);

#endif // WITH_SPLITTER
}

void MainWindow::createDockWidget()
{
#if WITH_DOCK
    QTextEdit *myEdit = new QTextEdit(tr("Main window"), this);
    myEdit->setAlignment(Qt::AlignCenter);
    setCentralWidget(myEdit);

    QDockWidget *dock1 = new QDockWidget(tr("DockWindow1"), this);
    /* 使用QDockWidget，需要设置最基本的下面两项 */
    dock1->setFeatures(QDockWidget::DockWidgetMovable);
    /* 停靠窗口只能放在主窗口的左边或右边 */
    dock1->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QTextEdit *myEdit1 = new QTextEdit("Dock Text widget 1, just can move.");
    /* 这里只有setWidget，没有addWidget，说明如果想用复杂的widget，则需要自己实现相关类 */
    dock1->setWidget(myEdit1);
    /* addDockWidget只有在QMainWindow类中才有，所以需要继承该类 */
    addDockWidget(Qt::RightDockWidgetArea, dock1);

    QDockWidget *dock2 = new QDockWidget(tr("DockWindow2"), this);
    /* 停靠窗口可以关闭，也可以通过浮动移动到主窗口的上下左右位置 */
    dock2->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
    QTextEdit *myEdit2 = new QTextEdit("Dock Text widget 2, can close and float.");
    dock2->setWidget(myEdit2);
    addDockWidget(Qt::RightDockWidgetArea, dock2);

    QDockWidget *dock3 = new QDockWidget(tr("DockWindow3"), this);
    dock3->setFeatures(QDockWidget::AllDockWidgetFeatures);
    QTextEdit *myEdit3 = new QTextEdit("Dock text Widget 3, can do all it can.");
    dock3->setWidget(myEdit3);
    addDockWidget(Qt::RightDockWidgetArea, dock3);

#endif // WITH_DOCK
}

void MainWindow::createStackWidget()
{
#if WITH_STACK
    //QHBoxLayout *layout = new QHBoxLayout(this);
    /**
     * 1.注意：QMainWindow默认有一个布局，只能通过setCentralWidget的方式去设置Central Widget的布局，这与QMainwindow的定义有关，详细
     * 也可以参考QMainWindow类的描述。但是QDialog等是可以的，因为他们不包含Menu bar, Tool bar等
     * 2.这里如果不用setCentralWidget，则整个布局展示时会异常；
     */
    QWidget *widget = new QWidget(this);
    this->setCentralWidget(widget);
    QHBoxLayout *layout = new QHBoxLayout;
    widget->setLayout(layout);

    /* 分别定义ListWidget和stackedWidget */
    QStackedWidget *stackedWidget = new QStackedWidget(widget);
    QListWidget *listWidget = new QListWidget(widget);

    listWidget->insertItem(0, "Window1");
    listWidget->insertItem(1, "Window2");
    listWidget->insertItem(2, "Window3");
    listWidget->setSpacing(3);

    stackedWidget->addWidget(new QLabel(tr("Label1")));
    stackedWidget->addWidget(new QLabel(tr("Label2")));
    stackedWidget->addWidget(new QLabel(tr("Label3")));

    layout->addWidget(listWidget);
    layout->addWidget(stackedWidget, 0, Qt::AlignHCenter);
    layout->setSpacing(5);
    layout->setMargin(5);
    /* 这里可以用setStretchFactor,也可使用setStretch */
    layout->setStretchFactor(listWidget, 1);
    layout->setStretchFactor(stackedWidget, 3);

    connect(listWidget, SIGNAL(currentRowChanged(int)), stackedWidget, SLOT(setCurrentIndex(int)));

#endif // WITH_STACK
}
