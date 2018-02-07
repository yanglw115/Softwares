#ifndef CFACEDETECTER_H
#define CFACEDETECTER_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include <QPixmap>
#include <QStackedWidget>

#include "vs_charset.h"
#include "CResultDetail.h"
#include "CObjectResult.h"
#include "faceDetect/faceLandmarkDetect.h"
#include "faceDetect/faceSpotsDetect.h"
#include "faceDetect/faceColor.h"
#include "faceDetect/faceTamura.h"

enum enumRowItem {
    ROW_PIMPLES = 0,
    ROW_BLACKHEADS,
    ROW_FACE_COLOR,
    ROW_PORE,
    ROW_COARSENESS
};

class CFaceDetecter: public QWidget
{
public:
    CFaceDetecter(QWidget * parent = 0, Qt::WindowFlags f = 0);
    virtual ~CFaceDetecter();
    void startDetect(const QString &strImgPath, const enumItemType type, QWidget *pWidgetMain);
    CObjectResult& getObjResultRef() { return *m_pObjResult; }

private:
    void initWindow();
    void detectAll(const cv::Mat &srcMat);
    void detectPimples(const cv::Mat &srcMat);
    void detectBlackheads(const cv::Mat &srcMat);
    void detectFaceColor(const cv::Mat &srcMat);
    void detectPore(const cv::Mat &srcMat);
    void detectCoarseness(const cv::Mat &srcMat);
    void showListItems(const enumItemType type);
private:
    vectorContours m_vectorFace;
    bool m_bHasFace;
    cv::Rect m_rectFace;
    QString m_strImageName;
    QString m_strImagePath;
    CObjectResult *m_pObjResult;
    QHBoxLayout *m_pHLayoutMain;
    QVBoxLayout *m_pVLayoutLeft;
    QVBoxLayout *m_pVLayoutRight;
    QLabel *m_pLabelResult;
    QLabel *m_pLabelDetails;
    QFrame *m_pLineLeft;
    QFrame *m_pLineRight;
    QFrame *m_pLineMiddle;
    QListWidget *m_pListWidget;
    QStackedWidget *m_pStacked;
    CResultDetail *m_pDetailPimples;
    CResultDetail *m_pDetailBlackheads;
    CResultDetail *m_pDetailFaceColor;
    CResultDetail *m_pDetailPore;
    CResultDetail *m_pDetailCoarse;
};

#endif // CFACEDETECTER_H
