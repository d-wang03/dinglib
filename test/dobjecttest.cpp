#include "dobjecttest.h"

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

using namespace testing;

TEST_F(DObjectTest, Ctor)
{
    auto obj = makeObject<DObject>();
    EXPECT_STREQ("DObject", obj->getTypeName().c_str());
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

TEST_F(DObjectTest, GetObjectName)
{
    EXPECT_STREQ("TestObject", m_obj1->getTypeName().c_str());
}

TEST_F(DObjectTest, SetObjectName)
{
    m_obj1->setTypeName("ChangeName");
    EXPECT_STREQ("ChangeName", m_obj1->getTypeName().c_str());
}

TEST_F(DObjectTest, SetNullObjectName)
{
    m_obj1->setTypeName(std::string());
    EXPECT_STREQ("TestObject", m_obj1->getTypeName().c_str());
}
