#ifndef DELAPSEDTIMER_H
#define DELAPSEDTIMER_H

#include <chrono>

namespace ding
{

class DElapsedTimer
{
public:
    explicit DElapsedTimer(bool run = true)
    {
        if(run)
            reset();
    }
    void reset()
    {
        _start = std::chrono::steady_clock::now();
    }
    template<typename Durtion = std::chrono::milliseconds>
    int64_t elapsed()const
    {
        return std::chrono::duration_cast<Durtion>(
                   std::chrono::steady_clock::now() - _start).count();
    }
private:
    std::chrono::steady_clock::time_point _start;
};

}  // namespace ding

#endif // DELAPSEDTIMER_H
