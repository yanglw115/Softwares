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
        QString m_strImgPath;
        QString m_strLeft;
        QString m_strRight;
        QString m_strForehead;
        QString m_strJaw;
        QString m_strNose;
    };
    class CBlackheads
    {
        QString m_strImgPath;
        QString m_strCounts;
    };
    class CFaceColor
    {
        QString m_strImgPath;
        QString m_strColorType;
    };
    class CPore
    {
        QString m_strImgPath;
        QString m_strLeft;
        QString m_strRight;
        QString m_strPoreType;
    };
    class CCoarseness
    {
        QString m_strImgPath;
        QString m_strValue;
        QString m_strCoarseType;
    };
    CPimples m_objPimples;
    CBlackheads m_objBlackheads;
    CFaceColor m_objFaceColor;
    CPore m_objPore;
    CCoarseness m_objCoarse;
};

#endif // COBJECTRESULT_H
