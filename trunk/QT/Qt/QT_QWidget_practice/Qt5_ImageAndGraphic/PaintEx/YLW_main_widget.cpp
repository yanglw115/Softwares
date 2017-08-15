#include <QColorDialog>

#include "YLW_main_widget.h"

#ifndef _HW_CHARSET_H_
#define _HW_CHARSET_H_

/* 对于VS2013,QLabel中有时会有乱码出现，则这时需要添加以下宏处理(有时可能还需要将文件编码格式改为utf-8+bom)；
 * 但是对于MinGW则不会出现乱码 */
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#endif // _HW_CHARSET_H_

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , m_pPaintArea(NULL)
    , m_pLabelShap(NULL)
    , m_pLabelPenColor(NULL)
    , m_pLabelPenWidth(NULL)
    , m_pLabelPenStyle(NULL)
    , m_pLabelPenCap(NULL)
    , m_pLabelPenJoin(NULL)
    , m_pLabelFillRule(NULL)
    , m_pLabelSpreadStyle(NULL)
    , m_pLabelBrushColor(NULL)
    , m_pLabelBrushStyle(NULL)
    , m_pComboBoxShap(NULL)
    , m_pFramePenColor(NULL)
    , m_pButtonPenColor(NULL)
    , m_pSpinBoxPenWidth(NULL)
    , m_pComboBoxPenStyle(NULL)
    , m_pComboBoxPenCap(NULL)
    , m_pComboBoxPenJoin(NULL)
    , m_pComboBoxFillRule(NULL)
    , m_pComboBoxSpreadStyle(NULL)
    , m_pFrameBrushColor(NULL)
    , m_pButtonBrushColor(NULL)
    , m_pComboBoxBrushStyle(NULL)
    , m_pHLayout(NULL)
    , m_pGridLayout(NULL)
{
    m_pLabelShap = new QLabel(tr("形状:"), this);
    m_pLabelPenColor = new QLabel(tr("画笔颜色:"), this);
    m_pLabelPenWidth = new QLabel(tr("画笔线宽:"), this);
    m_pLabelPenStyle = new QLabel(tr("画笔风格:"), this);
    m_pLabelPenCap = new QLabel(tr("画笔顶帽:"), this);
    m_pLabelPenJoin = new QLabel(tr("画笔连接点:"), this);
    m_pLabelFillRule = new QLabel(tr("填充模式:"), this);
    m_pLabelSpreadStyle = new QLabel(tr("铺展效果:"), this);
    m_pLabelBrushColor = new QLabel(tr("画刷颜色:"), this);
    m_pLabelBrushStyle = new QLabel(tr("画刷风格:"), this);

    m_pComboBoxShap = new QComboBox(this);
    m_pComboBoxShap->addItem(tr("Line"), PaintArea::Line);
    m_pComboBoxShap->addItem(tr("Rectangle"), PaintArea::Rectangle);
    m_pComboBoxShap->addItem(tr("RoundRect"), PaintArea::RoundRect);
    m_pComboBoxShap->addItem(tr("Ellipse"), PaintArea::Ellipse);
    m_pComboBoxShap->addItem(tr("Polygon"), PaintArea::Polygon);
    m_pComboBoxShap->addItem(tr("PolyLine"), PaintArea::PolyLine);
    m_pComboBoxShap->addItem(tr("Points"), PaintArea::Points);
    m_pComboBoxShap->addItem(tr("Arc"), PaintArea::Arc);
    m_pComboBoxShap->addItem(tr("Path"), PaintArea::Path);
    m_pComboBoxShap->addItem(tr("Text"), PaintArea::Text);
    m_pComboBoxShap->addItem(tr("Pixmap"), PaintArea::Pixmap);
    connect(m_pComboBoxShap, SIGNAL(activated(int)), this, SLOT(slotChangeShap(int)));

    m_pFramePenColor = new QFrame(this);
    m_pFramePenColor->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_pFramePenColor->setAutoFillBackground(true);
    m_pFramePenColor->setPalette(QPalette(Qt::red)); /* QPalette::Window role */

    m_pButtonPenColor = new QPushButton(tr("更改"), this);
    connect(m_pButtonPenColor, SIGNAL(clicked()), this, SLOT(slotChangePenColor()));

    m_pSpinBoxPenWidth = new QSpinBox(this);
    m_pSpinBoxPenWidth->setRange(0, 20);
    connect(m_pSpinBoxPenWidth, SIGNAL(valueChanged(int)), this, SLOT(slotChangePenWidth(int)));

    m_pComboBoxPenStyle = new QComboBox(this);
    /** static_cast<int>(expression): 将expression转换为int类型.这里如果不转换则会
     * 报错：error C2248: “QVariant::QVariant”: 无法访问 private 成员(在“QVariant”类中声明)，至于有的不转换则不会报错，在网上
     * 暂时没有查到满意的答案 */
    m_pComboBoxPenStyle->addItem(tr("SolidLine"), static_cast<int>(Qt::SolidLine)); /* 实线 */
    m_pComboBoxPenStyle->addItem(tr("DashLine"), static_cast<int>(Qt::DashLine)); /* 虚线 */
    m_pComboBoxPenStyle->addItem(tr("DotLine"), static_cast<int>(Qt::DotLine)); /* 点线 */
    m_pComboBoxPenStyle->addItem(tr("DashDotLine"), static_cast<int>(Qt::DashDotLine)); /* -.-.-.-. */
    m_pComboBoxPenStyle->addItem(tr("DashDotDotLine"), static_cast<int>(Qt::DashDotDotLine)); /* -..-..-.. */
    m_pComboBoxPenStyle->addItem(tr("CustomDashLine"), static_cast<int>(Qt::CustomDashLine)); /* */
    connect(m_pComboBoxPenStyle, SIGNAL(activated(int)), this, SLOT(slotChangePenStyle(int)));

    m_pComboBoxPenCap = new QComboBox(this);
    m_pComboBoxPenCap->addItem(tr("SquareCap"), Qt::SquareCap); /* 画笔两头是方形，正方形效果 */
    m_pComboBoxPenCap->addItem(tr("FlatCap"), Qt::FlatCap); /* 画笔两头是方形，并且会延长半个线宽，有点像平行四边形的效果 */
    m_pComboBoxPenCap->addItem(tr("RoundCap"), Qt::RoundCap); /* 画笔两头是圆滑的 */
    connect(m_pComboBoxPenCap, SIGNAL(activated(int)), this, SLOT(slotChangePenCap(int)));

    m_pComboBoxPenJoin = new QComboBox(this);
    m_pComboBoxPenJoin->addItem(tr("BevelJoin"), Qt::BevelJoin); /* 根据定义图形看，很直观 */
    m_pComboBoxPenJoin->addItem(tr("MiterJoin"), Qt::MiterJoin);
    m_pComboBoxPenJoin->addItem(tr("RoundJoin"), Qt::RoundJoin);
    connect(m_pComboBoxPenJoin, SIGNAL(activated(int)), this, SLOT(slotChangePenJoin(int)));

    m_pComboBoxFillRule = new QComboBox(this);
    /* 指定以哪种方式决定填充路径和多边形。判断点是否在区域内的方法 */
    m_pComboBoxFillRule->addItem(tr("OddEvenFill"), Qt::OddEvenFill); /* 水平线与区域有奇数个焦点则在内部 */
    m_pComboBoxFillRule->addItem(tr("WindingFill"), Qt::WindingFill);
    connect(m_pComboBoxFillRule, SIGNAL(activated(int)), this, SLOT(slotChangeFillRule()));

    m_pComboBoxSpreadStyle = new QComboBox(this);
    /* 平铺效果。PadSpread没有效果 */
    m_pComboBoxSpreadStyle->addItem(tr("PadSpread"), QGradient::PadSpread);
    /* RepeatSpread和ReflectSpread只对线形和圆形渐变起作用 */
    m_pComboBoxSpreadStyle->addItem(tr("RepeatSpread"), QGradient::RepeatSpread);
    m_pComboBoxSpreadStyle->addItem(tr("ReflectSpread"), QGradient::ReflectSpread);
    connect(m_pComboBoxSpreadStyle, SIGNAL(activated(int)), this, SLOT(slotChangeSpreadStyle()));

    m_pButtonBrushColor = new QPushButton(tr("更改"), this);
    connect(m_pButtonBrushColor, SIGNAL(clicked()), this, SLOT(slotChangeBrushColor()));
    m_pFrameBrushColor = new QFrame(this);
    m_pFrameBrushColor->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_pFrameBrushColor->setAutoFillBackground(true);
    m_pFrameBrushColor->setPalette(QColor(Qt::green));

    m_pComboBoxBrushStyle = new QComboBox(this);
    /* 画刷样式 */
    m_pComboBoxBrushStyle->addItem(tr("SolidPattern"), static_cast<int>(Qt::SolidPattern)); /* 固态 */
    m_pComboBoxBrushStyle->addItem(tr("Dense1Pattern"), static_cast<int>(Qt::Dense1Pattern)); /* 密集型，点状 */
    m_pComboBoxBrushStyle->addItem(tr("Dense2Pattern"), static_cast<int>(Qt::Dense2Pattern));
    m_pComboBoxBrushStyle->addItem(tr("Dense3Pattern"), static_cast<int>(Qt::Dense3Pattern));
    m_pComboBoxBrushStyle->addItem(tr("Dense4Pattern"), static_cast<int>(Qt::Dense4Pattern));
    m_pComboBoxBrushStyle->addItem(tr("Dense5Pattern"), static_cast<int>(Qt::Dense5Pattern));
    m_pComboBoxBrushStyle->addItem(tr("Dense6Pattern"), static_cast<int>(Qt::Dense6Pattern));
    m_pComboBoxBrushStyle->addItem(tr("Dense7Pattern"), static_cast<int>(Qt::Dense7Pattern));
    m_pComboBoxBrushStyle->addItem(tr("HorPattern"), static_cast<int>(Qt::HorPattern)); /* 水平条状 */
    m_pComboBoxBrushStyle->addItem(tr("VerPattern"), static_cast<int>(Qt::VerPattern)); /* 垂直条状 */
    m_pComboBoxBrushStyle->addItem(tr("CrossPattern"), static_cast<int>(Qt::CrossPattern)); /* 水平垂直交叉，即网格  */
    m_pComboBoxBrushStyle->addItem(tr("BDiagPattern"), static_cast<int>(Qt::BDiagPattern)); /* 后向对角线 */
    m_pComboBoxBrushStyle->addItem(tr("FDiagPattern"), static_cast<int>(Qt::FDiagPattern)); /* 前向对角线 */
    m_pComboBoxBrushStyle->addItem(tr("DiagCrossPattern"), static_cast<int>(Qt::DiagCrossPattern)); /* 交叉对角线 */
    m_pComboBoxBrushStyle->addItem(tr("LinearGradientPattern"), static_cast<int>(Qt::LinearGradientPattern)); /* 线性渐变 */
    m_pComboBoxBrushStyle->addItem(tr("RadialGradientPattern"), static_cast<int>(Qt::RadialGradientPattern)); /* 放射渐变 */
    m_pComboBoxBrushStyle->addItem(tr("TexturePattern"), static_cast<int>(Qt::TexturePattern)); /*  */
    connect(m_pComboBoxBrushStyle, SIGNAL(activated(int)), this, SLOT(slotChangeBrushStyle(int)));

    m_pGridLayout = new QGridLayout;
    m_pGridLayout->addWidget(m_pLabelShap, 0, 0);
    m_pGridLayout->addWidget(m_pComboBoxShap, 0, 1);
    m_pGridLayout->addWidget(m_pLabelPenColor, 1, 0);
    m_pGridLayout->addWidget(m_pFramePenColor, 1, 1);
    m_pGridLayout->addWidget(m_pButtonPenColor, 1, 2);
    m_pGridLayout->addWidget(m_pLabelPenWidth, 2, 0);
    m_pGridLayout->addWidget(m_pSpinBoxPenWidth, 2, 1);
    m_pGridLayout->addWidget(m_pLabelPenStyle, 3, 0);
    m_pGridLayout->addWidget(m_pComboBoxPenStyle, 3, 1);
    m_pGridLayout->addWidget(m_pLabelPenCap, 4, 0);
    m_pGridLayout->addWidget(m_pComboBoxPenCap, 4, 1);
    m_pGridLayout->addWidget(m_pLabelPenJoin, 5, 0);
    m_pGridLayout->addWidget(m_pComboBoxPenJoin, 5, 1);
    m_pGridLayout->addWidget(m_pLabelFillRule, 6, 0);
    m_pGridLayout->addWidget(m_pComboBoxFillRule, 6, 1);
    m_pGridLayout->addWidget(m_pLabelSpreadStyle, 7, 0);
    m_pGridLayout->addWidget(m_pComboBoxSpreadStyle, 7, 1);
    m_pGridLayout->addWidget(m_pLabelBrushColor, 8, 0);
    m_pGridLayout->addWidget(m_pFrameBrushColor, 8, 1);
    m_pGridLayout->addWidget(m_pButtonBrushColor, 8, 2);
    m_pGridLayout->addWidget(m_pLabelBrushStyle, 9, 0);
    m_pGridLayout->addWidget(m_pComboBoxBrushStyle, 9, 1);

    m_pGridLayout->setColumnStretch(0, 1);
    m_pGridLayout->setColumnStretch(1, 2);
    m_pGridLayout->setColumnStretch(2, 1);

    m_pHLayout = new QHBoxLayout;
    m_pPaintArea = new PaintArea(this);
    m_pHLayout->addWidget(m_pPaintArea);
    m_pHLayout->addSpacing(15);
    m_pHLayout->addLayout(m_pGridLayout);

    this->setLayout(m_pHLayout);
}

