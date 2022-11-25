#ifndef DOBJECTTEST_H
#define DOBJECTTEST_H

#include "gtest/gtest.h"
#include "dobject.h"
#include "utility/delapsedtimer.h"
#include <queue>
#include <iostream>

// object used in test.
using namespace ding;
class TestObject : public ::ding::DObject
{
    SIGNAL(TestObject,sig1)
    SIGNAL(TestObject,sig2)
    SIGNAL(TestObject,errSig)
    SIGNAL(TestObject,tempSig)
private:
    int m_nSlot1Count;
    int m_nSlot2Count;
    bool m_bDetailInfo;
    std::queue<DParam> m_signals;
    DElapsedTimer timer;
    uint64_t perf_sum;
    uint64_t perf_index;
    uint64_t perf_num;

public:
    TestObject(bool detailInfo = false)
        : DObject(__func__)
        , m_nSlot1Count(0)
        , m_nSlot2Count(0)
        , m_bDetailInfo(detailInfo)
        , perf_sum(0)
        , perf_index(0)
        , perf_num(0)
    {
        ADD_SIGNAL(TestObject, sig1);
        ADD_SIGNAL(TestObject, sig2);
        ADD_SIGNAL(TestObject, perfSender);
    }
    ~TestObject()override = default;
    TestObject(const TestObject&) = default;
    TestObject(TestObject && rhs)noexcept : ding::DObject(std::move(rhs))
    {
        m_nSlot1Count = rhs.m_nSlot1Count;
        rhs.m_nSlot1Count = 0;
        m_nSlot2Count = rhs.m_nSlot2Count;
        rhs.m_nSlot2Count = 0;
        m_bDetailInfo = rhs.m_bDetailInfo;
        rhs.m_bDetailInfo = false;
        m_signals = std::move(rhs.m_signals);
        perf_sum = rhs.perf_sum;
        perf_index = rhs.perf_index;
        perf_num = rhs.perf_num;
    }
    void slot1(DParam &param) { ++m_nSlot1Count; }

    void slot2(DParam &param) { ++m_nSlot2Count; }

    void tempSlot(DParam &param) {}

    void setPerfNum(uint64_t value)
    {
        perf_num = value;
    }
    class PerfMsg : public DParam
    {
        public:
        PerfMsg():DParam(__func__){}
        PerfMsg(PerfMsg &&other):DParam(std::move(other))
        {
            value = other.value;
        }
        PerfMsg* move()noexcept override
        {
            return new PerfMsg(std::move(*this));
        }
        uint64_t value;
    };
    void perfSender(DParam& param)
    {
        if (++perf_index > perf_num)
            return;
        PerfMsg &msg = static_cast<PerfMsg&>(param);
        timer.reset();
        emitSignal(&TestObject::perfSender, msg);
    }

    void perfReceiver(DParam &param) 
    {
        PerfMsg &msg = static_cast<PerfMsg&>(param);
        perf_sum += timer.elapsed<std::chrono::nanoseconds>();
        perfSender(msg);
    }

    void showPerfResult()const
    {
        std::cout << "Total send->receive: " << perf_index << ", cost " << perf_sum << " nanoseconds. average cost is " << perf_sum/perf_index << std::endl;
    }

    int getSlot1Count() { return m_nSlot1Count; }
    int getSlot2Count() { return m_nSlot2Count; }
    DParam getFirstSignal()
    {
        if (m_signals.empty())
            return DParam{};
        DParam ret = m_signals.front();
        m_signals.pop();
        return ret;
    }
    DParam getLastSignal()
    {
        if (m_signals.empty())
            return DParam{};
        DParam ret = m_signals.back();
        m_signals.pop();
        return ret;
    }
    bool empty() const { return m_signals.empty(); }
    void clear()
    {
        m_nSlot1Count = 0;
        m_nSlot2Count = 0;
    }
    template <uint32_t Index>
    void output(DParam& param)
    {
        emitSignal(&TestObject::output<Index>, param);
    }
};

// The fixture for testing class TObjectTest.
class DObjectTest : public ::testing::Test
{
public:
    DObjectTest()
    {
        // You can do set-up work for each test here.
        m_obj1 = makeObject<TestObject>();
        m_obj2 = makeObject<TestObject>();
    }

    virtual ~DObjectTest()
    {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    virtual void SetUp()
    {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    virtual void TearDown()
    {
        // Code here will be called immediately after each test (right
        // before the destructor).
        m_obj1->clear();
        m_obj2->clear();
    }

    // Objects declared here can be used by all tests in the test case for TObjectTest.
    std::shared_ptr<TestObject> m_obj1;
    std::shared_ptr<TestObject> m_obj2;
};

#endif // DOBJECTTEST_H
