#include <QtDebug>

#include "YLW_main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_pDrawWidget(NULL)
    , m_pToolBar(NULL)
    , m_pLabelStyle(NULL)
    , m_pLabelWidth(NULL)
    , m_pComboBoxStyle(NULL)
    , m_pSpinBoxWidth(NULL)
    , m_pButtonColor(NULL)
    , m_pButtonClear(NULL)
{
    m_pDrawWidget = new DrawWidget; /* 这里不能指定父窗口为this，否则不会显示 */
    m_pLabelStyle = new QLabel(tr("Style: "), this);
    m_pLabelWidth = new QLabel(tr("LineWidth: "), this);

    m_pComboBoxStyle = new QComboBox(this);
    /* 在画线的时候，如果画的比较慢，则基本上画出来的是SolidLine风格；应该是与mouseMoveEvent有关，该事件回调有一定的
     * 间隙，该间隙应该是毫秒级别的 */
    m_pComboBoxStyle->addItem(tr("SolidLine"), static_cast<int>(Qt::SolidLine));
    m_pComboBoxStyle->addItem(tr("DashLine"), static_cast<int>(Qt::DashLine));
    m_pComboBoxStyle->addItem(tr("DotLine"), static_cast<int>(Qt::DotLine));
    m_pComboBoxStyle->addItem(tr("DashDotLine"), static_cast<int>(Qt::DashDotLine));
    m_pComboBoxStyle->addItem(tr("DashDotDotLine"), static_cast<int>(Qt::DashDotDotLine));
    m_pComboBoxStyle->addItem(tr("CutomDashLine"), static_cast<int>(Qt::CustomDashLine));
    m_pComboBoxStyle->setCurrentIndex(0);
    connect(m_pComboBoxStyle, SIGNAL(activated(int)), this, SLOT(slotChangeStyle(int)));

    m_pSpinBoxWidth = new QSpinBox(this);
    m_pSpinBoxWidth->setValue(20);
    m_pSpinBoxWidth->setValue(1);
    connect(m_pSpinBoxWidth, SIGNAL(valueChanged(int)), this, SLOT(slotChangeWidth(int)));

    /** QToolButton与QAction有一定的关联。
     * 1、QToolButton一般用于显示图标，而一般的QButton则用于显示文本
     * 2、QAction在添加到QToolBar时候，会自然生成对应的QToolButton。所以如果不使用QToolButton，使用QAction也是一样的。
     */
    m_pButtonColor = new QToolButton(this);
    QPixmap pixmap(40, 20);
    pixmap.fill(Qt::black);
    m_pButtonColor->setIcon(QIcon(pixmap));
    connect(m_pButtonColor, SIGNAL(clicked()), this, SLOT(slotChangeColor()));

    m_pButtonClear = new QToolButton(this);
    m_pButtonClear->setText(tr("Clear"));
    connect(m_pButtonClear, SIGNAL(clicked()), this, SLOT(slotClearDraw()));

    /* QToolBar可以通过下面这种方式addToolBar()进行创建，也可以使用m_pToolBar = this.addToolBar()来进行 */
    m_pToolBar = new QToolBar(this);
    m_pToolBar->addWidget(m_pLabelStyle);
    m_pToolBar->addWidget(m_pComboBoxStyle);
    m_pToolBar->addSeparator();
    m_pToolBar->addWidget(m_pLabelWidth);
    m_pToolBar->addWidget(m_pSpinBoxWidth);
    m_pToolBar->addSeparator();
    m_pToolBar->addWidget(m_pButtonColor);
    m_pToolBar->addWidget(m_pButtonClear);

    this->addToolBar(m_pToolBar);

    this->setMinimumSize(600, 400);
    this->setCentralWidget(m_pDrawWidget);
}

MainWindow::~MainWindow()
{

}

void MainWindow::slotChangeColor()
{
    QColor color = QColorDialog::getColor(QColor(Qt::white), this);
    if (color.isValid()) {
        QPixmap pixmap(QSize(40, 20));
        pixmap.fill(color);
        m_pButtonColor->setIcon(QIcon(pixmap));
        m_pDrawWidget->setColor(color);
    }
}

void MainWindow::slotChangeWidth(int value)
{
    //int width = m_pSpinBoxWidth->value();
    m_pDrawWidget->setWidth(value);
}

void MainWindow::slotChangeStyle(int value)
{
    int style = m_pComboBoxStyle->itemData(value, Qt::UserRole).toInt();
    m_pDrawWidget->setStyle(style);
}

void MainWindow::slotClearDraw()
{
    m_pDrawWidget->clearDraw();
}
