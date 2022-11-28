#include "dlogger.h"
#include "dlogger_p.h"
#include <iostream>
#include <fstream>

namespace ding
{
/*!
    \class ding::DLogger
    \brief The DLogger class prints DLogMsg.
    \ingroup logging
    \inmodule ding

    It is also the base class for logging system.
    Logging messages are sent to input() slot.
    Then the messages are copied to an internal synchronous queue.
    A logging thread prints the messages in queue.

    In the initial version of DLogger, the messages are printed immediantly.
    It is fast and inplace.
    However, the calling thread will be pending on the print, thus not suitable for some critical scenarios.
    Further more, if the message is shared between threads, the print operation may cause concurrency issues.

    The logging thread avoid the above sideeffects.
    No pending, no competition.
    Messages may be printed later, but the timestamps of the message is not affected.
    Bescuse they are set in message's ctor, thus no matter when and how the message is printed.

    All loggers should inherit DLogger, and re-implement \l processMsg().

    There should be only one instance in a single process, therefore the copy ctor and move ctor
    are deleted.

    Signal bypass() is used to make several loggers to be a logging chain.
    The log message will flow over the chain, and be processed by each logger.
*/

D_REGISTER_OBJECT_CLASS(DLogger);

DLoggerPrivate::DLoggerPrivate()
    : DCyclicThreadPrivate()
    , m_moduleName("")
    , m_outputLevel(DLogMsg::Info)
    , m_defaultProcessor(
          [this](const DLogMsg &msg) { std::cout << "[" << m_moduleName << "]:" << msg.toString() << std::endl; })
{
}

/*!
    Constructs an empty DLogger instance.
    \note It is NOT recommended to use this constructor.
 */
DLogger::DLogger()
    : DQueueThread(__func__, *new DLoggerPrivate)
{
}

/*!
    Constructs a DLogger instance with \a modualName and \a outputLevel.
    Log messages with lower level will not be processed.
    \note It is recommended to use this constructor.
 */
DLogger::DLogger(const std::string &modualName, DLogMsg::MsgLevel outputLevel)
    : DLogger()
{
    setModuleName(modualName);
    setOutputLevel(outputLevel);
}

/*!
    Constructs a DLogger instance with \a type and the private instance \a dd.
    This constructor is used for dynamic library compliance.
    \note It is NOT recommended to use this constructor.
 */
DLogger::DLogger(const char *type, DCyclicThreadPrivate &dd)
    : DQueueThread(type, dd)
{
}

/*!
    Destroys a DLogger instance.
 */
DLogger::~DLogger()
{
    if (isRunning())
        stop();
}
/*!
    This virtual function is to process log message \a msg.
    DLogger's process is outputting to stdout.
 */
int DLogger::processMsg(DParam &msg)
{
    if (!msg.is_derived_from<DLogMsg>())
        return -1;
    DLogMsg& logMsg = static_cast<DLogMsg&>(msg);
    if (logMsg.getLevel() >= getOutputLevel())
    {
        D_D(DLogger);
        if (d)
            d->m_defaultProcessor(logMsg);
    }
    return 0;
}

/*!
    Set \a name as module name.
 */
void DLogger::setModuleName(const std::string &name)
{
    D_D(DLogger);
    if (d)
        d->m_moduleName = name;
}

/*!
    Returns \c string contains module name.
 */
std::string DLogger::getModuleName()const
{
    D_D_CONST(DLogger);
    return d? d->m_moduleName : std::string();
}

/*!
    Set \a level as output level.
 */
void DLogger::setOutputLevel(DLogMsg::MsgLevel level)
{
    D_D(DLogger);
    if (d)
        d->m_outputLevel = level;
}

/*!
    Returns \c DLogMsg::MsgLevel contains output level.
 */
DLogMsg::MsgLevel DLogger::getOutputLevel()const
{
    D_D_CONST(DLogger);
    return d? d->m_outputLevel : DLogMsg::Debug;
}

/*!
    set the default \a processor to the message.

    If the programmer want to set his own processor to DLogger, use this method.
    Otherwise, DO NOT call this method.

    In default, the processor is set as below.
    \code
    [this](const DLogMsg &msg)
    {
        std::cout << "[" << m_moduleName << "]:" << msg.toString() << std::endl;
    }
    \endcode
 */
void DLogger::setDefaultProcessor(std::function<void(const DLogMsg &)> &&processor)
{
    D_D(DLogger);
    if (d)
        d->m_defaultProcessor = std::move(processor);
}

} // namespace ding
