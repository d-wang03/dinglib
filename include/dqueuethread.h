#ifndef DQUEUETHREAD_H
#define DQUEUETHREAD_H

#include "dcyclicthread.h"
#include "utility/dsyncqueue.h"

namespace ding
{
template <typename TMsg, typename = std::enable_if_t<std::is_base_of<DParam, std::decay_t<TMsg>>::value>>
class DQueueThread : public DCyclicThread
{
    DISABLE_COPY(DQueueThread)
    DISABLE_MOVE(DQueueThread)
    SIGNAL(DQueueThread,bypass)
public:
    virtual ~DQueueThread() override = default;

    // slot input
    void input(DParam &param)
    {
        if (!param.is_derived_from<TMsg>())
            return;
        m_queue.push_back(static_cast<TMsg &>(param));
        bypass(param);
    }

    virtual inline void addSignals()override
    {
        DObject::addSignals();
        ADD_SIGNAL(DQueueThread, bypass);
    }

protected:
    DQueueThread(const char *type)
        : DCyclicThread(type)
    {
    }
    DQueueThread(const char *type, DCyclicThreadPrivate &dd)
        : DCyclicThread(type, dd)
    {
    }

    // operations
    virtual void additionalStopSteps() override { m_queue.push_back(TMsg()); }
    virtual int cycle() override
    {
        int ret = 0;
        auto pMsg = m_queue.pop_front();
        if (pMsg != nullptr)
            ret = processMsg(*pMsg);
        return ret;
    }

    // thread functions, must be implemented by derived classes.
    virtual void initialize() override {}
    virtual void destroy() override {}
    virtual int processMsg(TMsg &msg) = 0;

private:
    DSyncQueue<TMsg> m_queue;
};

} // namespace ding
#endif // DQUEUETHREAD_H
