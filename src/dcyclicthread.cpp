#include "dcyclicthread.h"
#include "dcyclicthread_p.h"

using namespace std::literals;

namespace ding
{
/*!
    \class ding::TCyclicThread
    \brief This class defines cyclic thread.
    \ingroup core
    \inmodule ding

    Cyclic thread is widely used in ding library, and user defined modules,
    e.g. drivers, event router, and various service tasks.

    There also exists many implementations for cyclic threads, from basic system API to 3rd party components.

    Consider cross platform and minimizing porting effort, ding library implements its own cyclic thread
    template by STL(C++11).
    Drivers, dispatcher, loggers... are derived from this template.

    In this class, thread is started after start() called, and running flag change to true.

    In the thread, virtual protected initialize() will be called before loop.
    Then in the loop, virtual protected cycle() is called each time.
    The only way to jump out of loop is calling stop().
    And then, virtual protected destroy() is called.

    When stop() is called, it changes the running flag, and call virtual protected additionalStopSteps().
    This method is useful in some situation, that the cycle() is pending to wait for some message.
    In this case, use this method to unlock the cycle(), otherwise the thread will not exit.

    \note The stop() is not called in dtor, for it should be called in dtor of derived classes.
*/

DCyclicThreadPrivate::DCyclicThreadPrivate()
    : DObjectPrivate()
    , m_bRunning(false)
{
}
DCyclicThreadPrivate::~DCyclicThreadPrivate() = default;

// class TCyclicThread
DCyclicThread::DCyclicThread(const char *type) noexcept
    : DObject(type, *new DCyclicThreadPrivate)
{
}

DCyclicThread::DCyclicThread(const char *type, DCyclicThreadPrivate &dd)
    : DObject(type, dd)
{
}

DCyclicThread::~DCyclicThread() = default;

/*!
 * Start the cyclic thread.
 *
 * Returns \c true if success. It will fail only if the thread is already running.
 */
bool DCyclicThread::start()
{
    D_D(DCyclicThread);
    if (d.m_bRunning || (d.m_taskReturn.valid() && d.m_taskReturn.wait_for(0s) == std::future_status::timeout))
    {
        loggingW("Thread is already running.");
        return false;
    }

    d.m_bRunning = true;
    d.m_taskReturn = std::async(std::launch::async, [this]() {
        int ret = 0;
        initialize();
        while (d_func().m_bRunning)
        {
            ret = cycle();
        }
        destroy();
        return ret;
    });
    loggingD("Thread %1 is started.", getTypeName());
    return true;
}

/*!
 * Stop the cyclic thread.
 *
 * It will wait up to 2 seconds for the thread termination.
 * If thread will not exit in 2 seconds, this method returns \c false.
 *
 * If the thread has already stopped, returns \c false.
 */
bool DCyclicThread::stop()
{
    D_D(DCyclicThread);
    if (d.m_bRunning)
    {
        d.m_bRunning = false;

        if (d.m_taskReturn.valid())
        {
            additionalStopSteps();
            if (d.m_taskReturn.wait_for(2s) != std::future_status::ready)
            {
                loggingE("Thread %1 can not terminated.", getTypeName());
                return false;
            }
        }
        loggingD("Thread %1 is stopped.", getTypeName());
        return true;
    }
    else
    {
        loggingW("Thread %1 is already stopped.", getTypeName());
        return false;
    }
}

/*!
 * Returns \c true if thread is running.
 */
bool DCyclicThread::isRunning() const
{
    D_D_CONST(DCyclicThread);
    return d.m_bRunning;
}

/*!
 * Returns \c std::shared_future<int> of the thread.
 */
std::shared_future<int> DCyclicThread::getFuture()
{
    D_D(DCyclicThread);
    return d.m_taskReturn.share();
}

/*!
  \fn TCyclicThread::additionalStopSteps()
  Called in stop(). Usually unlock cycle() if it is pending.
*/

/*!
  \fn TCyclicThread::initialize()
  Called in thread, before thread's loop.
*/

/*!
  \fn TCyclicThread::cycle()
  Called in thread's loop.
*/

/*!
  \fn TCyclicThread::destroy()
  Called post thread's loop.
*/
} // namespace ding
