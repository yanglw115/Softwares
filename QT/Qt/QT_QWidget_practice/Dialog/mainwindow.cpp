#include <iostream>
#include <QColorDialog>
#include <QFileDialog>
#include <QFontDialog>
#include <QPalette>
#include <QtDebug>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
    , m_pButtonFileOpen(NULL)
    , m_pLineFileOpen(NULL)
    , m_pButtonColor(NULL)
    , m_pFrameColor(NULL)
    , m_pButtonFont(NULL)
    , m_pLineFontOpen(NULL)
    , m_pButtonInput(NULL)
    , m_pInputDlg(NULL)
    , m_pButtonMsgBox(NULL)
    , m_pMsgBoxDlg(NULL)
    , m_pButtonProgress(NULL)
    , m_pProgressDlg(NULL)
    , m_pButtonPalette(NULL)
    , m_pPaletteDilg(NULL)
    , m_pGridLaout(NULL)
{
    /* 设置标题 */
    setWindowTitle(tr("各种标准对话框的实例"));

    m_pButtonFileOpen = new QPushButton(tr("文件标准对话框实例"), this);
    m_pLineFileOpen = new QLineEdit(this);
    //m_pLineFileOpen->setEnabled(false);
    connect(m_pButtonFileOpen, SIGNAL(clicked()), this, SLOT(slotSetFileName()));

    m_pButtonColor = new QPushButton(tr("颜色标准对话框实例"), this);
    m_pFrameColor = new QFrame(this);
    /* QFrame是带框架的基本窗口，可以看作是带框架的最基本的元素，很多控件都继承于他，例如QLabel等 */
    //m_pFrameColor->setFrameShape(QFrame::Box);
    /* 设置QFrame的外形 */
    m_pFrameColor->setFrameShape(QFrame::WinPanel);
    /* 设置QFrame的阴影 */
    m_pFrameColor->setFrameShadow(QFrame::Raised);
    /* 设置自动更新QFrame的背景颜色 */
    m_pFrameColor->setAutoFillBackground(true);
    connect(m_pButtonColor, SIGNAL(clicked()), this, SLOT(slotSetColor()));

    m_pButtonFont = new QPushButton(tr("字体标准对话框实例"), this);
    m_pLineFontOpen = new QLineEdit(this);
    m_pLineFontOpen->setText(tr("您好, 字体!"));
    connect(m_pButtonFont, SIGNAL(clicked()), this, SLOT(slotSetFont()));

    /* 输入标准对话框通过单独的一个类来实现 */
    m_pButtonInput = new QPushButton(tr("输入标准对话框实例"), this);
    connect(m_pButtonInput, SIGNAL(clicked()), this, SLOT(slotOpenInputDlg()));

    /* 消息对话框 */
    m_pButtonMsgBox = new QPushButton(tr("消息标准对话框实例"), this);
    connect(m_pButtonMsgBox, SIGNAL(clicked()), this, SLOT(slotShowMsgBoxDlg()));

    /* 进度条 */
    m_pButtonProgress = new QPushButton(tr("进度条的展示"), this);
    connect(m_pButtonProgress, SIGNAL(clicked()), this, SLOT(slotShowProgressBar()));

    /* Palette */
    m_pButtonPalette = new QPushButton(tr("Palette"), this);
    connect(m_pButtonPalette, SIGNAL(clicked()), this, SLOT(slotShowPalette()));

    m_pGridLaout = new QGridLayout;
    m_pGridLaout->addWidget(m_pButtonFileOpen, 0, 0);
    m_pGridLaout->addWidget(m_pLineFileOpen, 0, 1);
    m_pGridLaout->addWidget(m_pButtonColor, 1, 0);
    m_pGridLaout->addWidget(m_pFrameColor, 1, 1);
    m_pGridLaout->addWidget(m_pButtonFont, 2, 0);
    m_pGridLaout->addWidget(m_pLineFontOpen, 2, 1);
    m_pGridLaout->addWidget(m_pButtonInput, 3, 0);
    m_pGridLaout->addWidget(m_pButtonMsgBox, 3, 1);
    m_pGridLaout->addWidget(m_pButtonProgress, 4, 0);
    m_pGridLaout->addWidget(m_pButtonPalette, 4, 1);

    /* 注意，这里在设置stretch的时候，需要设置对应列或行的 */
    m_pGridLaout->setColumnStretch(0, 1);
    m_pGridLaout->setColumnStretch(1, 2);


    this->setLayout(m_pGridLaout);
}

MainWindow::~MainWindow()
{

}

void MainWindow::slotSetFileName()
{
    /* 注意：在getOpenFileName静态函数中，第4个参数是用于过滤的，如果有多个过滤选项，则中间用;;分开，同一时间只能选择一个过滤选项。另外，过滤的内容
       通过()中的内容进行，如果有多个过滤，则中间用空格分开 */
    //QString strFileName = QFileDialog::getOpenFileName(this, "Open file dialog", "/", "C++ files(*.cpp);;C files(*.c *.h)");
    QString strFileName = QFileDialog::getOpenFileName(this, "Open file dialog", "/", "C&C++ files(*.cpp *.c *.h)");
    if (strFileName.isEmpty() || strFileName.isNull()) {
        std::cout << "Not select file!" << std::endl;
    } else {
        m_pLineFileOpen->setText(strFileName);
    }
}

void MainWindow::slotSetColor()
{
    QColor color = QColorDialog::getColor(Qt::blue, 0, "Select color dialog");
    /* 根据isValid()成员函数来判断是否选择了有效的颜色 */
    if (color.isValid()) {
        m_pFrameColor->setPalette(QPalette(color));
    }
}

void MainWindow::slotSetFont()
{
    bool bOK = false;
    QFont font = QFontDialog::getFont(&bOK, 0);
    /* 根据传入的bOK来判断是否获取成功 */
    if (bOK) {
        m_pLineFontOpen->setFont(font);
    }
}

void MainWindow::slotOpenInputDlg()
{
    if (NULL == m_pInputDlg) {
        qDebug() << "New a InputDlg.";
        /* 如果是在堆中创建的InputDlg,则为了在关闭对话框时自动销毁，则需要设置WA_DeleteOnClose属性 */
        m_pInputDlg = new InputDlg(this);
        m_pInputDlg->setAttribute(Qt::WA_DeleteOnClose);

        /* 这里在关闭释放内存之后，通过信号槽机制将m_pInputDlg设置为NULL */
        connect(m_pInputDlg, SIGNAL(finished(int)), this, SLOT(slotSetInputPointer()));
        /* 这里调用exec使对话框成为模态对话框 */
        m_pInputDlg->exec();

        /* 如果调用的不是exec,而是show，则非模态对话框 */
        //m_pInputDlg->show();
    } else {
        m_pInputDlg->show();
    }
}

void MainWindow::slotSetInputPointer()
{
    m_pInputDlg = NULL;
}

void MainWindow::slotShowMsgBoxDlg()
{
    if (m_pMsgBoxDlg != NULL) {
        delete m_pMsgBoxDlg;
    }
    m_pMsgBoxDlg = new MsgBoxDlg;
    m_pMsgBoxDlg->show();
}

void MainWindow::slotShowProgressBar()
{
    if (m_pProgressDlg != NULL) {
        delete m_pProgressDlg;
    }
    m_pProgressDlg = new ProgressDlg;
    m_pProgressDlg->exec();
}

void MainWindow::slotShowPalette()
{
    if (m_pPaletteDilg != NULL) {
        delete m_pPaletteDilg;
    }
    m_pPaletteDilg = new Pallete;
    m_pPaletteDilg->exec();
}