MainWidget::~MainWidget()
{

}

void MainWidget::slotChangeShap(int value)
{
    PaintArea::enumShap shap = PaintArea::enumShap(m_pComboBoxShap->itemData(value, Qt::UserRole).toInt());
    m_pPaintArea->setShap(shap);
    /* 除了设置画出来的图形，还需要同步设置画刷，这样才能够使用当前选中的画刷的一些属性 */
    slotChangeBrushStyle(m_pComboBoxBrushStyle->currentIndex());
}

void MainWidget::slotChangePenColor()
{
    QColor color = QColorDialog::getColor(static_cast<int>(Qt::red));
    /* 改变调色板的颜色 */
    m_pFramePenColor->setPalette(QPalette(color));
    int width = m_pSpinBoxPenWidth->value();

    Qt::PenStyle penStyle = Qt::PenStyle(m_pComboBoxPenStyle->itemData(m_pComboBoxPenStyle->currentIndex(), Qt::UserRole).toInt());
    Qt::PenJoinStyle joinStyle = Qt::PenJoinStyle(m_pComboBoxPenJoin->itemData(m_pComboBoxPenJoin->currentIndex(), Qt::UserRole).toInt());
    Qt::PenCapStyle capStyle = Qt::PenCapStyle(m_pComboBoxPenCap->itemData(m_pComboBoxPenCap->currentIndex(), Qt::UserRole).toInt());
    m_pPaintArea->setPen(QPen(color, width, penStyle, capStyle, joinStyle));
}

