#ifndef DLOGGER_P_H_
#define DLOGGER_P_H_

#include "dcyclicthread_p.h"
#include "dlogger.h"

namespace ding
{
class DLoggerPrivate : public DCyclicThreadPrivate
{
public:
    DECLARE_PUBLIC(DLogger)
    DISABLE_COPY(DLoggerPrivate)
    DISABLE_MOVE(DLoggerPrivate)
    DLoggerPrivate();
    virtual ~DLoggerPrivate() override = default;

private:
    std::string m_moduleName;
    DLogMsg::MsgLevel m_outputLevel;
    std::function<void(const DLogMsg &)> m_defaultProcessor;
};

} // namespace ding

#endif /* DLOGGER_P_H_ */
