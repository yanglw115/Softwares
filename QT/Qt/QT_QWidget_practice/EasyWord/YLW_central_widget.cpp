
#include <QtDebug>

#include "YLW_central_widget.h"

CCentralWidget::CCentralWidget(QWidget *parent)
    : QWidget(parent)
    , m_pLabel(NULL)
    , m_pView(NULL)
    , m_pScene(NULL)
    , m_pImgItem(NULL)
    , m_pTextEdit(NULL)
    , m_pHLayout(NULL)
{
    /* 中心widget有左右两部分组成，左边展示图片，右边编辑文本 */
    QGraphicsView *m_pView = new QGraphicsView(this);
    QGraphicsScene *m_pScene = new QGraphicsScene(this);
#ifndef WITH_LABEL_IMAGE
    QString strPath = QApplication::applicationDirPath() + "/" + "2.png";
    QImage img;
    if (img.load(strPath)) {
        /* 通过对返回的QGraphicsItem进行setTransform操作，可以实现图片的放大及缩小 */
        m_pImgItem = m_pScene->addPixmap(QPixmap::fromImage(img));
    } else {
        qWarning() << "Image load failed:" << strPath;
    }
    m_pView->setScene(m_pScene);

#else // WITH_LABEL_IMAGE
    /** 使用QLabel展示图片的时候，如果对图片进行放大，则Label也会变大，不满足在固定的平面展示放大后的图片需求，
     * 因为QLabel不具备scroll功能，使用QGraphicsView则能满足 */
    m_pLabel = new QLabel(this);
    m_pLabel->setScaledContents(false);
    m_pLabel->setFixedSize(200, 200);
#endif // WITH_LABEL_IMAGE

    m_pTextEdit = new QTextEdit(this);
    m_pHLayout = new QHBoxLayout(this);

#ifdef WITH_LABEL_IMAGE
    m_pHLayout->addWidget(m_pLabel);
#else
    m_pHLayout->addWidget(m_pView);
#endif // WITH_LABEL_IMAGE
    m_pHLayout->addWidget(m_pTextEdit);

}

