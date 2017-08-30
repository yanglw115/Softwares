#include "YLW_key_value.h"

KeyValue::KeyValue()
{

}

void KeyValue::putString(const QString &strKey, const QString &strValue)
{
    if (strKey.isEmpty() || strKey.isNull()) {
        return;
    }

    m_mapData[strKey]= strValue;
}

void KeyValue::setConnector(const QString &strConnector)
{
    m_strConnector = strConnector;
}

void KeyValue::setDelimiter(const QString &strDelimiter)
{
    m_strDelimiter = strDelimiter;
}

QString KeyValue::toQString()
{
    QString keyValue;

    std::map<QString, QString>::iterator it = m_mapData.begin();
    for (; it != m_mapData.end(); ++it) {
        keyValue += it->first;
        keyValue += m_strConnector;
        keyValue += it->second;
        keyValue += m_strDelimiter;
    }

    if (!keyValue.isEmpty() && !keyValue.isNull()
            && !m_strDelimiter.isEmpty() && !m_strDelimiter.isNull()) {
        keyValue = keyValue.left(keyValue.length() - m_strDelimiter.length());
    }

    return keyValue;
}

QString KeyValue::getStringValue(const QString &strKey)
{
    /* find by key */
    std::map<QString, QString>::iterator it = m_mapData.find(strKey);
    if (m_mapData.end() != it) {
        return it->second;
    } else {
        return QString("");
    }
}
