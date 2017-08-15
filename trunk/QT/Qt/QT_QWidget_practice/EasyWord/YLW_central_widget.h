#ifndef CCENTRALWIDGET_H
#define CCENTRALWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QApplication>


class CCentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CCentralWidget(QWidget *parent = 0);

signals:

public slots:

public:
    QImage m_image;
    QLabel *m_pLabel;
    QGraphicsView *m_pView;
    QGraphicsScene *m_pScene;
    QGraphicsItem *m_pImgItem;
    QTextEdit *m_pTextEdit;
    QHBoxLayout *m_pHLayout;
};

#endif // CCENTRALWIDGET_H
