#ifndef DOBJECTTEST_H
#define DOBJECTTEST_H

#include "gtest/gtest.h"
#include "dobject.h"
#include "utility/delapsedtimer.h"
#include "dparam_p.h"
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
    DElapsedTimer *timer;
#ifdef __x86_64__
    DElapsedTimer2 *timer2;
    uint64_t perf_sum2;
#endif
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
        , timer(new DElapsedTimer(false))
#ifdef __x86_64__
        , timer2(new DElapsedTimer2(2401,false))
        , perf_sum2(0)
#endif
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
    class PerfMsgPrivate : public DParamPrivate
    {
        public:
        PerfMsgPrivate():DParamPrivate(),m_value(0){}
        uint64_t m_value;
    };
    class PerfMsg : public DParam
    {
        DECLARE_PRIVATE(PerfMsg);
        public:
        PerfMsg():DParam(__func__, *new PerfMsgPrivate){}
        virtual PerfMsg *move() noexcept override
        {
            return new PerfMsg(std::move(*this));
        }
        virtual PerfMsg *clone() const override
        {
            return new PerfMsg(*this);
        }
        inline uint64_t getValue()const
        {
            D_D_CONST(PerfMsg);
            return d ? d->m_value : 0;
        }
        void setValue(uint64_t value)
        {
            D_D(PerfMsg);
            if (d)
                d->m_value = value;
        }

    };
    void perfSender(DParam& param)
    {
        if (perf_index == 0)
        {
            timer->reset();
        #ifdef __x86_64__
            timer2->reset();
        #endif
            ++perf_index;
            emitSignal(&TestObject::perfSender, param);
        }
        else if (perf_index < perf_num)
        {
            ++perf_index;
            emitSignal(&TestObject::perfSender, param);
        }
        else
        {
            perf_sum = timer->elapsed<std::chrono::nanoseconds>();
        #ifdef __x86_64__
            perf_sum2 += timer2->elapsedClks();
        #endif            
            return;
        }
    }

    void perfReceiver(DParam &param) 
    {
        perfSender(param);
    }

    void showPerfResult()const
    {
        std::cout << "std::chrono: Total send->receive: " << perf_index << ", cost " << perf_sum << " nanoseconds. average cost is " << perf_sum/perf_index << std::endl;
        #ifdef __x86_64
        std::cout << "rdtscp: Total send->receive: " << perf_index << ", cost " << perf_sum2 << " clocks. average cost is " << perf_sum2/perf_index << std::endl;
        #endif
    }

    int getSlot1Count() { return m_nSlot1Count; }
    int getSlot2Count() { return m_nSlot2Count; }
    void clearSlotCounter()
    {
        m_nSlot1Count = 0;
        m_nSlot2Count = 0;
    }
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
