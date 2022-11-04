#ifndef DOBJECTTEST_H
#define DOBJECTTEST_H

#include "gtest/gtest.h"
#include "dobject.h"
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

public:
    TestObject(bool detailInfo = false)
        : DObject(__func__)
        , m_nSlot1Count(0)
        , m_nSlot2Count(0)
        , m_bDetailInfo(detailInfo)
    {
        ADD_SIGNAL(TestObject, sig1);
        ADD_SIGNAL(TestObject, sig2);
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
    }
    void slot1(DParam &param) { ++m_nSlot1Count; }

    void slot2(DParam &param) { ++m_nSlot2Count; }

    void tempSlot(DParam &param) {}

    void signalChecker(DParam &param)
    {
        m_signals.push(*param.clone());
        if (m_bDetailInfo)
        {
            std::cout << "Trigger Name:" << param.getTriggerName()
                      << "Trigger Signal:" << param.getTriggerSignal()
                      << std::endl;
        }
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

#define CHECK_SIGNAL(checker, exp_obj, exp_sig)                                                                        \
    do                                                                                                                 \
    {                                                                                                                  \
        bool ret = checker->empty();                                                                                   \
        EXPECT_FALSE(ret);                                                                                             \
        if (!ret)                                                                                                      \
        {                                                                                                              \
            DParam param = checker->getFirstSignal();                                                                  \
            EXPECT_STREQ(param.getTriggerName().c_str(), exp_obj.c_str());                                             \
            EXPECT_STREQ(param.getTriggerSignal().c_str(), exp_sig.c_str());                                           \
        }                                                                                                              \
    } while (0)

inline void checkSignal(const std::shared_ptr<TestObject> &checker, const std::string &exp_obj,
                        const std::string &exp_sig)
{
    bool ret = checker->empty();
    EXPECT_FALSE(ret);
    if (ret)
        return;
    DParam param = checker->getFirstSignal();
    EXPECT_STREQ(param.getTriggerName().c_str(), exp_obj.c_str());
    EXPECT_STREQ(param.getTriggerSignal().c_str(), exp_sig.c_str());
}

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