void MainWidget::slotChangePenWidth(int valueWidth)
{
    QColor color = m_pFramePenColor->palette().color(QPalette::Window);
    Qt::PenStyle penStyle = Qt::PenStyle(m_pComboBoxPenStyle->itemData(m_pComboBoxPenStyle->currentIndex(), Qt::UserRole).toInt());
    Qt::PenJoinStyle joinStyle = Qt::PenJoinStyle(m_pComboBoxPenJoin->itemData(m_pComboBoxPenJoin->currentIndex(), Qt::UserRole).toInt());
    Qt::PenCapStyle capStyle = Qt::PenCapStyle(m_pComboBoxPenCap->itemData(m_pComboBoxPenCap->currentIndex(), Qt::UserRole).toInt());
    m_pPaintArea->setPen(QPen(color, valueWidth, penStyle, capStyle, joinStyle));
}

void MainWidget::slotChangePenStyle(int value)
{
    QColor color = m_pFramePenColor->palette().color(QPalette::Window);
    int width = m_pSpinBoxPenWidth->value();
    Qt::PenStyle penStyle = Qt::PenStyle(m_pComboBoxPenStyle->itemData(value, Qt::UserRole).toInt());
    Qt::PenJoinStyle joinStyle = Qt::PenJoinStyle(m_pComboBoxPenJoin->itemData(m_pComboBoxPenJoin->currentIndex(), Qt::UserRole).toInt());
    Qt::PenCapStyle capStyle = Qt::PenCapStyle(m_pComboBoxPenCap->itemData(m_pComboBoxPenCap->currentIndex(), Qt::UserRole).toInt());
    m_pPaintArea->setPen(QPen(color, width, penStyle, capStyle, joinStyle));
}

