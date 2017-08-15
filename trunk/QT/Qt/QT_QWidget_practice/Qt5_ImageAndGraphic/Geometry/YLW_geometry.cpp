#include "YLW_geometry.h"

Geometry::Geometry(QWidget *parent)
    : QDialog(parent)
    , m_pLabelX(NULL)
    , m_pLabelXValue(NULL)
    , m_pLabelY(NULL)
    , m_pLabelYValue(NULL)
    , m_pLabelPos(NULL)
    , m_pLabelPosValue(NULL)
    , m_pLabelFrame(NULL)
    , m_pLabelFrameValue(NULL)
    , m_pLabelGeometry(NULL)
    , m_pLabelGeoValue(NULL)
    , m_pLabelWidth(NULL)
    , m_pLabelWValue(NULL)
    , m_pLabelHeight(NULL)
    , m_pLabelHValue(NULL)
    , m_pLabelRect(NULL)
    , m_pLabelRectValue(NULL)
    , m_pLabelSize(NULL)
    , m_pLabelSizeValue(NULL)
    , m_pLayout(NULL)
{
    m_pLabelX = new QLabel(tr("x():"), this);
    m_pLabelXValue = new QLabel(this);
    m_pLabelY = new QLabel(tr("y():"), this);
    m_pLabelYValue = new QLabel(this);
    m_pLabelPos = new QLabel(tr("Pos():"), this);
    m_pLabelPosValue = new QLabel(this);
    m_pLabelFrame = new QLabel(tr("Frame():"), this);
    m_pLabelFrameValue = new QLabel(this);
    m_pLabelGeometry = new QLabel(tr("Geometry():"), this);
    m_pLabelGeoValue = new QLabel(this);
    m_pLabelWidth = new QLabel(tr("width():"), this);
    m_pLabelWValue = new QLabel(this);
    m_pLabelHeight = new QLabel(tr("height():"), this);
    m_pLabelHValue = new QLabel(this);
    m_pLabelRect = new QLabel(tr("Rect():"), this);
    m_pLabelRectValue = new QLabel(this);
    m_pLabelSize = new QLabel(tr("size():"), this);
    m_pLabelSizeValue = new QLabel(this);

    m_pLayout = new QGridLayout(this);
    m_pLayout->addWidget(m_pLabelX, 0, 0);
    m_pLayout->addWidget(m_pLabelXValue, 0, 1);
    m_pLayout->addWidget(m_pLabelY, 1, 0);
    m_pLayout->addWidget(m_pLabelYValue, 1, 1);
    m_pLayout->addWidget(m_pLabelPos, 2, 0);
    m_pLayout->addWidget(m_pLabelPosValue, 2, 1);
    m_pLayout->addWidget(m_pLabelFrame, 3, 0);
    m_pLayout->addWidget(m_pLabelFrameValue, 3, 1);
    m_pLayout->addWidget(m_pLabelGeometry, 4, 0);
    m_pLayout->addWidget(m_pLabelGeoValue, 4, 1);
    m_pLayout->addWidget(m_pLabelWidth, 5, 0);
    m_pLayout->addWidget(m_pLabelWValue, 5, 1);
    m_pLayout->addWidget(m_pLabelHeight, 6, 0);
    m_pLayout->addWidget(m_pLabelHValue, 6, 1);
    m_pLayout->addWidget(m_pLabelRect, 7, 0);
    m_pLayout->addWidget(m_pLabelRectValue, 7, 1);
    m_pLayout->addWidget(m_pLabelSize, 8, 0);
    m_pLayout->addWidget(m_pLabelSizeValue, 8, 1);
}

Geometry::~Geometry()
{

}

void Geometry::updateValue()
{
    m_pLabelXValue->setText(QString("").setNum(this->x()));
    m_pLabelYValue->setText(QString("").setNum(this->y()));
    m_pLabelPosValue->setText("(" + QString("").setNum(this->pos().x()) + ", " + QString().setNum(this->pos().y()) + ")");
    m_pLabelFrameValue->setText("(" + QString("").setNum(this->frameGeometry().x()) + ", " + QString("").setNum(this->frameGeometry().y())
                                + ", " + QString("").setNum(this->frameGeometry().width()) + ", " + QString("").setNum(this->frameGeometry().height()) + ")");
    m_pLabelGeoValue->setText("(" + QString("").setNum(this->geometry().x()) + ", " + QString("").setNum(this->geometry().y())
                              + ", " + QString("").setNum(this->geometry().width()) + ", " + QString("").setNum(this->geometry().height()) + ")");
    m_pLabelWValue->setText(QString("").setNum(this->width()));
    m_pLabelHValue->setText(QString("").setNum(this->height()));
    m_pLabelRectValue->setText("(" + QString("").setNum(this->rect().x()) + ", " + QString("").setNum(this->rect().y())
                               + ", " + QString("").setNum(this->rect().width()) + ", " + QString("").setNum(this->rect().height()) + ")");
    m_pLabelSizeValue->setText("(" + QString("").setNum(this->size().width())+ ", " + QString("").setNum(this->size().height()) + ")");
}

void Geometry::moveEvent(QMoveEvent *)
{
    updateValue();
}

void Geometry::resizeEvent(QResizeEvent *)
{
    updateValue();
}
