#include "ddispatcher.h"
#include "dcyclicthread_p.h"
#include "dutility.h"
#include <vector>

namespace ding
{
/*!
    \class ding::DDispatcher
    \brief The DDispatcher class dispatches messages.
    \ingroup core
    \inmodule dingLib

    The DDispatcher class dispatches messages. It is the core of message processing.
    It receives messages from slot input, and transmits the message to different
    destination.

    DDispatcher dispatches messages based on different message paths. A message path
    is consist of standard message and the related output signal. The messages from
    input() will be compared with all standard messages, and sent over the equal
    message path.

    Message path is configured outside. User can add a path through addMessagePath().

    Signal bypass() is emitted when a message is received. It is useful for simulation
    toolkit.

    As input() may be triggered by different threads, semaphores are used to make it
    thread safe.
*/

// class DRoutePath
struct DRoutePath
{
    DParam* msg;
    DDispatcher::OutFunc signal;
    DRoutePath(): msg(nullptr), signal(nullptr){}
    ~DRoutePath()
    {
        if (msg)
            delete msg;
    }
    bool empty()const
    {
        return !msg;
    }
};

// class DDispatcherPrivate
class DDispatcherPrivate : public DCyclicThreadPrivate
{
public:
    DECLARE_PUBLIC(DDispatcher)
    DISABLE_COPY(DDispatcherPrivate)
    DDispatcherPrivate() = default;
    virtual ~DDispatcherPrivate() = default;

    inline const DRoutePath* find(const DParam &param) const
    {
        for(int i = 0; i < MAX_SIGNAL_NUM; ++i)
        {
            if (!m_paths[i].msg)
                return nullptr;
            else if (m_paths[i].msg->equals(param))
                return &m_paths[i];
        }
        return nullptr;
    }
    inline bool contains(const DParam &param) const
    {
        return find(param) != nullptr;
    }
    inline DDispatcher::OutFunc route(const DParam &param) const
    {
        auto ret = find(param);
        if (ret)
            return ret->signal;
        return nullptr;
    }
    inline bool addPath(DParam* msg, DDispatcher::OutFunc signal)
    {
        if (!msg || !signal)
            return false;
        for (int i = 0; i < MAX_SIGNAL_NUM; ++i)
        {
            if (m_paths[i].empty())
            {
                m_paths[i].msg = msg;
                m_paths[i].signal = signal;
                return true;
            }
        }
        return false;
    }
    DRoutePath m_paths[MAX_SIGNAL_NUM];
    uint8_t m_pathIndex;
};

// class DDispatcher

/*!
    Constructs a DDispatcher object.
    There should be only one dispatcher in the framework.
 */
DDispatcher::DDispatcher()
    : DQueueThread(__func__, *new DDispatcherPrivate)
{
    D_D(DDispatcher);
    if (d)
        d->m_pathIndex = 0;
}

/*!
    Destroys a DDispatcher instance.
    The message loop will be stopped, and the unprocessed messages
    will be destroyed.
 */
DDispatcher::~DDispatcher() = default;

/*!
    Add message path with standard message \a msg and the related \a signal.
    Returns \c true if no error, otherwise returns \c false.
 */
bool DDispatcher::addRoutePath(DParam *msg, OutFunc signal)
{
    bool ret = false;
    D_D(DDispatcher);
    if (d && msg && !d->contains(*msg))
    {
        ret = addSignal(string_format("RoutePath%1",d->m_pathIndex++).c_str(), signal);
        if (ret)
            d->addPath(msg, signal);
    }
    return ret;
}

/*!
    \internal
    Route and emit related signals.
 */
int DDispatcher::processMsg(DParam &msg)
{
    D_D(DDispatcher);
    if (!d)
        return -1;
    auto signal = d->route(msg);
    
    if (signal)
        (this->*signal)(msg);

    return 0;
}

} // namespace ding
