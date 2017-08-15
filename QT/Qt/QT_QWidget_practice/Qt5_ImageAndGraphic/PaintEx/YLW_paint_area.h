#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QPen>

class PaintArea : public QWidget
{
    Q_OBJECT
public:
    enum enumShap {
        Line, Rectangle, RoundRect, Ellipse, Polygon, PolyLine, Points, Arc, Path, Text, Pixmap
    };
    explicit PaintArea(QWidget *parent = 0);

    void setShap(enumShap);
    void setBrush(QBrush);
    void setPen(QPen);
    void setFillRule(Qt::FillRule);
    void paintEvent(QPaintEvent *);

signals:

public slots:
private:
    enumShap m_shap;
    QBrush m_brush;
    QPen m_pen;
    Qt::FillRule m_fillRule;
};

#endif // PAINTAREA_H
