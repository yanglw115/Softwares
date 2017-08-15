#include "YLW_draw_widget.h"

DrawWidget::DrawWidget(QWidget *parent)
    : QWidget(parent)
    , m_pPixmap(NULL)
    , m_color(Qt::black)
    , m_style(1)
    , m_width(1)
{
    this->setAutoFillBackground(true);
    this->setPalette(QPalette(Qt::white));
    /* 这里双Buffer的原理，是先把图形画在QPixmap上面，然后再通过QPaintr直接将QPixmap画出来。为了使整个绘画过程
     * 当中原来绘画的内容不被冲掉，QPixmap需要放在堆上 */
    m_pPixmap = new QPixmap(this->size());
    m_pPixmap->fill(Qt::white);
    this->setMinimumSize(600, 400);
}

DrawWidget::~DrawWidget()
{
    if (m_pPixmap) {
        delete m_pPixmap;
        m_pPixmap = NULL;
    }
}

void DrawWidget::mousePressEvent(QMouseEvent *event)
{
    m_pointStart = event->pos();
}

void DrawWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_pointEnd = event->pos();
    /* 设置画笔 */
    QPen pen;
    pen.setStyle((Qt::PenStyle)m_style);
    pen.setColor(m_color);
    pen.setWidth(m_width);

    /* 以下两种方式是一样的 */
#if 0
    QPainter painter;
    painter.begin(m_pPixmap);
    /* setPen()的动作需要放在begin()之后，不然不生效，因为此时painter还未激活 */
    painter.setPen(pen);
    painter.drawLine(m_pointStart, m_pointEnd);
    painter.end();
#else
    QPainter painter(m_pPixmap);
    painter.setPen(pen);
    painter.drawLine(m_pointStart, m_pointEnd);
#endif

    /* 记得这里需要刷新画笔的起始点 */
    m_pointStart = m_pointEnd;

    update();
}

void DrawWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    /* 将m_pPixmap中的内容全部画出来 */
    painter.drawPixmap(QPoint(0, 0), *m_pPixmap);
}

void DrawWidget::resizeEvent(QResizeEvent *event)
{
    /* 如果不对resize进行处理，则窗口大小变化后，新扩大的部分区域将不能绘画 */
    if ((this->width() > m_pPixmap->width()) || (this->height() > m_pPixmap->height())) {
        QPixmap *pPixmapNew = new QPixmap(this->size());
        pPixmapNew->fill(Qt::white);
        QPainter painter(pPixmapNew);
        painter.drawPixmap(0, 0, *m_pPixmap);
        /* 这里并不需要进行update()，因为this这个QWidget上面已经画好了，m_pPixmap只是一个缓冲区，这里只是对缓冲区进行重新设置 */
        delete m_pPixmap;
        m_pPixmap = pPixmapNew;
    }
    QWidget::resizeEvent(event);
}

void DrawWidget::setColor(const QColor &color)
{
    m_color = color;
}

void DrawWidget::setWidth(const int &width)
{
    m_width = width;
}

void DrawWidget::setStyle(const int style)
{
    m_style = style;
}

void DrawWidget::clearDraw()
{
    m_pPixmap->fill(Qt::white);
    update();
}

