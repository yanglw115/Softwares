#ifndef YLW_GEOMETRY_H
#define YLW_GEOMETRY_H

#include <QDialog>
#include <QLabel>
#include <QGridLayout>

/**
 * @brief The Geometry class
 * 对于一个Frame的窗口而言，其有边框与clientArea，所以存在几个组成部分的坐标
 */
class Geometry : public QDialog
{
    Q_OBJECT

public:
    Geometry(QWidget *parent = 0);
    ~Geometry();

    void updateValue();
    void moveEvent(QMoveEvent *);
    void resizeEvent(QResizeEvent *);

private:
    QLabel *m_pLabelX;	/* 对于this->x()返回的是整个边框的x坐标 */
    QLabel *m_pLabelXValue;
    QLabel *m_pLabelY;	/* this->y()返回的是整个边框的y坐标 */
    QLabel *m_pLabelYValue;
    QLabel *m_pLabelPos; /* this->pos()返回的是整个边框的位置，即x与y坐标的组合 */
    QLabel *m_pLabelPosValue;
    QLabel *m_pLabelFrame; /* frameGeometry()返回的是整个边框的几何坐标 */
    QLabel *m_pLabelFrameValue;
    QLabel *m_pLabelGeometry; /* geometry()返回的是clienArea的几何坐标，即除边框外的相当于正文区的坐标 */
    QLabel *m_pLabelGeoValue;
    QLabel *m_pLabelWidth; /* this->width()返回clientArea的宽度 */
    QLabel *m_pLabelWValue;
    QLabel *m_pLabelHeight; /* this->height()返回clientArea的高度 */
    QLabel *m_pLabelHValue;
    QLabel *m_pLabelRect; /* this->rect()返回clientArea的矩形坐标，一般与geometry是一致的 */
    QLabel *m_pLabelRectValue;
    QLabel *m_pLabelSize; /*  this->size()返回clientAream的长宽大小 */
    QLabel *m_pLabelSizeValue;

    QGridLayout *m_pLayout;
};

#endif // YLW_GEOMETRY_H
