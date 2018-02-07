#include <QtDebug>

#include "CResultDetail.h"
#include "vs_charset.h"

CResultDetail::CResultDetail(enumItemType type, QWidget *parent)
    : QWidget(parent)
    , m_type(type)
{
    m_pLabelImage = new QLabel(this);
    m_pLabelImage->setFixedSize(360, 540);
    m_pLabelItem = new QLabel(this);
    m_pEditTop = new QLineEdit(this);

    m_pVLayoutMain = new QVBoxLayout;
    m_pHLayoutData = new QHBoxLayout;
    m_pGridData = new QGridLayout;

    m_pVLayoutMain->addWidget(m_pLabelImage, 0, Qt::AlignHCenter);
    m_pVLayoutMain->addLayout(m_pHLayoutData);
    m_pHLayoutData->addStretch(1);
    m_pHLayoutData->addWidget(m_pLabelItem);
    m_pHLayoutData->addLayout(m_pGridData);
    m_pHLayoutData->addStretch(1);

    switch (type) {
    case TYPE_PIMPLES:
        /* 这里不太方便将所有的LineEdit放在前面统一初始化，因为不用的widget也可能会被展示出来 */
        m_pEditMiddle1 = new QLineEdit(this);
        m_pEditMiddle2 = new QLineEdit(this);
        m_pEditMiddle3 = new QLineEdit(this);
        m_pEditBottom = new QLineEdit(this);
        m_pLabelItem->setText(tr("痘痘："));
        m_pGridData->addWidget(m_pEditTop, 0, 1);
        m_pGridData->addWidget(m_pEditMiddle1, 1, 0);
        m_pGridData->addWidget(m_pEditMiddle2, 1, 1);
        m_pGridData->addWidget(m_pEditMiddle3, 1, 2);
        m_pGridData->addWidget(m_pEditBottom, 2, 1);
        break;
    case TYPE_BLACKHEADS:
        m_pLabelItem->setText(tr("黑头："));
        m_pGridData->addWidget(m_pEditTop, 0, 0);
        break;
    case TYPE_FACE_COLOR:
        m_pEditMiddle1 = new QLineEdit(this);
        m_pLabelItem->setText(tr("肤色："));
        m_pGridData->addWidget(m_pEditTop, 0, 0);
        m_pGridData->addWidget(m_pEditMiddle1, 1, 0);
        break;
    case TYPE_PORE:
        m_pEditMiddle1 = new QLineEdit(this);
        m_pEditMiddle2 = new QLineEdit(this);
        m_pLabelItem->setText(tr("毛孔粗大度："));
        m_pGridData->addWidget(m_pEditTop, 0, 0);
        m_pGridData->addWidget(m_pEditMiddle1, 1, 0);
        m_pGridData->addWidget(m_pEditMiddle2, 1, 1);
        break;
    case TYPE_COARSENESS:
        m_pEditMiddle1 = new QLineEdit(this);
        m_pLabelItem->setText(tr("皮肤光滑度："));
        m_pGridData->addWidget(m_pEditTop, 0, 0);
        m_pGridData->addWidget(m_pEditMiddle1, 1, 0);
        break;
    default:
        break;
    }

    this->setLayout(m_pVLayoutMain);
}

CResultDetail::~CResultDetail()
{

}

void CResultDetail::setImagePath(const QString &strImagePath)
{
    m_strImagePath = strImagePath;
    if (m_pixmap.load(m_strImagePath)) {
        m_pixmap = m_pixmap.scaled(m_pLabelImage->width(), m_pLabelImage->height(), Qt::KeepAspectRatio);
        m_pLabelImage->setPixmap(m_pixmap);
    } else {
        qWarning() << "Load image failed: " << m_strImagePath;
    }
}

void CResultDetail::setData(const QString &strData)
{
    m_pEditTop->setText(strData);
}

void CResultDetail::setData(const QString &strData1, const QString &strData2)
{
    m_pEditTop->setText(strData1);
    m_pEditMiddle1->setText(strData2);
}

void CResultDetail::setData(const QString &strData1, const QString &strData2, const QString &strData3)
{
    m_pEditTop->setText(strData1);
    m_pEditMiddle1->setText(strData2);
    m_pEditMiddle2->setText(strData3);
}

void CResultDetail::setData(const QString &strData1, const QString &strData2, const QString &strData3,
                            const QString &strData4, const QString &strData5)
{
    m_pEditTop->setText(strData1);
    m_pEditMiddle1->setText(strData2);
    m_pEditMiddle2->setText(strData3);
    m_pEditMiddle3->setText(strData4);
    m_pEditBottom->setText(strData5);
}

