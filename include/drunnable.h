#ifndef D_RUNNABLE_H
#define D_RUNNABLE_H

namespace ding
{
class DRunnable
{
public:
    virtual ~DRunnable() = default;
    virtual bool start() = 0;
    virtual bool stop() = 0;
};
} // namespace ding

#endif
