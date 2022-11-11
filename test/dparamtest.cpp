#include <gtest/gtest.h>
#include "dparam.h"

using namespace ding;

TEST(DParamTest, default_ctor)
{
    DParam a;
    EXPECT_STREQ("DParam", a.getTypeName());
}

TEST(DParamTest, ctor_with_name)
{
    DParam a("TypeA");
    EXPECT_STREQ("TypeA", a.getTypeName());
}

TEST(DParamTest, copy_ctor)
{
    DParam a;
    a.setTypeName("TypeA");
    a.setTrigger("Trigger1", "sig1");
    DParam b = a;
    EXPECT_STREQ("TypeA", b.getTypeName());
    EXPECT_STREQ("Trigger1", b.getTriggerName());
    EXPECT_STREQ("sig1", b.getTriggerSignal());
}

TEST(DParamTest, move_ctor)
{
    DParam a;
    a.setTypeName("TypeA");
    a.setTrigger("Trigger1", "sig1");
    DParam b = std::move(a);
    EXPECT_STREQ("TypeA", b.getTypeName());
    EXPECT_FALSE(a.getTypeName());
    EXPECT_STREQ("Trigger1", b.getTriggerName());
    EXPECT_FALSE(a.getTriggerName());
    EXPECT_STREQ("sig1", b.getTriggerSignal());
    EXPECT_FALSE(a.getTriggerSignal());
}

TEST(DParamTest, assignment)
{
    DParam a;
    a.setTypeName("TypeA");
    DParam b = a;
    EXPECT_STREQ("TypeA", b.getTypeName());
}

TEST(DParamTest, move_assignment)
{
    DParam a;
    a.setTypeName("TypeA");
    DParam b;
    b = std::move(a);
    EXPECT_STREQ("TypeA", b.getTypeName());
    EXPECT_FALSE(a.getTypeName());
}

TEST(DParamTest, clone)
{
    DParam a;
    a.setTypeName("TypeA");
    DParam *b = a.clone();
    EXPECT_STREQ("TypeA", b->getTypeName());
}

TEST(DParamTest, move)
{
    DParam a;
    a.setTypeName("TypeA");
    DParam *b = a.move();
    EXPECT_STREQ("TypeA", b->getTypeName());
    EXPECT_FALSE(a.getTypeName());
}

TEST(DParamTest, factory)
{
    auto ptr = DObjectFactory::CreateObject<DParam>("DParam");
    EXPECT_STREQ(ptr->getTypeName(), "DParam");
    EXPECT_TRUE(!ptr->getTriggerName());
    EXPECT_TRUE(!ptr->getTriggerSignal());
}

TEST(DParamTest, toString)
{
    DParam a;
    a.setTypeName("TypeA");
    EXPECT_STREQ("TypeA", a.toString().c_str());
}

TEST(DParamTest, equals)
{
    DParam a;
    a.setTypeName("TypeA");
    DParam b;
    EXPECT_FALSE(a.equals(b));
    b = a;
    EXPECT_TRUE(a.equals(b));
}

TEST(DParamTest, equals2)
{
    DParam a;
    DParam b;
    EXPECT_TRUE(a.equals(b));
}

TEST(DParamTest, equals3)
{
    DParam a;
    DParam b;
    a.setTypeName(nullptr);
    b.setTypeName(nullptr);
    EXPECT_TRUE(a.equals(b));
}

TEST(DParamTest, equal_op)
{
    DParam a;
    a.setTypeName("TypeA");
    DParam b;
    EXPECT_FALSE(a == b);
    b = a;
    EXPECT_TRUE(a == b);
}

TEST(DParamTest, setTrigger)
{
    DParam a;
    a.setTrigger("obj1", "sig1");
    EXPECT_STREQ(a.getTriggerName(), "obj1");
    EXPECT_STREQ(a.getTriggerSignal(), "sig1");
}

TEST(DParamTest, setTrigger2)
{
    DParam a;
    a.setTrigger(nullptr, "sig1");
    EXPECT_FALSE(a.getTriggerName());
    EXPECT_STREQ(a.getTriggerSignal(), "sig1");
}

TEST(DParamTest, setTrigger3)
{
    DParam a;
    a.setTrigger("obj1", nullptr);
    EXPECT_FALSE(a.getTriggerSignal());
    EXPECT_STREQ(a.getTriggerName(), "obj1");
}

TEST(DParamTest, setTrigger4)
{
    DParam a;
    a.setTrigger("obj1", "sig1");
    EXPECT_STREQ(a.getTriggerName(), "obj1");
    EXPECT_STREQ(a.getTriggerSignal(), "sig1");
    a.setTrigger("obj1", "sig1");
    EXPECT_STREQ(a.getTriggerName(), "obj1");
    EXPECT_STREQ(a.getTriggerSignal(), "sig1");
}
TEST(DParamTest, setTrigger5)
{
    DParam a;
    a.setTrigger("obj1", "sig1");
    EXPECT_STREQ(a.getTriggerName(), "obj1");
    EXPECT_STREQ(a.getTriggerSignal(), "sig1");
    a.setTrigger(nullptr, nullptr);
    EXPECT_FALSE(a.getTriggerName());
    EXPECT_FALSE(a.getTriggerSignal());
}
TEST(DParamTest, setTrigger6)
{
    DParam a;
    a.setTrigger("obj1", "sig1");
    EXPECT_STREQ(a.getTriggerName(), "obj1");
    EXPECT_STREQ(a.getTriggerSignal(), "sig1");
    a.setTrigger("obj2", "sig2");
    EXPECT_STREQ(a.getTriggerName(), "obj2");
    EXPECT_STREQ(a.getTriggerSignal(), "sig2");
}
