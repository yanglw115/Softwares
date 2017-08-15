#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPen>
#include <QPainter>
#include <QResizeEvent>

class DrawWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawWidget(QWidget *parent = 0);
    virtual ~DrawWidget();

    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

    void setColor(const QColor &);
    void setWidth(const int &);
    void setStyle(const int);
    void clearDraw();

private:
    QPixmap *m_pPixmap;
    QColor m_color;
    int m_style;
    int m_width;
    QPoint m_pointStart;
    QPoint m_pointEnd;
};

#endif // DRAWWIDGET_H
