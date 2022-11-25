/**
 * @file            ddispatcher.h
 * @ingroup         AdvancedServiceFramework Framework Library
 * @author          DiWang
 * @date            2016-6-28
 * This file defined dispatcher class.
 */
#ifndef DDISPATCHER_H
#define DDISPATCHER_H

#include "dobject.h"
#include "dqueuethread.h"

namespace ding
{
class DDispatcherPrivate;
class DDispatcher : public DQueueThread<DParam>
{
    DECLARE_PRIVATE(DDispatcher)
    DISABLE_COPY(DDispatcher)
    DISABLE_MOVE(DDispatcher)
public:
    DDispatcher();
    virtual ~DDispatcher() override;

    using OutFunc = void (DDispatcher::*)(DParam& param);
    template <uint32_t Index>
    void output(DParam& param)
    {
        emitSignal(&DDispatcher::output<Index>, param);
    }

    bool addRoutePath(std::unique_ptr<DParam> &&msg, OutFunc signal);

protected:
    virtual int processMsg(DParam &msg) override;
};

} // namespace ding

#endif // DDISPATCHER_H
