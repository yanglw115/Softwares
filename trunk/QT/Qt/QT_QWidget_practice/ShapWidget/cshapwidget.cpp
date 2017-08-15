
#include <QMouseEvent>
#include <QBitmap>
#include <QPainter>
#include <QtDebug>
#include <QPixmap>
#include <QImageReader>
#include <QApplication>

#include "cshapwidget.h"

CShapWidget::CShapWidget(QWidget *parent)
    : QWidget(parent)
{
    QPixmap pixmap;
    /* 注意，这里在加载图片的时候，windows下只写图片而不是绝路路径，则无法正常加载 */
    QString strPicPath = QApplication::applicationDirPath() + "/" + "2.png";
    if (!pixmap.load(strPicPath, 0, Qt::AvoidDither | Qt::ThresholdDither | Qt::ThresholdAlphaDither)) {
        qWarning() << "Pixmap load failed!";
        return;
    }
    /* 使窗口大小与图片大小一致 */
    this->resize(pixmap.size());
    qDebug() << "Set window size:" << pixmap.size();
    /* 这里需要用到png透明图片，不然达不到想要的效果。设置之后，只有不透明的地方才可以响应鼠标 */
    this->setMask(QBitmap(pixmap.mask()));

    foreach (QString format, QImageReader::supportedImageFormats()) {
        qDebug() << format;
    }
}

CShapWidget::~CShapWidget()
{
}

void CShapWidget::mousePressEvent(QMouseEvent *event)
{
    if (Qt::LeftButton == event->button()) {
        m_dragPoint = event->globalPos() - this->frameGeometry().topLeft();
        event->accept();
    }
}

void CShapWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        this->move(event->globalPos() - m_dragPoint);
        event->accept();
    }
}

void CShapWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixmap;
    QString strPicPath = QApplication::applicationDirPath() + "/" + "2.png";
    if (!pixmap.load(strPicPath)) {
        qWarning() << "Paintevent load pixmap failed!";
        return;
    }
    painter.drawPixmap(0, 0, pixmap);
}
