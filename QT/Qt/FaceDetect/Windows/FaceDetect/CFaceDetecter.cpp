#include <QFileInfo>
#include <QMessageBox>

#include "CFaceDetecter.h"
#include "faceDetect/faceLandmarkDetect.h"

CFaceDetecter::CFaceDetecter(QWidget * parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    this->hide();
    this->setFixedSize(800, 600);
    initWindow();
    m_pObjResult = new CObjectResult(this);
}


CFaceDetecter::~CFaceDetecter()
{

}

void CFaceDetecter::startDetect(const QString &strImgPath, const enumItemType type)
{
    QFileInfo fileInfo(strImgPath);
    if (faceDetect(fileInfo.fileName().toStdString(), fileInfo.absoluteFilePath().toStdString())) {
        qDebug() << "Detected face from image file: " << strImgPath;
    } else {
        if (TYPE_ALL == type) {
            QMessageBox::critical(this, tr("面部特征检测"), tr("检测失败，没有检测到人脸，请确认图片!"));
            this->deleteLater();
            return;
        }
    }
    this->show();
}

void CFaceDetecter::initWindow()
{
    m_pHLayoutMain = new QHBoxLayout;
    m_pVLayoutLeft = new QVBoxLayout;
    m_pVLayoutRight = new QVBoxLayout;
    m_pLabelResult = new QLabel(tr("检测项目"), this);
    m_pLabelDetails = new QLabel(tr("结果详情"), this);
    m_pLineLeft = new QFrame(this);
    m_pLineLeft->setFrameShape(QFrame::HLine);
    m_pLineRight = new QFrame(this);
    m_pLineRight->setFrameShape(QFrame::HLine);
    m_pLineMiddle = new QFrame(this);
    m_pLineMiddle->setFrameShape(QFrame::VLine);
    m_pListWidget = new QListWidget(this);
    m_pListWidget->setSpacing(5);
    m_pListWidget->addItem(tr("痘痘"));
    m_pListWidget->addItem(tr("肤色"));
    m_pListWidget->addItem(tr("黑头"));
    m_pListWidget->addItem(tr("毛孔粗大度"));
    m_pListWidget->addItem(tr("皮肤光滑度"));
    m_pDetailPimples = new CResultDetail(TYPE_PIMPLES, this);
    m_pDetailBlackheads = new CResultDetail(TYPE_BLACKHEADS, this);
    m_pDetailFaceColor = new CResultDetail(TYPE_FACE_COLORE, this);
    m_pDetailPore = new CResultDetail(TYPE_PORE, this);
    m_pDetailCoarse = new CResultDetail(TYPE_COARSENESS, this);
    m_pStacked = new QStackedWidget(this);
    m_pStacked->addWidget(m_pDetailPimples);
    m_pStacked->addWidget(m_pDetailBlackheads);
    m_pStacked->addWidget(m_pDetailFaceColor);
    m_pStacked->addWidget(m_pDetailPore);
    m_pStacked->addWidget(m_pDetailCoarse);
    connect(m_pListWidget, SIGNAL(currentRowChanged(int)), m_pStacked, SLOT(setCurrentIndex(int)));

    m_pVLayoutLeft->addWidget(m_pLabelResult, 0, Qt::AlignHCenter);
    m_pVLayoutLeft->addWidget(m_pLineLeft);
    m_pVLayoutLeft->addWidget(m_pListWidget);

    m_pVLayoutRight->addWidget(m_pLabelDetails, 0, Qt::AlignHCenter);
    m_pVLayoutRight->addWidget(m_pLineRight);
    m_pVLayoutRight->addWidget(m_pStacked);

    m_pHLayoutMain->addLayout(m_pVLayoutLeft);
    m_pHLayoutMain->addWidget(m_pLineMiddle);
    m_pHLayoutMain->addLayout(m_pVLayoutRight, 1);
    this->setLayout(m_pHLayoutMain);
}
