#ifndef DLOGGER_H_
#define DLOGGER_H_

#include "dqueuethread.h"
#include "dlogmsg.h"

namespace ding
{
#ifdef D_OS_ANDROID
#include <android/log.h>
inline void _androidLogProcess(const DLogMsg &msg)
{
    android_LogPriority priority;
    switch (msg.getLevel())
    {
    case DLogMsg::Debug:
        priority = ANDROID_LOG_DEBUG;
        break;
    case DLogMsg::Info:
        priority = ANDROID_LOG_INFO;
        break;
    case DLogMsg::Warning:
        priority = ANDROID_LOG_WARN;
        break;
    case DLogMsg::Error:
        priority = ANDROID_LOG_ERROR;
        break;
    default:
        return;
    }
    __android_log_write(priority, "ding", msg.toString().c_str());
}
#endif

class DLoggerPrivate;
class DLogger : public DQueueThread
{
    DECLARE_PRIVATE(DLogger)
    DISABLE_COPY(DLogger)
    DISABLE_MOVE(DLogger)
public:
    DLogger();
    DLogger(const std::string &modualName, DLogMsg::MsgLevel outputLevel = DLogMsg::Info);
    virtual ~DLogger() override;

    void setModuleName(const std::string &name);
    std::string getModuleName()const;
    void setOutputLevel(DLogMsg::MsgLevel level);
    DLogMsg::MsgLevel getOutputLevel()const;
    void setDefaultProcessor(std::function<void(const DLogMsg &)> &&processor);

protected:
    virtual int processMsg(DParam &msg) override;
    DLogger(const char *type, DCyclicThreadPrivate &dd);
};

} // namespace ding

#endif /* DLogger_H_ */
