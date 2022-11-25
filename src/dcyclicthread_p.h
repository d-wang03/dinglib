#ifndef DCYCLICTHREAD_P_H
#define DCYCLICTHREAD_P_H

#include "dobject_p.h"
#include "dcyclicthread.h"

#include <atomic>
#include <future>

namespace ding
{
class DCyclicThreadPrivate : public DObjectPrivate
{
public:
    DECLARE_PUBLIC(DCyclicThread)
    DISABLE_COPY(DCyclicThreadPrivate)
    DCyclicThreadPrivate();
    virtual ~DCyclicThreadPrivate() override;

    std::atomic<bool> m_bRunning;
    std::future<int> m_taskReturn;
};

} // namespace ding

#endif // DCYCLICTHREAD_P_H
