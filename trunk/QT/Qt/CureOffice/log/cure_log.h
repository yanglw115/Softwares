/******************************************************************************
 * Copyright(c) 2015 Daboo Technology Co.,Ltd.
 *
 * @FileName: HW_log.h
 * @Author: yangliwei
 * @Version: 1.0
 * @Date: 2015.7.20
 * @Description: 应用程序日志系统。使用该日志系统，使qDebug/qWarnig/qCritical和
 *     qFatalt等API的输出定向到文件，便于通过文件的方式查看程序的运行日志。
 * @Others:
 * @History:
 *     1、Date: 2015.7.20
 *        Author: yangliwei
 *        Modification: 实现基本功能
*****************************************************************************/

#ifndef _HW_LOG_H_
#define _HW_LOG_H_

#include <QDateTime>
#include <QFile>
#include <QMutex>
#include <QTextStream>

class CMyLog
{
public:
    CMyLog();
    ~CMyLog();

    static void initLog();

    /**************************************************************************
     * Function: messageHandler
     * Description: 日志消息输出到文件的处理函数
     * Input: 日志类型、日志信息相关内容和日志内容
     * Output: 无
     * Return: 空
     *************************************************************************/
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    /**************************************************************************
     * Function: clearLogFile
     * Description: 当日志文件过大时，清空日志文件内容
     * Input: 无
     * Output: 无
     * Return: 空
     *************************************************************************/
    static void clearLogFile();

private:
    /**************************************************************************
     * Function: getFileName
     * Description: 解析获取日志文件全名，不包括路径
     * Input: 日志文件路径
     * Output: 无
     * Return: 日志文件全名
     *************************************************************************/
    static QString getFileName(const QString &strFileNamePath);

    /**************************************************************************
     * Function: getFunctionName
     * Description: 解析获取日志所在函数名称，不包括参数表
     * Input: 日志所在函数名全称
     * Output: 无
     * Return: 日志所在函数名，不包括参数表
     *************************************************************************/
    static QString getFunctionName(const QString &strFunctionNamePath);

    /**************************************************************************
     * Function: getAppPathName
     * Description: 解析获取当前是app1还是app2安装包内的程序
     * Input: 无
     * Output: 无
     * Return: 所在安装包的名称
     *************************************************************************/
    static QString getAppPathName();

private:
};

#endif // _HW_LOG_H_

