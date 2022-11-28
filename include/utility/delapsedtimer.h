#ifndef DELAPSEDTIMER_H
#define DELAPSEDTIMER_H

#include <chrono>
#include <x86intrin.h>

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
#ifdef __x86_64__
class DElapsedTimer2
{
public:
    explicit DElapsedTimer2(uint32_t freq, bool run = true)
    {
        _freq = freq;//freq is MHz
        if(run)
            reset();
    }
    void reset()
    {
        _start = __rdtscp(&_aux);
    }
    uint64_t elapsed()
    {
        return (__rdtscp(&_aux) - _start)*1000/_freq;//nanoseconds
    }
private:
    uint64_t _start;
    uint32_t _aux;
    uint32_t _freq;
};
#endif

}  // namespace ding

#endif // DELAPSEDTIMER_H
