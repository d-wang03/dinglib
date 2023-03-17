#include <gtest/gtest.h>
#include "dmsgpool.h"
#include "utility/delapsedtimer.h"

using namespace ding;

TEST(DMsgPoolTest, alloc_free)
{
    auto msg = allocMsg<DParam>("TypeA");
    EXPECT_STREQ(msg->getTypeName(), "TypeA");
    auto &ins = DMessagePool<DParam>::getInstance();
    ins.clear();
    EXPECT_TRUE(ins.empty());
    freeMsg(msg);
    EXPECT_FALSE(ins.empty());
    EXPECT_EQ(ins.size(), 1U);
    ins.clear();
}

TEST(DMsgPoolTest, free)
{
    auto msg = allocMsg<DParam>("TypeA");
    EXPECT_STREQ(msg->getTypeName(), "TypeA");
    auto &ins = DMessagePool<DParam>::getInstance();
    EXPECT_TRUE(ins.empty());
    freeMsg(msg);
    EXPECT_FALSE(ins.empty());
    EXPECT_EQ(ins.size(), 1U);
    freeMsg(msg);
    EXPECT_EQ(ins.size(), 1U);
    msg = allocMsg<DParam>("TypeA");
    auto msg2 = allocMsg<DParam>("TypeA");
    freeMsg(msg);
    freeMsg(msg2);
    EXPECT_EQ(ins.size(), 2U);
    ins.clear();
}

TEST(DMsgPoolTest, perf)
{
    uint32_t times = 1000000;
    DElapsedTimer timer;
    for (uint32_t i = 0; i < times; ++i)
    {
        auto msg = allocMsg<DParam>("TypeA");
        freeMsg(msg);
    }
    auto interval = timer.elapsed();
    std::cout << times << " times alloc and free cost " << interval << " nanoseconds" << std::endl;

    timer.reset();
    for (uint32_t i = 0; i < times; ++i)
    {
        auto msg = new DParam("TypeA");
        delete msg;
    }
    interval = timer.elapsed();
    std::cout << times << " times new and delete cost " << interval << " nanoseconds" << std::endl;

}