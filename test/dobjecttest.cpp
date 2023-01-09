#include "dobjecttest.h"
#include "utility/delapsedtimer.h"
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

using namespace testing;

TEST_F(DObjectTest, Ctor)
{
    auto obj = makeObject<DObject>();
    EXPECT_STREQ("DObject", obj->getTypeName());
}

TEST_F(DObjectTest, AllNullConnect)
{
    EXPECT_FALSE(connect(std::weak_ptr<DObject>{}, static_cast<void (DObject::*)()>(nullptr),
                         std::weak_ptr<DObject>{}, static_cast<void (DObject::*)()>(nullptr)));
}

TEST_F(DObjectTest, SrcNullConnect)
{
    EXPECT_FALSE(connect(std::weak_ptr<TestObject>{}, &TestObject::sig1,
                         std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    EXPECT_FALSE(connect(std::weak_ptr<TestObject>{}, &TestObject::sig1,
                         std::weak_ptr<DObject>{}, &TestObject::slot1));
    EXPECT_FALSE(connect(std::weak_ptr<TestObject>{}, &TestObject::sig1,
                         m_obj2, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    EXPECT_FALSE(connect(std::weak_ptr<TestObject>{}, &TestObject::sig1,
                         m_obj2, &TestObject::slot1));
    EXPECT_FALSE(connect(std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                         std::weak_ptr<DObject>{}, &TestObject::slot1));
    EXPECT_FALSE(connect(std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                         m_obj2, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    EXPECT_FALSE(connect(std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                         m_obj2, &TestObject::slot1));
}

TEST_F(DObjectTest, SigNullConnect)
{
    EXPECT_FALSE(connect(std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                         std::weak_ptr<TestObject>{}, &TestObject::slot1));
    EXPECT_FALSE(connect(std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                         m_obj2, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    EXPECT_FALSE(connect(std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                         m_obj2, &TestObject::slot1));
    EXPECT_FALSE(connect(m_obj1, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                         std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    EXPECT_FALSE(connect(m_obj1, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                         std::weak_ptr<TestObject>{}, &TestObject::slot1));
    EXPECT_FALSE(connect(m_obj1, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                         m_obj2, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    EXPECT_FALSE(connect(m_obj1, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                         m_obj2, &TestObject::slot1));
}

TEST_F(DObjectTest, DstNullConnect)
{
    EXPECT_FALSE(connect(std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                         std::weak_ptr<TestObject>{}, &TestObject::slot1));
    EXPECT_FALSE(connect(std::weak_ptr<TestObject>{}, &TestObject::sig1,
                         std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    EXPECT_FALSE(connect(std::weak_ptr<TestObject>{}, &TestObject::sig1,
                         std::weak_ptr<TestObject>{}, &TestObject::slot1));
    EXPECT_FALSE(connect(m_obj1, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                         std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    EXPECT_FALSE(connect(m_obj1, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                         std::weak_ptr<TestObject>{}, &TestObject::slot1));
    EXPECT_FALSE(connect(m_obj1, &TestObject::sig1,
                         std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    EXPECT_FALSE(connect(m_obj1, &TestObject::sig1,
                         std::weak_ptr<TestObject>{}, &TestObject::slot1));
}

TEST_F(DObjectTest, SlotNullConnect)
{
    EXPECT_FALSE(connect(std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                         m_obj2, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    EXPECT_FALSE(connect(std::weak_ptr<TestObject>{}, &TestObject::sig1,
                         std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    EXPECT_FALSE(connect(std::weak_ptr<TestObject>{}, &TestObject::sig1,
                         m_obj2, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    EXPECT_FALSE(connect(m_obj1, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                         std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    EXPECT_FALSE(connect(m_obj1, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                         m_obj2, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    EXPECT_FALSE(connect(m_obj1, &TestObject::sig1,
                         std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    EXPECT_FALSE(connect(m_obj1, &TestObject::sig1, m_obj2, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
}

TEST_F(DObjectTest, ErrSigConnect)
{
    EXPECT_FALSE(connect(m_obj1, &TestObject::errSig, m_obj2, &TestObject::slot1));
}

TEST_F(DObjectTest, CorrectConnect)
{
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
}

TEST_F(DObjectTest, NormalDisconnect)
{
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    EXPECT_TRUE(disconnect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    m_obj1->emitSignal(&TestObject::sig1, DParam{});
    EXPECT_EQ(m_obj2->getSlot1Count(),0U);
}

TEST_F(DObjectTest, ErrSigDisconnect)
{
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    EXPECT_FALSE(disconnect(m_obj1, &TestObject::errSig, m_obj2, &TestObject::slot1));
}

TEST_F(DObjectTest, AllNullDisconnect)
{
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    EXPECT_FALSE(disconnect(std::weak_ptr<DObject>{}, static_cast<void (DObject::*)()>(nullptr),std::weak_ptr<DObject>{}, static_cast<void (DObject::*)()>(nullptr)));
    m_obj1->emitSignal(&TestObject::sig1, DParam{});
    EXPECT_EQ(1, m_obj2->getSlot1Count());
}

TEST_F(DObjectTest, SrcNullDisconnect)
{
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    EXPECT_FALSE(disconnect(std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                            std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    EXPECT_FALSE(disconnect(std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                            std::weak_ptr<TestObject>{}, &TestObject::slot1));
    EXPECT_FALSE(disconnect(std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                            m_obj2, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    EXPECT_FALSE(disconnect(std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                            m_obj2, &TestObject::slot1));
    EXPECT_FALSE(disconnect(std::weak_ptr<TestObject>{}, &TestObject::sig1,
                            std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    EXPECT_FALSE(disconnect(std::weak_ptr<TestObject>{}, &TestObject::sig1,
                            std::weak_ptr<TestObject>{}, &TestObject::slot1));
    EXPECT_FALSE(disconnect(std::weak_ptr<TestObject>{}, &TestObject::sig1,
                            m_obj2, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    EXPECT_FALSE(disconnect(std::weak_ptr<TestObject>{}, &TestObject::sig1,
                            m_obj2, &TestObject::slot1));
    m_obj1->emitSignal(&TestObject::sig1, DParam{});
    EXPECT_EQ(1, m_obj2->getSlot1Count());
}

TEST_F(DObjectTest, SigNullDisconnect)
{
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    EXPECT_TRUE(disconnect(m_obj1, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                           std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    m_obj1->emitSignal(&TestObject::sig1, DParam{});
    EXPECT_EQ(m_obj2->getSlot1Count(),0);

    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    EXPECT_TRUE(disconnect(m_obj1, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                           std::weak_ptr<TestObject>{}, &TestObject::slot1));
    m_obj1->emitSignal(&TestObject::sig1, DParam{});
    EXPECT_EQ(m_obj2->getSlot1Count(),0);

    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    EXPECT_TRUE(disconnect(m_obj1, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                           m_obj2, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    m_obj1->emitSignal(&TestObject::sig1, DParam{});
    EXPECT_EQ(m_obj2->getSlot1Count(),0);

    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    EXPECT_TRUE(disconnect(m_obj1, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                           m_obj2, &TestObject::slot1));
    m_obj1->emitSignal(&TestObject::sig1, DParam{});
    EXPECT_EQ(m_obj2->getSlot1Count(),0);
}

TEST_F(DObjectTest, DstNullDisconnect)
{
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    EXPECT_TRUE(disconnect(m_obj1, &TestObject::sig1,
                           std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    m_obj1->emitSignal(&TestObject::sig1, DParam{});
    EXPECT_EQ(m_obj2->getSlot1Count(),0);

    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    EXPECT_TRUE(disconnect(m_obj1, &TestObject::sig1,
                           std::weak_ptr<TestObject>{}, &TestObject::slot1));
    m_obj1->emitSignal(&TestObject::sig1, DParam{});
    EXPECT_EQ(m_obj2->getSlot1Count(),0);
}

TEST_F(DObjectTest, SlotNullDisconnect)
{
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    EXPECT_TRUE(disconnect(m_obj1, &TestObject::sig1,
                           m_obj2, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    m_obj1->emitSignal(&TestObject::sig1, DParam{});
    EXPECT_EQ(m_obj2->getSlot1Count(),0);
}

TEST_F(DObjectTest, SingleDisconnect)
{
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    EXPECT_TRUE(disconnect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    m_obj1->emitSignal(&TestObject::sig1, DParam{});
    EXPECT_EQ(m_obj2->getSlot1Count(),0);
}

TEST_F(DObjectTest, MultiSlotDisconnect)
{
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot2));
    EXPECT_TRUE(disconnect(m_obj1, &TestObject::sig1, m_obj2, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    m_obj1->emitSignal(&TestObject::sig1, DParam{});
    EXPECT_EQ(m_obj2->getSlot1Count(),0);
    EXPECT_EQ(m_obj2->getSlot2Count(),0);
}

TEST_F(DObjectTest, MultiDstDisconnect)
{
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj1, &TestObject::slot1));
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    EXPECT_TRUE(disconnect(m_obj1, &TestObject::sig1, std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    m_obj1->emitSignal(&TestObject::sig1, DParam{});
    EXPECT_EQ(m_obj1->getSlot1Count(),0);
    EXPECT_EQ(m_obj2->getSlot1Count(),0);

    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj1, &TestObject::slot1));
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    EXPECT_TRUE(disconnect(m_obj1, &TestObject::sig1, std::weak_ptr<TestObject>{}, &TestObject::slot2));
    m_obj1->emitSignal(&TestObject::sig1, DParam{});
    EXPECT_EQ(m_obj1->getSlot1Count(),0);
    EXPECT_EQ(m_obj2->getSlot1Count(),0);
}

TEST_F(DObjectTest, MultiSigDisconnect)
{
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig2, m_obj2, &TestObject::slot2));
    EXPECT_TRUE(disconnect(m_obj1, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                           std::weak_ptr<TestObject>{}, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    m_obj1->emitSignal(&TestObject::sig1, DParam{});
    EXPECT_EQ(m_obj2->getSlot1Count(),0);
    EXPECT_EQ(m_obj2->getSlot2Count(),0);
}

TEST_F(DObjectTest, SpecialDstDisconnect)
{
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj1, &TestObject::slot1));
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig2, m_obj2, &TestObject::slot1));
    EXPECT_TRUE(disconnect(m_obj1, static_cast<void (TestObject::*)(DParam &)>(nullptr),
                           m_obj2, static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    m_obj1->emitSignal(&TestObject::sig1, DParam{});
    EXPECT_EQ(m_obj1->getSlot1Count(),0);
    m_obj1->emitSignal(&TestObject::sig2, DParam{});
    EXPECT_EQ(m_obj2->getSlot1Count(),0);
}

TEST_F(DObjectTest, SingleSlotEmit)
{
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    m_obj1->emitSignal(&TestObject::sig1, DParam{});
    EXPECT_EQ(1, m_obj2->getSlot1Count());
}

TEST_F(DObjectTest, SlotEmitTime)
{
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    int i = 0;
    int64_t sum = 1000000;

    m_obj2->clearSlotCounter();
    DParam param;
    DElapsedTimer timer;
    for (i=0; i < sum; ++i)
        m_obj1->emitSignal(&TestObject::sig1, param);
    auto t = timer.elapsed<std::chrono::nanoseconds>();
    std::cout << "chrono::steady_clock: " << m_obj2->getSlot1Count() << " time signal emit cost " << t << " nanoseconds. average is " << t/m_obj2->getSlot1Count() << std::endl;

    #ifdef __x86_64__
    m_obj2->clearSlotCounter();
    DElapsedTimer2 timer2(2401,true);
    for (i=0; i < sum; ++i)
    {
        m_obj1->emitSignal(&TestObject::sig1, param);
    }
    auto t2 = timer2.elapsed();
    std::cout << "rdtscp: " <<  m_obj2->getSlot1Count() << " time signal emit cost " << t2 << " nanoseconds. average is " << t2/m_obj2->getSlot1Count() << std::endl;
    #endif
}

TEST_F(DObjectTest, ErrSigEmit)
{
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    m_obj1->emitSignal(&TestObject::errSig, DParam{});
    EXPECT_EQ(m_obj1->getSlot1Count(),0);
}

TEST_F(DObjectTest, DuplicateSlotEmit)
{
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    m_obj1->emitSignal(&TestObject::sig1, DParam{});
    EXPECT_EQ(2, m_obj2->getSlot1Count());
}

TEST_F(DObjectTest, MultiSlotEmit)
{
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot1));
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::slot2));
    m_obj1->emitSignal(&TestObject::sig1, DParam{});
    EXPECT_EQ(1, m_obj2->getSlot1Count());
    EXPECT_EQ(1, m_obj2->getSlot2Count());
}

TEST_F(DObjectTest, Signal2SignalEmit)
{
    EXPECT_TRUE(connect(m_obj1, &TestObject::sig1, m_obj2, &TestObject::sig1));
    EXPECT_TRUE(connect(m_obj2, &TestObject::sig1, m_obj2, &TestObject::slot1));
    m_obj1->emitSignal(&TestObject::sig1, DParam{});
    EXPECT_EQ(1, m_obj2->getSlot1Count());
}

TEST_F(DObjectTest, NormalAddSignal)
{
    EXPECT_TRUE(m_obj1->addSignal("tempSig",&TestObject::tempSig));
    EXPECT_TRUE(m_obj1->isSignal("tempSig"));
    EXPECT_TRUE(m_obj1->isSignal(&TestObject::tempSig));
}

TEST_F(DObjectTest, AddNullSignal)
{
    EXPECT_FALSE(m_obj1->addSignal("",static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    EXPECT_FALSE(m_obj1->addSignal("aaa",static_cast<void (TestObject::*)(DParam &)>(nullptr)));
    EXPECT_FALSE(m_obj1->addSignal("",&TestObject::tempSig));
}

TEST_F(DObjectTest, AddDuplicateSignal)
{
    EXPECT_FALSE(m_obj1->addSignal("sig1",&TestObject::sig1));
}

TEST_F(DObjectTest, AddMaxiumSignals)
{
    EXPECT_TRUE(m_obj1->addSignal("sig1",&TestObject::output<1>));
    EXPECT_TRUE(m_obj1->addSignal("sig2",&TestObject::output<2>));
    EXPECT_TRUE(m_obj1->addSignal("sig3",&TestObject::output<3>));
    EXPECT_TRUE(m_obj1->addSignal("sig4",&TestObject::output<4>));
    EXPECT_TRUE(m_obj1->addSignal("sig5",&TestObject::output<5>));
    EXPECT_TRUE(m_obj1->addSignal("sig6",&TestObject::output<6>));
    EXPECT_TRUE(m_obj1->addSignal("sig7",&TestObject::output<7>));
    EXPECT_TRUE(m_obj1->addSignal("sig8",&TestObject::output<8>));
    EXPECT_TRUE(m_obj1->addSignal("sig9",&TestObject::output<9>));
    EXPECT_TRUE(m_obj1->addSignal("sig10",&TestObject::output<10>));
    EXPECT_TRUE(m_obj1->addSignal("sig11",&TestObject::output<11>));
    EXPECT_TRUE(m_obj1->addSignal("sig12",&TestObject::output<12>));
    EXPECT_FALSE(m_obj1->addSignal("sig13",&TestObject::output<13>));
    EXPECT_FALSE(m_obj1->addSignal("sig14",&TestObject::output<14>));
    EXPECT_FALSE(m_obj1->addSignal("sig15",&TestObject::output<15>));
    EXPECT_FALSE(m_obj1->addSignal("sig16",&TestObject::output<16>));
    EXPECT_FALSE(m_obj1->addSignal("sig17",&TestObject::output<17>));
}

TEST_F(DObjectTest, GetObjectName)
{
    EXPECT_STREQ("TestObject", m_obj1->getTypeName());
}

TEST_F(DObjectTest, SetObjectName)
{
    m_obj1->setTypeName("ChangeName");
    EXPECT_STREQ("ChangeName", m_obj1->getTypeName());
}

TEST_F(DObjectTest, SetNullObjectName)
{
    m_obj1->setTypeName(nullptr);
    EXPECT_FALSE(m_obj1->getTypeName());
}

TEST_F(DObjectTest, IsSignal_string)
{
    EXPECT_TRUE(m_obj1->isSignal("sig1"));
    EXPECT_TRUE(m_obj1->isSignal("sig2"));
    EXPECT_FALSE(m_obj1->isSignal("errSig"));
    EXPECT_FALSE(m_obj1->isSignal("tempSig"));
}

TEST_F(DObjectTest, IsSignal_PTR)
{
    EXPECT_TRUE(m_obj1->isSignal(&TestObject::sig1));
    EXPECT_TRUE(m_obj1->isSignal(&TestObject::sig2));
    EXPECT_FALSE(m_obj1->isSignal(&TestObject::errSig));
    EXPECT_FALSE(m_obj1->isSignal(&TestObject::tempSig));
}

TEST_F(DObjectTest, copy_ctor)
{
    TestObject obj = *m_obj1;
    EXPECT_TRUE(obj.isSignal(&TestObject::sig1));
    EXPECT_TRUE(obj.isSignal(&TestObject::sig2));
    EXPECT_TRUE(m_obj1->isSignal(&TestObject::sig1));
    EXPECT_TRUE(m_obj1->isSignal(&TestObject::sig2));
}

TEST_F(DObjectTest, dobject_copy_move)
{
    DObject obj1;
    obj1.setTypeName("Obj1");
    DObject obj2 = obj1;
    EXPECT_STREQ(obj1.getTypeName(),"Obj1");
    EXPECT_STREQ(obj2.getTypeName(),"Obj1");
    DObject obj3 = std::move(obj1);
    EXPECT_FALSE(obj1.getTypeName());
    EXPECT_STREQ(obj2.getTypeName(), "Obj1");
    EXPECT_STREQ(obj3.getTypeName(), "Obj1");
}

TEST_F(DObjectTest, dobject_clone)
{
    auto obj1 = makeObject<DObject>();
    obj1->setTypeName("Obj1");
    EXPECT_STREQ(obj1->getTypeName(), "Obj1");

    auto obj2 = obj1->clone();
    EXPECT_STREQ(obj1->getTypeName(), "Obj1");
    EXPECT_STREQ(obj2->getTypeName(), "Obj1");
}

TEST_F(DObjectTest, dobject_move)
{
    auto obj1 = makeObject<DObject>();
    obj1->setTypeName("Obj1");
    EXPECT_STREQ(obj1->getTypeName(), "Obj1");

    auto obj2 = obj1->move();
    EXPECT_FALSE(obj1->getTypeName());
    EXPECT_STREQ(obj2->getTypeName(), "Obj1");
    EXPECT_FALSE(obj1->isSignal("logging"));
    EXPECT_TRUE(obj2->isSignal("logging"));
}

TEST_F(DObjectTest, move_ctor)
{
    TestObject obj = std::move(*m_obj1);
    EXPECT_TRUE(obj.isSignal(&TestObject::sig1));
    EXPECT_TRUE(obj.isSignal(&TestObject::sig2));
    EXPECT_FALSE(m_obj1->isSignal(&TestObject::sig1));
    EXPECT_FALSE(m_obj1->isSignal(&TestObject::sig2));
}

TEST_F(DObjectTest, copy_op)
{
    auto obj1 = makeObject<DObject>();
    obj1->setTypeName("Obj1");
    EXPECT_STREQ(obj1->getTypeName(), "Obj1");

    auto obj2 = makeObject<DObject>();
    *obj2 = *obj1;
    EXPECT_STREQ(obj1->getTypeName(), "Obj1");
    EXPECT_STREQ(obj2->getTypeName(), "Obj1");
}

TEST_F(DObjectTest, move_op)
{
    auto obj1 = makeObject<DObject>();
    obj1->setTypeName("Obj1");
    EXPECT_STREQ(obj1->getTypeName(), "Obj1");

    auto obj2 = makeObject<DObject>();
    *obj2 = std::move(*obj1);
    EXPECT_FALSE(obj1->getTypeName());
    EXPECT_STREQ(obj2->getTypeName(), "Obj1");
    EXPECT_FALSE(obj1->isSignal("logging"));
    EXPECT_TRUE(obj2->isSignal("logging"));
}

TEST_F(DObjectTest, factory)
{
    auto obj1 = DObjectFactory::CreateObject<DObject>("DObject");
    EXPECT_TRUE(obj1);
    EXPECT_STREQ(obj1->getTypeName(), "DObject");
    EXPECT_TRUE(obj1->isSignal("logging"));
}