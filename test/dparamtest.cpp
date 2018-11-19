#include <gtest/gtest.h>
#include "dparam.h"

using namespace ding;

TEST(DParamTest, default_ctor)
{
    DParam a;
    EXPECT_STREQ("DParam", a.getTypeName().c_str());
}

TEST(DParamTest, copy_ctor)
{
    DParam a;
    a.setTypeName("TypeA");
    EXPECT_STREQ("TypeA", DParam(a).getTypeName().c_str());
}

TEST(DParamTest, move_ctor)
{
    DParam a;
    a.setTypeName("TypeA");
    EXPECT_STREQ("TypeA", DParam(std::move(a)).getTypeName().c_str());
    EXPECT_STREQ("", a.getTypeName().c_str());
}

TEST(DParamTest, assignment)
{
    DParam a;
    a.setTypeName("TypeA");
    DParam b = a;
    EXPECT_STREQ("TypeA", b.getTypeName().c_str());
}

TEST(DParamTest, move_assignment)
{
    DParam a;
    a.setTypeName("TypeA");
    DParam b = std::move(a);
    EXPECT_STREQ("TypeA", b.getTypeName().c_str());
    EXPECT_STREQ("", a.getTypeName().c_str());
}

TEST(DParamTest, clone)
{
    DParam a;
    a.setTypeName("TypeA");
    DParam *b = a.clone();
    EXPECT_STREQ("TypeA", b->getTypeName().c_str());
}

TEST(DParamTest, move)
{
    DParam a;
    a.setTypeName("TypeA");
    DParam *b = a.move();
    EXPECT_STREQ("TypeA", b->getTypeName().c_str());
    EXPECT_STREQ("", a.getTypeName().c_str());
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

TEST(DParamTest, equal_op)
{
    DParam a;
    a.setTypeName("TypeA");
    DParam b;
    EXPECT_FALSE(a == b);
    b = a;
    EXPECT_TRUE(a == b);
}
