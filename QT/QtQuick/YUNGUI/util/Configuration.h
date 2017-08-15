#ifndef CCONFIGURATION_H
#define CCONFIGURATION_H

/*---------读取配置文件------------*/

#include <QString>
#include <QFile>
#include <QDomDocument>


class CConfiguration
{
public:
    CConfiguration(const QString &filePath);
    ~CConfiguration();

    QString GetConfiguration(QString node,QString key);//读取配置文件指定节点的信息
private:
    bool DomInit();//初始化dom
private:
    QString m_filePath;
    QFile *m_pFile;
    QDomDocument *m_pDoc;
};

#endif
