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
    template<typename Durtion = std::chrono::nanoseconds>
    int64_t elapsed()const
    {
        return std::chrono::duration_cast<Durtion>(
                   std::chrono::steady_clock::now() - _start).count();
    }
private:
    std::chrono::steady_clock::time_point _start;
};
#ifdef __x86_64__
#include <x86intrin.h>
#include <cstdio>
class DElapsedTimer2
{
public:
    explicit DElapsedTimer2(uint32_t freq, bool run = true)
    {
        // _freq = freq;//freq is MHz
        _freq = DElapsedTimer2::getFreq();
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
    uint64_t elapsedClks()
    {
        return (__rdtscp(&_aux) - _start);
    }

    static uint64_t getFreq()
    {
        FILE *fp = nullptr;
        char cycleStr[16] = {0};
        fp = popen("cat /proc/cpuinfo | grep MHz | uniq | cut -d: -f2","r");
        if (!fp)
            return 0;
        fgets(cycleStr, 16, fp);
        fclose(fp);
        return (uint64_t)atof(cycleStr);
    }
private:
    uint64_t _start;
    uint32_t _aux;
    uint32_t _freq;
};
#endif

}  // namespace ding

#endif // DELAPSEDTIMER_H
