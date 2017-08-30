/******************************************************************************
 * Copyright(c) 2015 Daboo Technology Co.,Ltd.
 *
 * @FileName:    keyvalue.h
 * @Author:      pengxuejin
 * @Version:     1.4
 * @Date:        2015.8.4
 * @Description: http请求需要发送到服务器的数据格式化处理辅助类
 * @Others:
 * @History:
 *     1、Date:         2015.8.4
 *        Author:       pengxuejin
 *        Modification: 基本功能添加
 *     2、Date:         2015.8.17
 *        Author:       pengxuejin
 *        Modification: 给出一个查找某个key对应字符串的接口以供日志上传模块使用
 *     3、Date:         2015.8.19
 *        Author:       pengxuejin
 *        Modification: 头文件中模块说明添加
 *     4、Date:         2015.8.20
 *        Author:       pengxuejin
 *        Modification: 代码小调整
*****************************************************************************/
#ifndef KEYVALUE_H
#define KEYVALUE_H

#include <map>
#include <QString>
#include "YLW_vs_char_set.h"

typedef std::map<QString, QString> MapQStringtoQString;

class KeyValue
{
public:
    KeyValue();

    void putString(const QString &strKey, const QString &strValue);
    void setConnector(const QString &strConnector);
    void setDelimiter(const QString &strDelimiter);
    QString toQString();
    QString getStringValue(const QString &strKey);

private:
    QString m_strConnector;
    QString m_strDelimiter;
    MapQStringtoQString m_mapData;
};

#endif // KEYVALUE_H
