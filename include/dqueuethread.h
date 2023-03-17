#ifndef DQUEUETHREAD_H
#define DQUEUETHREAD_H

#include "dcyclicthread.h"
#include "utility/dsyncqueue.h"
#include "utility/dlockfreequeue.h"

namespace ding
{
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
        if (enable_lockfree)
            m_lfqueue.push_back(param);
        else
            m_queue.push_back(param);
        bypass(param);
    }

    void setLockfree(bool flag)
    {
        enable_lockfree = flag;
    }

protected:
    DQueueThread(const char *type)
        : DCyclicThread(type), enable_lockfree(true)
    {
        ADD_SIGNAL(DQueueThread, bypass);
    }
    DQueueThread(const char *type, DCyclicThreadPrivate &dd)
        : DCyclicThread(type, dd), enable_lockfree(true)
    {
        ADD_SIGNAL(DQueueThread, bypass);
    }

    // operations
    virtual void additionalStopSteps() override { m_queue.push_back(DParam()); }
    virtual int cycle() override
    {
        int ret = 0;
        DParam* pMsg = enable_lockfree ? m_lfqueue.pop_front() : m_queue.pop_front();
        if (pMsg != nullptr)
            ret = processMsg(*pMsg);
        return ret;
    }

    // thread functions, must be implemented by derived classes.
    virtual void initialize() override {}
    virtual void destroy() override {}
    virtual int processMsg(DParam &msg) = 0;

private:
    DSyncQueue<DParam> m_queue;
    DLockFreeQueue<DParam> m_lfqueue;
    bool enable_lockfree;
};

} // namespace ding
#endif // DQUEUETHREAD_H
