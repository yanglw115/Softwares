#include "StdAfx.h"
#include "HWLog.h"

#include <string>

#define PATH_BUFFER_SIZE MAX_PATH

HWLog::HWLog()
{
    char buffer[PATH_BUFFER_SIZE] = {0};
    string strLogFileDir;
    string strLogFilePath;
    if (GetCurrentDirectory(PATH_BUFFER_SIZE, buffer) > 0) {
        strLogFileDir = string(buffer);
        strLogFileDir += "\\..\\share\\log";
        strLogFilePath = strLogFileDir + "\\Measure.log";
    }

    FLAGS_log_dir = strLogFilePath;
    FLAGS_colorlogtostderr = true;
    FLAGS_logtostderr = false;
    FLAGS_logbufsecs = 0;
    FLAGS_max_log_size = 3;
    FLAGS_stop_logging_if_full_disk = true;

    google::InitGoogleLogging("");
    google::SetLogDestination(google::GLOG_INFO, strLogFilePath.c_str());
    google::SetLogDestination(google::GLOG_WARNING, strLogFilePath.c_str());
    google::SetLogDestination(google::GLOG_ERROR, strLogFilePath.c_str());
    google::SetLogDestination(google::GLOG_FATAL, strLogFilePath.c_str());
    google::SetStderrLogging(google::GLOG_ERROR);
    google::SetLogFilenameExtension("");
    google::SetLogAppName("app1");
}


HWLog::~HWLog()
{
    google::ShutdownGoogleLogging();
}