void MainWidget::slotChangePenCap(int value)
{
    QColor color = m_pFramePenColor->palette().color(QPalette::Window);
    int width = m_pSpinBoxPenWidth->value();
    Qt::PenStyle penStyle = Qt::PenStyle(m_pComboBoxPenStyle->itemData(value, Qt::UserRole).toInt());
    Qt::PenJoinStyle joinStyle = Qt::PenJoinStyle(m_pComboBoxPenJoin->itemData(m_pComboBoxPenJoin->currentIndex(), Qt::UserRole).toInt());
    Qt::PenCapStyle capStyle = Qt::PenCapStyle(m_pComboBoxPenCap->itemData(value, Qt::UserRole).toInt());
    m_pPaintArea->setPen(QPen(color, width, penStyle, capStyle, joinStyle));
}

void MainWidget::slotChangePenJoin(int value)
{
    QColor color = m_pFramePenColor->palette().color(QPalette::Window);
    int width = m_pSpinBoxPenWidth->value();
    Qt::PenStyle penStyle = Qt::PenStyle(m_pComboBoxPenStyle->itemData(value, Qt::UserRole).toInt());
    Qt::PenJoinStyle joinStyle = Qt::PenJoinStyle(m_pComboBoxPenJoin->itemData(value, Qt::UserRole).toInt());
    Qt::PenCapStyle capStyle = Qt::PenCapStyle(m_pComboBoxPenCap->itemData(value, Qt::UserRole).toInt());
    m_pPaintArea->setPen(QPen(color, width, penStyle, capStyle, joinStyle));
}

