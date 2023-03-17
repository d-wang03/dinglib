#include <gtest/gtest.h>
#include <thread>
#include "utility/dlockfreequeue.h"
#include "utility/dsyncqueue.h"
#include "utility/delapsedtimer.h"
#include "dlogmsg.h"
#include "dnormalmsg.h"

using namespace ding;

void check(int range, int n, std::map<int, int>& counter1, std::map<int, int>& counter2)
{
    for (int i = 0; i < range; i++)
    {
        if (counter1[i] + counter2[i] != n)
            printf("%d\n",i);
        EXPECT_EQ(counter1[i] + counter2[i], n);
    }
}

TEST(DLockFreeQueue, ctor)
{
    DLockFreeQueue<DLogMsg, 8> queue;
}

TEST(DLockFreeQueue, perf)
{
    DLockFreeQueue<DNormalMsg> lfqueue;
    int times = 100000;
    std::map<int, int> consumer1;
    std::map<int, int> consumer2;
    std::atomic<int> producer(8);
    
    auto&& push = [&lfqueue, &producer, times]() {
        int i = 0;
        while (i < times)
        {
            auto ret = lfqueue.push(new DNormalMsg(std::to_string(i)));
            if (!ret)
            {
                std::this_thread::yield();
                continue;
            }
            i++;
        }
        --producer;
    };

    auto&& pop = [&lfqueue, &producer](std::map<int, int>& counter) {
        while (!lfqueue.empty() || producer > 0)
        {
            auto ret = lfqueue.pop();
            if (!ret)
            {
                std::this_thread::yield();
                continue;
            }
            ;
            counter[std::stoi(ret->toString())]++;
        }
    };

    DElapsedTimer timer;
    std::thread producerThread1(push);
    std::thread producerThread2(push);
    std::thread producerThread3(push);
    std::thread producerThread4(push);
    std::thread producerThread5(push);
    std::thread producerThread6(push);
    std::thread producerThread7(push);
    std::thread producerThread8(push);
    std::thread consumerThread1(pop, std::ref(consumer1));
    std::thread consumerThread2(pop, std::ref(consumer2));
    producerThread1.join();
    producerThread2.join();
    producerThread3.join();
    producerThread4.join();
    producerThread5.join();
    producerThread6.join();
    producerThread7.join();
    producerThread8.join();
    consumerThread1.join();
    consumerThread2.join();

    auto interval = timer.elapsed();
    std::cout << times << "times push and pop by 8 producer and 2 consumer cost " << interval << " nanoseconds" << std::endl;
    std::cout << "average time is " << interval / times << " nanoseconds" << std::endl;
    EXPECT_EQ(lfqueue.size(), 0);
    check(times, 8, consumer1, consumer2);

    producer = 2;
    consumer1.clear();
    consumer2.clear();
    timer.reset();
    std::thread p1(push);
    std::thread p2(push);
    std::thread c1(pop, std::ref(consumer1));
    p1.join();
    p2.join();
    c1.join();
    interval = timer.elapsed();
    std::cout << times << "times push and pop by 2 producer and 1 consumer cost " << interval << " nanoseconds" << std::endl;
    std::cout << "average time is " << interval / times << " nanoseconds" << std::endl;
    EXPECT_EQ(lfqueue.size(), 0);
    check(times, 2, consumer1, consumer2);

    producer = 1;
    consumer1.clear();
    consumer2.clear();
    timer.reset();
    std::thread p11(push);
    std::thread c11(pop, std::ref(consumer1));
    p11.join();
    c11.join();
    interval = timer.elapsed();
    std::cout << times << "times push and pop by 1 producer and 1 consumer cost " << interval << " nanoseconds" << std::endl;
    std::cout << "average time is " << interval / times << " nanoseconds" << std::endl;
    EXPECT_EQ(lfqueue.size(), 0);
    check(times, 1, consumer1, consumer2);
}

TEST(DSyncQueue, perf)
{
    DSyncQueue<DNormalMsg> syncqueue;
    int times = 100000;
    std::map<int, int> consumer1;
    std::map<int, int> consumer2;
    std::atomic<int> producer(8);
    
    auto&& push = [&syncqueue, &producer, times]() {
        int i = 0;
        while (i < times)
        {
            syncqueue.push(new DNormalMsg(std::to_string(i)));
            ++i;
        }
        --producer;
    };

    auto&& pop = [&syncqueue, &producer](std::map<int, int>& counter) {
        while (!syncqueue.empty() || producer > 0)
        {
            auto ret = syncqueue.pop_front();
            if (!ret)
            {
                std::this_thread::yield();
                continue;
            }
            counter[std::stoi(ret->toString())]++;
        }
    };

    DElapsedTimer timer;
    std::thread producerThread1(push);
    std::thread producerThread2(push);
    std::thread producerThread3(push);
    std::thread producerThread4(push);
    std::thread producerThread5(push);
    std::thread producerThread6(push);
    std::thread producerThread7(push);
    std::thread producerThread8(push);
    std::thread consumerThread1(pop, std::ref(consumer1));
    std::thread consumerThread2(pop, std::ref(consumer2));
    producerThread1.join();
    producerThread2.join();
    producerThread3.join();
    producerThread4.join();
    producerThread5.join();
    producerThread6.join();
    producerThread7.join();
    producerThread8.join();
    consumerThread1.join();
    consumerThread2.join();

    auto interval = timer.elapsed();
    std::cout << times << "*8 push and pop cost " << interval << " nanoseconds" << std::endl;
    std::cout << "average time is " << interval / times << " nanoseconds" << std::endl;
    EXPECT_EQ(syncqueue.size(), 0);
    check(times, 8, consumer1, consumer2);

    producer = 2;
    consumer1.clear();
    consumer2.clear();
    timer.reset();
    std::thread p1(push);
    std::thread p2(push);
    std::thread c1(pop, std::ref(consumer1));
    p1.join();
    p2.join();
    c1.join();
    interval = timer.elapsed();
    std::cout << times << "times push and pop by 2 producer and 1 consumer cost " << interval << " nanoseconds" << std::endl;
    std::cout << "average time is " << interval / times << " nanoseconds" << std::endl;
    EXPECT_EQ(syncqueue.size(), 0);
    check(times, 2, consumer1, consumer2);

    producer = 1;
    consumer1.clear();
    consumer2.clear();
    timer.reset();
    std::thread p11(push);
    std::thread c11(pop, std::ref(consumer1));
    p11.join();
    c11.join();
    interval = timer.elapsed();
    std::cout << times << "times push and pop by 1 producer and 1 consumer cost " << interval << " nanoseconds" << std::endl;
    std::cout << "average time is " << interval / times << " nanoseconds" << std::endl;
    EXPECT_EQ(syncqueue.size(), 0);
    check(times, 1, consumer1, consumer2);
}