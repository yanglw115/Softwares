// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <windows.h>
#undef ERROR // 因为glog中也定义了ERROR，避免冲突，关闭windows的

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "HWHttpClient.h"
#include "HWUpdateApp.h"

// TODO: 在此处引用程序需要的其他头文件
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include "HWLog.h"
#include "HWSingleton.h"

/* 日志不输出到终端 */
#pragma comment(linker,"/subsystem:\"Windows\" /entry:\"mainCRTStartup\"")
