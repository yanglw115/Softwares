#ifndef CSHAPWIDGET_H
#define CSHAPWIDGET_H

#include <QWidget>

class CShapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CShapWidget(QWidget *parent = 0);
    ~CShapWidget();

    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

    void paintEvent(QPaintEvent *);

private:
    QPoint m_dragPoint;

};

#endif // CSHAPWIDGET_H
