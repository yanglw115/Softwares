#ifndef COBJECTRESULT_H
#define COBJECTRESULT_H

#include <QObject>

#include "vs_charset.h"

class CObjectResult : public QObject
{
    Q_OBJECT
public:
    explicit CObjectResult(QObject *parent = 0);
    virtual ~CObjectResult();

signals:

public slots:
public:
    class CPimples
    {
    public:
        QString m_strImgPath;
        QString m_strLeft;
        QString m_strRight;
        QString m_strForehead;
        QString m_strJaw;
        QString m_strNose;
        double m_dMinAreaSize;
        double m_dMaxAreaSize;
        int m_nMinLColorValue;
        int m_nMinRGDiffValue;
        double m_dMinRatio;
        double m_dMaxRatio;
        CPimples &operator = (const CPimples &obj)
        {
            m_strImgPath = obj.m_strImgPath;
            m_strLeft = obj.m_strLeft;
            m_strRight = obj.m_strRight;
            m_strForehead = obj.m_strForehead;
            m_strJaw = obj.m_strJaw;
            m_strNose = obj.m_strNose;
            m_dMinAreaSize = obj.m_dMinAreaSize;
            m_dMaxAreaSize = obj.m_dMaxAreaSize;
            m_nMinLColorValue = obj.m_nMinLColorValue;
            m_nMinRGDiffValue = obj.m_nMinRGDiffValue;
            m_dMinRatio = obj.m_dMinRatio;
            m_dMaxRatio = obj.m_dMaxRatio;
        }
    };
    class CBlackheads
    {
    public:
        QString m_strImgPath;
        QString m_strCounts;
        double m_dMaxAreaSize;
        int m_nMaxColor;
        double m_dMinRatio;
        double m_dMaxRatio;
        CBlackheads &operator = (const CBlackheads &obj)
        {
            m_strImgPath = obj.m_strImgPath;
            m_strCounts = obj.m_strCounts;
            m_dMaxAreaSize = obj.m_dMaxAreaSize;
            m_nMaxColor = obj.m_nMaxColor;
            m_dMinRatio = obj.m_dMinRatio;
            m_dMaxRatio = obj.m_dMaxRatio;

        }
    };
    class CFaceColor
    {
    public:
        QString m_strImgPath;
        QString m_strColorType;
        QString m_strColorValue;
        int m_nTouBai;
        int m_nBaiXi;
        int m_nZiRan;
        int m_nXiaoMai;
        int m_nAnChen;
        int m_nYouHei;
        CFaceColor &operator = (const CFaceColor &obj)
        {
            m_strImgPath = obj.m_strImgPath;
            m_strColorType = obj.m_strColorType;
            m_strColorValue = obj.m_strColorValue;
            m_nTouBai = obj.m_nTouBai;
            m_nBaiXi = obj.m_nBaiXi;
            m_nZiRan = obj.m_nZiRan;
            m_nXiaoMai = obj.m_nXiaoMai;
            m_nAnChen = obj.m_nAnChen;
            m_nYouHei = obj.m_nYouHei;
        }
    };
    class CPore
    {
    public:
        QString m_strImgPath;
        QString m_strLeft;
        QString m_strRight;
        QString m_strPoreType;
        int m_nRough;
        int m_nNormal;
        CPore &operator = (const CPore &obj)
        {
            m_strImgPath = obj.m_strImgPath;
            m_strLeft = obj.m_strLeft;
            m_strRight = obj.m_strRight;
            m_strPoreType = obj.m_strPoreType;

            m_nRough = obj.m_nRough;
            m_nNormal = obj.m_nNormal;
        }
    };
    class CCoarseness
    {
    public:
        QString m_strImgPath;
        QString m_strValue;
        QString m_strCoarseType;
        double m_dRough;
        double m_dNormal;
        CCoarseness &operator = (const CCoarseness &obj)
        {
            m_strImgPath = obj.m_strImgPath;
            m_strValue = obj.m_strValue;
            m_strCoarseType = obj.m_strCoarseType;
            m_dRough = obj.m_dRough;
            m_dNormal = obj.m_dNormal;
        }
    };

    CPimples m_objPimples;
    CBlackheads m_objBlackheads;
    CFaceColor m_objFaceColor;
    CPore m_objPore;
    CCoarseness m_objCoarse;

    CObjectResult &operator = (const CObjectResult &obj)
    {
        m_objPimples = obj.m_objPimples;
        m_objBlackheads = obj.m_objBlackheads;
        m_objFaceColor = obj.m_objFaceColor;
        m_objPore = obj.m_objPore;
        m_objCoarse = obj.m_objCoarse;
    }
};

#endif // COBJECTRESULT_H
