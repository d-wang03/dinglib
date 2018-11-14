#ifndef DCYCLICTHREAD_H
#define DCYCLICTHREAD_H

#include "dobject.h"
#include "drunnable.h"
#include <future>

namespace ding
{
class DCyclicThreadPrivate;
class DCyclicThread : public DObject, public DRunnable
{
    DISABLE_COPY(DCyclicThread)
    DISABLE_MOVE(DCyclicThread)
    DECLARE_PRIVATE(DCyclicThread)
public:
    virtual ~DCyclicThread() override;

    // operations
    virtual bool start() override;
    virtual bool stop() override;
    bool isRunning() const;
    std::shared_future<int> getFuture();

protected:
    DCyclicThread(const std::string &type) noexcept;
    DCyclicThread(const std::string &type, DCyclicThreadPrivate &dd);

    // additional stop steps
    virtual void additionalStopSteps() = 0;
    // thread functions
    virtual void initialize() = 0;
    virtual int cycle() = 0;
    virtual void destroy() = 0;
};

} // namespace ding

#endif // DCYCLICTHREAD_H
