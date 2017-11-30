
#ifndef __CURE_LOG__H
#define __CURE_LOG__H

#include <glog/logging.h>

#undef ERROR // 因为glog中也定义了ERROR，避免冲突，关闭windows的

class CureLog
{
public:
    CureLog();
    ~CureLog();
};

#endif // __CURE_LOG__H

