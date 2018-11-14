#ifndef DLATEREXECUTE_H
#define DLATEREXECUTE_H

#include <functional>
#include <chrono>
#include <future>

namespace ding
{

template <typename callable,
          typename... arguments>
void later_execute(unsigned after, bool async, callable&& f, arguments&&... args)
{
    std::function<typename std::result_of<callable(arguments...)>::type()>
    task(std::bind(std::forward<callable>(f),
                   std::forward<arguments>(args)...));

    if(async)
    {
        std::thread([after, task]()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(after));
            task();
        }).detach();
    }
    else
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(after));
        task();
    }
}

template <typename callable,
          typename... arguments>
void later_execute(unsigned after, int repeat, bool async, std::shared_future<void>&& sf,
                   callable&& f, arguments&&... args)
{
    std::function<typename std::result_of<callable(arguments...)>::type()>
    task(std::bind(std::forward<callable>(f),
                   std::forward<arguments>(args)...));

    if(async)
    {
        std::thread([after, repeat, task, sf]()
        {
            while(sf.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
            {
                static int nTimes = 0;
                std::this_thread::sleep_for(std::chrono::milliseconds(after));
                task();
                if(repeat > 0 && nTimes++ >= repeat)
                    break;

            }
        }).detach();
    }
    else
    {
        while(sf.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
        {
            static int nTimes = 0;
            std::this_thread::sleep_for(std::chrono::milliseconds(after));
            task();
            if(repeat > 0 && nTimes++ >= repeat)
                break;
        }
    }
}

} // namespace ding

#endif // DLATEREXECUTE_H
