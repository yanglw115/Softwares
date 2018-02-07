#include <QFileInfo>
#include <QMessageBox>

#include "CFaceDetecter.h"
#include "vs_charset.h"

CFaceDetecter::CFaceDetecter(QWidget * parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    this->hide();
    this->setFixedSize(800, 700);
    this->setWindowTitle(tr("Detection details"));
    this->setWindowIcon(QIcon(":/icons/result.png"));
    initWindow();
    m_pObjResult = new CObjectResult(this);
}


CFaceDetecter::~CFaceDetecter()
{

}

void CFaceDetecter::startDetect(const QString &strImgPath, const enumItemType type, QWidget *pWidgetMain)
{
    QFileInfo fileInfo(strImgPath);
    enumTypeResult nHasFace = TYPE_ERROR;
    m_strImageName = fileInfo.fileName();
    m_strImagePath = fileInfo.absoluteFilePath();
    cv::Mat matSrc = cv::imread(m_strImagePath.toStdString());
    if ((matSrc.cols > 1280 && matSrc.rows > 720) ||
            (matSrc.rows > 1280 && matSrc.cols > 720)) {
        if (matSrc.cols > matSrc.rows) {
            cv::resize(matSrc, matSrc, cv::Size(1280, 1280 * matSrc.rows / matSrc.cols));
        } else {
            cv::resize(matSrc, matSrc, cv::Size(1280 * matSrc.cols / matSrc.rows, 1280));
        }
    }

    pWidgetMain->setDisabled(true); // 主界面使能关闭
    nHasFace = faceLandmarkDetect(m_strImageName.toStdString(), matSrc,
                                      m_vectorFace, m_rectFace);
    switch (nHasFace) {
    case TYPE_OK:
        m_bHasFace = true;
        qDebug() << "Detected face from image file: " << strImgPath;
        break;
    case TYPE_FALSE:
        m_bHasFace = false;
        if (TYPE_ALL == type) {
            QMessageBox::critical(this, tr("面部特征检测"), tr("检测失败，没有检测到人脸，请确认图片!"));
            pWidgetMain->setDisabled(false); // 主界面使能打开
            this->deleteLater();
            return;
        }
        break;
    case TYPE_ERROR:
        QMessageBox::critical(this, tr("面部特征检测"), tr("输入图片不合法或特征库加载失败，详情参考日志!"));
        pWidgetMain->setDisabled(false); // 主界面使能打开
        this->deleteLater();
        return;
        break;
    default:
        break;
    }

    switch (type) {
    case TYPE_ALL:
        detectAll(matSrc);
        break;
    case TYPE_PIMPLES:
        detectPimples(matSrc);
        m_pListWidget->setCurrentRow(ROW_PIMPLES);
        break;
    case TYPE_BLACKHEADS:
        detectBlackheads(matSrc);
        m_pListWidget->setCurrentRow(ROW_BLACKHEADS);
        break;
    case TYPE_FACE_COLOR:
        detectFaceColor(matSrc);
        m_pListWidget->setCurrentRow(ROW_FACE_COLOR);
        break;
    case TYPE_PORE:
        detectPore(matSrc);
        m_pListWidget->setCurrentRow(ROW_PORE);
        break;
    case TYPE_COARSENESS:
        detectCoarseness(matSrc);
        m_pListWidget->setCurrentRow(ROW_COARSENESS);
        break;
    default:
        break;
    }
    showListItems(type);
    this->show();
    this->activateWindow();
    pWidgetMain->setDisabled(false); // 主界面使能打开
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
    m_pListWidget->addItem(tr("黑头"));
    m_pListWidget->addItem(tr("肤色"));
    m_pListWidget->addItem(tr("毛孔粗大度"));
    m_pListWidget->addItem(tr("皮肤光滑度"));
    m_pListWidget->item(ROW_PIMPLES)->setHidden(true);
    m_pListWidget->item(ROW_BLACKHEADS)->setHidden(true);
    m_pListWidget->item(ROW_FACE_COLOR)->setHidden(true);
    m_pListWidget->item(ROW_PORE)->setHidden(true);
    m_pListWidget->item(ROW_COARSENESS)->setHidden(true);

    m_pDetailPimples = new CResultDetail(TYPE_PIMPLES, this);
    m_pDetailBlackheads = new CResultDetail(TYPE_BLACKHEADS, this);
    m_pDetailFaceColor = new CResultDetail(TYPE_FACE_COLOR, this);
    m_pDetailPore = new CResultDetail(TYPE_PORE, this);
    m_pDetailCoarse = new CResultDetail(TYPE_COARSENESS, this);
    m_pStacked = new QStackedWidget(this);
    m_pStacked->addWidget(m_pDetailPimples);
    m_pStacked->addWidget(m_pDetailBlackheads);
    m_pStacked->addWidget(m_pDetailFaceColor);
    m_pStacked->addWidget(m_pDetailPore);
    m_pStacked->addWidget(m_pDetailCoarse);
    m_pStacked->setHidden(true);
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

void CFaceDetecter::detectAll(const cv::Mat &srcMat)
{
    detectPimples(srcMat);
    detectBlackheads(srcMat);
    detectFaceColor(srcMat);
    detectPore(srcMat);
    detectCoarseness(srcMat);
}

void CFaceDetecter::detectPimples(const cv::Mat &srcMat)
{
    findFaceSpots(m_strImageName.toStdString(), srcMat,
                  m_bHasFace, m_vectorFace, TYPE_PIMPLES, m_pObjResult);
    m_pDetailPimples->setImagePath(m_pObjResult->m_objPimples.m_strImgPath);
    m_pDetailPimples->setData(m_pObjResult->m_objPimples.m_strForehead, m_pObjResult->m_objPimples.m_strRight,
                              m_pObjResult->m_objPimples.m_strNose, m_pObjResult->m_objPimples.m_strLeft,
                              m_pObjResult->m_objPimples.m_strJaw);
}

void CFaceDetecter::detectBlackheads(const cv::Mat &srcMat)
{
    findFaceSpots(m_strImageName.toStdString(), srcMat,
                  m_bHasFace, m_vectorFace, TYPE_BLACKHEADS, m_pObjResult);
    m_pDetailBlackheads->setImagePath(m_pObjResult->m_objBlackheads.m_strImgPath);
    m_pDetailBlackheads->setData(m_pObjResult->m_objBlackheads.m_strCounts);
}

void CFaceDetecter::detectFaceColor(const cv::Mat &srcMat)
{
    getFaceColorType(m_strImageName.toStdString(), srcMat, m_bHasFace, m_rectFace, m_pObjResult);
    m_pDetailFaceColor->setImagePath(m_pObjResult->m_objFaceColor.m_strImgPath);
    m_pDetailFaceColor->setData(m_pObjResult->m_objFaceColor.m_strColorType,
                                m_pObjResult->m_objFaceColor.m_strColorValue);
}

void CFaceDetecter::detectPore(const cv::Mat &srcMat)
{
    findFaceSpots(m_strImageName.toStdString(), srcMat,
                  m_bHasFace, m_vectorFace, TYPE_PORE, m_pObjResult);
    m_pDetailPore->setImagePath(m_pObjResult->m_objPore.m_strImgPath);
    m_pDetailPore->setData(m_pObjResult->m_objPore.m_strPoreType, m_pObjResult->m_objPore.m_strLeft,
                           m_pObjResult->m_objPore.m_strRight);
}

void CFaceDetecter::detectCoarseness(const cv::Mat &srcMat)
{
    cv::Rect rect;
    if (m_bHasFace) {
        rect = cv::Rect(m_vectorFace[INDEX_CONTOUR_FACE][0], m_vectorFace[INDEX_CONTOUR_FACE][2]);
    }
    getFaceCoarseness(m_strImageName.toStdString(), srcMat, m_bHasFace, rect, m_pObjResult);
    m_pDetailCoarse->setImagePath(m_pObjResult->m_objCoarse.m_strImgPath);
    m_pDetailCoarse->setData(m_pObjResult->m_objCoarse.m_strCoarseType, m_pObjResult->m_objCoarse.m_strValue);
}

void CFaceDetecter::showListItems(const enumItemType type)
{
    if (m_pStacked->isHidden()) {
        m_pStacked->show();
    }
    if (type & TYPE_PIMPLES) {
        if (m_pListWidget->item(ROW_PIMPLES)->isHidden()) {
            m_pListWidget->item(ROW_PIMPLES)->setHidden(false);
        }
    }
    if (type & TYPE_BLACKHEADS) {
        if (m_pListWidget->item(ROW_BLACKHEADS)->isHidden()) {
            m_pListWidget->item(ROW_BLACKHEADS)->setHidden(false);
        }
    }
    if (type & TYPE_FACE_COLOR) {
        if (m_pListWidget->item(ROW_FACE_COLOR)->isHidden()) {
            m_pListWidget->item(ROW_FACE_COLOR)->setHidden(false);
        }
    }
    if (type & TYPE_PORE) {
        if (m_pListWidget->item(ROW_PORE)->isHidden()) {
            m_pListWidget->item(ROW_PORE)->setHidden(false);
        }
    }
    if (type & TYPE_COARSENESS) {
        if (m_pListWidget->item(ROW_COARSENESS)->isHidden()) {
            m_pListWidget->item(ROW_COARSENESS)->setHidden(false);
        }
    }
}