void MainWidget::slotChangeFillRule()
{
    Qt::FillRule fillRule = Qt::FillRule(m_pComboBoxFillRule->itemData(m_pComboBoxFillRule->currentIndex(), Qt::UserRole).toInt());
    m_pPaintArea->setFillRule(fillRule);
}

void MainWidget::slotChangeSpreadStyle()
{
    m_spread = QGradient::Spread(m_pComboBoxSpreadStyle->itemData(m_pComboBoxSpreadStyle->currentIndex(), Qt::UserRole).toInt());
}

void MainWidget::slotChangeBrushColor()
{
    QColor color = QColorDialog::getColor(static_cast<int>(Qt::red));
    m_pFrameBrushColor->setPalette(QPalette(color));
    slotChangeBrushStyle(m_pComboBoxBrushStyle->currentIndex());
}

void MainWidget::slotChangeBrushStyle(int value)
{
    QColor color = m_pFrameBrushColor->palette().color(QPalette::Window);
    Qt::BrushStyle style = Qt::BrushStyle(m_pComboBoxBrushStyle->itemData(value, Qt::UserRole).toInt());

    switch (style) {
    /* 线性渐变 */
    case Qt::LinearGradientPattern: {
        /* 因为case语句中存在局部变量的定义，所以case语句需要用{}，否则编译器会警告 */
        QLinearGradient linearGradient(0, 0, 400, 400);
        linearGradient.setColorAt(0.0, Qt::white);
        linearGradient.setColorAt(0.2, color);
        linearGradient.setColorAt(1.0, Qt::black);
        linearGradient.setSpread(m_spread);
        m_pPaintArea->setBrush(linearGradient);
        break;
    }
    case Qt::RadialGradientPattern: {
        /* 放射性渐变 */
        QRadialGradient radialGradient(200, 200, 150, 150, 100);
        radialGradient.setColorAt(0.0, Qt::white);
        radialGradient.setColorAt(0.2, color);
        radialGradient.setColorAt(1.0, Qt::black);
        radialGradient.setSpread(m_spread);
        m_pPaintArea->setBrush(radialGradient);
        break;
    }
    case Qt::ConicalGradientPattern: {
        /* 圆锥形渐变 */
        QConicalGradient conicalGradient(200, 200, 300);
        conicalGradient.setColorAt(0.0, Qt::white);
        conicalGradient.setColorAt(0.2, color);
        conicalGradient.setColorAt(1.0, Qt::black);
        conicalGradient.setSpread(m_spread);
        m_pPaintArea->setBrush(conicalGradient);
        break;
    }
    case Qt::TexturePattern:
        m_pPaintArea->setBrush(QBrush(QPixmap(":/butterfly.jpg")));
        break;
    default:
        m_pPaintArea->setBrush(QBrush(color, style));
        break;
    }
}
