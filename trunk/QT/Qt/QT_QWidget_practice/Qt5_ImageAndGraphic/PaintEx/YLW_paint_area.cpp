#include <QPainter>
#include <QtDebug>
#include <QLabel>

#include "YLW_paint_area.h"

PaintArea::PaintArea(QWidget *parent) : QWidget(parent)
{
    /* setPalette(QPalette(Qt::white))与setPalette(QPalette(QPalette::Window, Qt::white))
     * 等价,这种方法是清除原来的palette,只设置了Window role为white颜色。要保留原来的palette而只改
     * 变部分，则需要使用#if中的测试代码 */
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
    setMinimumSize(400, 400);
#if 0
    QPalette palette = this->palette();
    palette.setColor(QPalette::WindowText, Qt::red);
    this->setPalette(palette);

    QLabel *label = new QLabel(tr("Hello QT!"), this);
    label->setGeometry(20, 20, 100, 100);
#endif
}

void PaintArea::setShap(PaintArea::enumShap shap)
{
    m_shap = shap;
    update();
}

void PaintArea::setBrush(QBrush brush)
{
    m_brush = brush;
    update();
}

void PaintArea::setPen(QPen pen)
{
    m_pen = pen;
    update();
}

void PaintArea::setFillRule(Qt::FillRule rule)
{
    m_fillRule = rule;
    update();
}

void PaintArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    /* 画笔是用来画轮廓 */
    painter.setPen(m_pen);
    /* 画刷用来填充内容 */
    painter.setBrush(m_brush);

    QPainterPath painterPath; /* 画笔路径 */
    QRect rect(50, 100, 300, 200);
    QPoint pointArray[] = { /* 点数组 */
        {150, 100},
        {300, 150},
        {350, 250},
        {100, 300}
    };

    /* 每1度分成16份表示，所以30度就是30*16 */
    int startAngle = 30 * 16; /* 起始角度，逆时针方向与X轴的角度 */
    int spinAngle = 120 * 16; /* 跨度 */

    /* 先画一个矩形 */
    painterPath.addRect(150, 150, 100, 100);
    /* 然后再将起始点移到(100, 100) */
    painterPath.moveTo(100, 100);
    /* 绘制三次贝赛尔曲线，详情可以参考说明文档 */
    /* void QPainterPath::cubicTo(qreal c1X, qreal c1Y, qreal c2X, qreal c2Y, qreal endPointX, qreal endPointY) */
    /* 第一条贝赛尔曲线起点为(100, 100)，终点为(300, 200) */
    painterPath.cubicTo(300, 100, 200, 200, 300, 200);
    /* 第二条贝赛尔曲线起点为(300, 200)，终点为(100, 100) */
    painterPath.cubicTo(100, 300, 200, 200, 100, 100);
    painterPath.setFillRule(m_fillRule);

    switch (m_shap) {
    case Line:
        /* 画直线 */
        painter.drawLine(rect.topLeft(), rect.bottomRight());
        break;
    case Rectangle:
        /* 画矩形 */
        painter.drawRect(rect);
        break;
    case RoundRect:
        /* 画圆角矩形 */
        //painter.drawRoundRect(rect);
        /* 自定义圆角弧度 */
        painter.drawRoundedRect(rect, 20, 30);
        break;
    case Ellipse:
        /* 画椭圆，根据矩形来画 */
        painter.drawEllipse(rect);
        break;
    case Polygon:
        /* 画多边形，4代表数组中的前4个点 */
        painter.drawPolygon(pointArray, 4);
        break;
    case PolyLine:
        /* 画多线段 */
        painter.drawPolyline(pointArray, 4);
        break;
    case Points:
        /* 画点，画数组中的前4个点 */
        painter.drawPoints(pointArray, 4);
        break;
    case Arc:
        /* 画弧形 */
        painter.drawArc(rect, startAngle, spinAngle);
        break;
    case Path:
        /* 根据指定的路径进行绘画 */
        painter.drawPath(painterPath);
        break;
        /* 绘制文本数据 */
    case Text:
        /* 在给定的矩形中画 */
        painter.drawText(rect, Qt::AlignCenter, tr("Hello QT!"));
        break;
    case Pixmap:
        /* 绘制图片，给定的起点。这种情况下，如果图片大于展示区域，则图片只会显示部分 */
        //painter.drawPixmap(20, 20, QPixmap(":/butterfly.jpg"));
        /* 在给定的矩形中绘制图片 */
        painter.drawPixmap(rect, QPixmap(":/butterfly.jpg"));
        break;
    default:
        break;
    }
}

