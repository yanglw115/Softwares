#include "Configuration.h"

#include <QDir>
#include <QApplication>
#include <QtDebug>

CConfiguration::CConfiguration(const QString &filePath)
{
    if(!filePath.isEmpty())
    {
        m_filePath = filePath;
    }
    m_pFile = NULL;
    m_pDoc = NULL;
}
CConfiguration::~CConfiguration()
{
    if(m_pFile != NULL)
        delete m_pFile;
    if(m_pDoc != NULL)
        delete m_pDoc;

    m_pFile = 0;
    m_pDoc = 0;
}

//初始化dom
bool CConfiguration::DomInit()
{
    if(m_pFile == NULL)
    {
        if(m_filePath.isEmpty())
            m_filePath = QApplication::applicationDirPath() + "/Config.xml";
        QFile tempfile;
        if(!tempfile.exists(m_filePath))
        {
//            qFatal("The config file(%s) not exist!", m_filePath);
            return false;
        }
        m_pFile = new QFile(m_filePath);
    }

    if(m_pDoc == NULL)
    {
        m_pDoc = new QDomDocument();
        if(m_pDoc->setContent(m_pFile))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return true;
}

//获取xml指定信息
QString CConfiguration::GetConfiguration(QString node, QString key)
{
    if(!this->DomInit())
    {
        return "";
    }

    QDomNodeList nodeList = this->m_pDoc->elementsByTagName("Configuration").at(0).childNodes();
    for(int i = 0 ; i < nodeList.count() ; i ++)
    {
        if(nodeList.at(i).nodeName() == node)
        {
            QDomNodeList valueList = nodeList.at(i).childNodes();
            for(int j = 0 ; j < valueList.count() ; j ++)
            {
                if(valueList.at(j).toElement().attribute("Key") == key)
                {
                    return valueList.at(j).toElement().text();
                }
            }
        }
    }
    return "";
}
