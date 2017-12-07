

#include "log.h"
#include <string>

#ifdef __linux
#include "version.h"
#endif


using namespace std;
CureLog::CureLog()
{
#ifdef __linux
	string strLogFilePath = "/usr/local/FaceParser/logs/FaceParserJni.log";
#else
    string strLogFilePath = "D:\\CureFaceParser\\logs\\FaceParserJni.log";
#endif

    FLAGS_log_dir = strLogFilePath;
    FLAGS_colorlogtostderr = true;
    FLAGS_logtostderr = false;
    FLAGS_logbufsecs = 0;
    FLAGS_max_log_size = 200; // 200M Max
    FLAGS_stop_logging_if_full_disk = true;

    google::InitGoogleLogging("");
    google::SetLogDestination(google::GLOG_INFO, strLogFilePath.c_str());
    google::SetLogDestination(google::GLOG_WARNING, strLogFilePath.c_str());
    google::SetLogDestination(google::GLOG_ERROR, strLogFilePath.c_str());
    google::SetLogDestination(google::GLOG_FATAL, strLogFilePath.c_str());
    //google::SetStderrLogging(google::GLOG_ERROR);
    google::SetLogFilenameExtension("");
    google::SetLogAppName("iFace");

#ifdef __linux
	LOG(INFO) << "###Log system start, library version: " << nVersion;
#else
	LOG(INFO) << "###Log system start...... " << nVersion;
#endif
}


CureLog::~CureLog()
{
	LOG(INFO) << "###Log system finish.";
    google::ShutdownGoogleLogging();
}
