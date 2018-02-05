#ifndef COBJECTRESULT_H
#define COBJECTRESULT_H

#include <QObject>

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
        CPimples &operator = (const CPimples &obj)
        {
            m_strImgPath = obj.m_strImgPath;
            m_strLeft = obj.m_strLeft;
            m_strRight = obj.m_strRight;
            m_strForehead = obj.m_strForehead;
            m_strJaw = obj.m_strJaw;
            m_strNose = obj.m_strNose;
        }
    };
    class CBlackheads
    {
    public:
        QString m_strImgPath;
        QString m_strCounts;
        CBlackheads &operator = (const CBlackheads &obj)
        {
            m_strImgPath = obj.m_strImgPath;
            m_strCounts = obj.m_strCounts;
        }
    };
    class CFaceColor
    {
    public:
        QString m_strImgPath;
        QString m_strColorType;
        CFaceColor &operator = (const CFaceColor &obj)
        {
            m_strImgPath = obj.m_strImgPath;
            m_strColorType = obj.m_strColorType;
        }
    };
    class CPore
    {
    public:
        QString m_strImgPath;
        QString m_strLeft;
        QString m_strRight;
        QString m_strPoreType;
        CPore &operator = (const CPore &obj)
        {
            m_strImgPath = obj.m_strImgPath;
            m_strLeft = obj.m_strLeft;
            m_strRight = obj.m_strRight;
            m_strPoreType = obj.m_strPoreType;
        }
    };
    class CCoarseness
    {
    public:
        QString m_strImgPath;
        QString m_strValue;
        QString m_strCoarseType;
        CCoarseness &operator = (const CCoarseness &obj)
        {
            m_strImgPath = obj.m_strImgPath;
            m_strValue = obj.m_strValue;
            m_strCoarseType = obj.m_strCoarseType;
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
