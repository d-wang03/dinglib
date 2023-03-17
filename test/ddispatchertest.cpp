#include "gtest/gtest.h"
#include "ddispatcher.h"
#include "dnormalmsg.h"
#include "dobjecttest.h"
#include <memory>
#include <unistd.h>

using namespace ding;
TEST(DDispatcherTest, ctor)
{
    auto dispatcher = makeObject<DDispatcher>();
    EXPECT_STREQ(dispatcher->getTypeName(), "DDispatcher");
    EXPECT_TRUE(dispatcher->isSignal("bypass"));
}

TEST(DDispatcherTest, addRoutePath)
{
    auto dispatcher = makeObject<DDispatcher>();
    EXPECT_TRUE(dispatcher->addRoutePath(new DNormalMsg("msg1"), &DDispatcher::output<1>));
    EXPECT_TRUE(dispatcher->isSignal(&DDispatcher::output<1>));
    EXPECT_TRUE(dispatcher->isSignal("RoutePath0"));
}

TEST(DDispatcherTest, addRoutePathMax)
{
    auto dispatcher = makeObject<DDispatcher>();
    EXPECT_TRUE(dispatcher->addRoutePath(new DNormalMsg("msg0"), &DDispatcher::output<0>));
    EXPECT_TRUE(dispatcher->isSignal(&DDispatcher::output<0>));
    EXPECT_TRUE(dispatcher->isSignal("RoutePath0"));
    EXPECT_TRUE(dispatcher->addRoutePath(new DNormalMsg("msg1"), &DDispatcher::output<1>));
    EXPECT_TRUE(dispatcher->isSignal(&DDispatcher::output<1>));
    EXPECT_TRUE(dispatcher->isSignal("RoutePath1"));
    EXPECT_TRUE(dispatcher->addRoutePath(new DNormalMsg("msg2"), &DDispatcher::output<2>));
    EXPECT_TRUE(dispatcher->isSignal(&DDispatcher::output<2>));
    EXPECT_TRUE(dispatcher->isSignal("RoutePath2"));
    EXPECT_TRUE(dispatcher->addRoutePath(new DNormalMsg("msg3"), &DDispatcher::output<3>));
    EXPECT_TRUE(dispatcher->isSignal(&DDispatcher::output<3>));
    EXPECT_TRUE(dispatcher->isSignal("RoutePath3"));
    EXPECT_TRUE(dispatcher->addRoutePath(new DNormalMsg("msg4"), &DDispatcher::output<4>));
    EXPECT_TRUE(dispatcher->isSignal(&DDispatcher::output<4>));
    EXPECT_TRUE(dispatcher->isSignal("RoutePath4"));
    EXPECT_TRUE(dispatcher->addRoutePath(new DNormalMsg("msg5"), &DDispatcher::output<5>));
    EXPECT_TRUE(dispatcher->isSignal(&DDispatcher::output<5>));
    EXPECT_TRUE(dispatcher->isSignal("RoutePath5"));
    EXPECT_TRUE(dispatcher->addRoutePath(new DNormalMsg("msg6"), &DDispatcher::output<6>));
    EXPECT_TRUE(dispatcher->isSignal(&DDispatcher::output<6>));
    EXPECT_TRUE(dispatcher->isSignal("RoutePath6"));
    EXPECT_TRUE(dispatcher->addRoutePath(new DNormalMsg("msg7"), &DDispatcher::output<7>));
    EXPECT_TRUE(dispatcher->isSignal(&DDispatcher::output<7>));
    EXPECT_TRUE(dispatcher->isSignal("RoutePath7"));
    EXPECT_TRUE(dispatcher->addRoutePath(new DNormalMsg("msg8"), &DDispatcher::output<8>));
    EXPECT_TRUE(dispatcher->isSignal(&DDispatcher::output<8>));
    EXPECT_TRUE(dispatcher->isSignal("RoutePath8"));
    EXPECT_TRUE(dispatcher->addRoutePath(new DNormalMsg("msg9"), &DDispatcher::output<9>));
    EXPECT_TRUE(dispatcher->isSignal(&DDispatcher::output<9>));
    EXPECT_TRUE(dispatcher->isSignal("RoutePath9"));
    EXPECT_TRUE(dispatcher->addRoutePath(new DNormalMsg("msg10"), &DDispatcher::output<10>));
    EXPECT_TRUE(dispatcher->isSignal(&DDispatcher::output<10>));
    EXPECT_TRUE(dispatcher->isSignal("RoutePath10"));
    EXPECT_TRUE(dispatcher->addRoutePath(new DNormalMsg("msg11"), &DDispatcher::output<11>));
    EXPECT_TRUE(dispatcher->isSignal(&DDispatcher::output<11>));
    EXPECT_TRUE(dispatcher->isSignal("RoutePath11"));
    EXPECT_TRUE(dispatcher->addRoutePath(new DNormalMsg("msg12"), &DDispatcher::output<12>));
    EXPECT_TRUE(dispatcher->isSignal(&DDispatcher::output<12>));
    EXPECT_TRUE(dispatcher->isSignal("RoutePath12"));
    EXPECT_TRUE(dispatcher->addRoutePath(new DNormalMsg("msg13"), &DDispatcher::output<13>));
    EXPECT_TRUE(dispatcher->isSignal(&DDispatcher::output<13>));
    EXPECT_TRUE(dispatcher->isSignal("RoutePath13"));
    EXPECT_FALSE(dispatcher->addRoutePath(new DNormalMsg("msg14"), &DDispatcher::output<14>));
    EXPECT_FALSE(dispatcher->isSignal(&DDispatcher::output<14>));
    EXPECT_FALSE(dispatcher->isSignal("RoutePath14"));
}

TEST(DDispatcherTest, addRoutePath_dup_msg)
{
    auto dispatcher = makeObject<DDispatcher>();
    EXPECT_TRUE(dispatcher->addRoutePath(new DNormalMsg("msg1"), &DDispatcher::output<0>));
    EXPECT_FALSE(dispatcher->addRoutePath(new DNormalMsg("msg1"), &DDispatcher::output<1>));
    EXPECT_TRUE(dispatcher->isSignal(&DDispatcher::output<0>));
    EXPECT_FALSE(dispatcher->isSignal(&DDispatcher::output<1>));
}

TEST(DDispatcherTest, addRoutePath_dup_signal)
{
    auto dispatcher = makeObject<DDispatcher>();
    EXPECT_TRUE(dispatcher->addRoutePath(new DNormalMsg("msg1"), &DDispatcher::output<0>));
    EXPECT_FALSE(dispatcher->addRoutePath(new DNormalMsg("msg2"), &DDispatcher::output<0>));
    EXPECT_TRUE(dispatcher->isSignal(&DDispatcher::output<0>));
}

TEST(DDispatcherTest, run)
{
    auto dispatcher = makeObject<DDispatcher>();
    EXPECT_TRUE(dispatcher->addRoutePath(new DLogMsg(DLogMsg::Debug, "", ""), &DDispatcher::output<0>));
    EXPECT_TRUE(dispatcher->addRoutePath(new DLogMsg(DLogMsg::Info, "", ""), &DDispatcher::output<1>));
    EXPECT_TRUE(dispatcher->addRoutePath(new DLogMsg(DLogMsg::Warning, "", ""), &DDispatcher::output<2>));
    EXPECT_TRUE(dispatcher->addRoutePath(new DLogMsg(DLogMsg::Error, "", ""), &DDispatcher::output<3>));
    auto receiver = makeObject<TestObject>();
    auto sender = makeObject<TestObject>();
    EXPECT_TRUE(connect(sender, &TestObject::sig1, dispatcher, &DDispatcher::input));
    EXPECT_TRUE(connect(dispatcher, &DDispatcher::output<0>, receiver, &TestObject::slot1));
    EXPECT_TRUE(connect(dispatcher, &DDispatcher::output<1>, receiver, &TestObject::slot1));
    EXPECT_TRUE(connect(dispatcher, &DDispatcher::output<2>, receiver, &TestObject::slot2));
    EXPECT_TRUE(connect(dispatcher, &DDispatcher::output<3>, receiver, &TestObject::slot2));
    EXPECT_TRUE(dispatcher->start());
    sender->sig1(* new DLogMsg(DLogMsg::Debug, "sender", "This is debug message."));
    sender->sig1(*new DLogMsg(DLogMsg::Info, "sender", "This is info message."));
    sender->sig1(*new DLogMsg(DLogMsg::Warning, "sender", "This is warning message."));
    sender->sig1(*new DLogMsg(DLogMsg::Error, "sender", "This is error message."));
    sleep(1);
    dispatcher->stop();
    EXPECT_EQ(receiver->getSlot1Count(), 2);
    EXPECT_EQ(receiver->getSlot2Count(), 2);
}

TEST(DDispatcherTest, run_as_lockfree)
{
    auto dispatcher = makeObject<DDispatcher>();
    dispatcher->setLockfree(true);
    EXPECT_TRUE(dispatcher->addRoutePath(new DLogMsg(DLogMsg::Debug, "", ""), &DDispatcher::output<0>));
    EXPECT_TRUE(dispatcher->addRoutePath(new DLogMsg(DLogMsg::Info, "", ""), &DDispatcher::output<1>));
    EXPECT_TRUE(dispatcher->addRoutePath(new DLogMsg(DLogMsg::Warning, "", ""), &DDispatcher::output<2>));
    EXPECT_TRUE(dispatcher->addRoutePath(new DLogMsg(DLogMsg::Error, "", ""), &DDispatcher::output<3>));
    auto receiver = makeObject<TestObject>();
    auto sender = makeObject<TestObject>();
    EXPECT_TRUE(connect(sender, &TestObject::sig1, dispatcher, &DDispatcher::input));
    EXPECT_TRUE(connect(dispatcher, &DDispatcher::output<0>, receiver, &TestObject::slot1));
    EXPECT_TRUE(connect(dispatcher, &DDispatcher::output<1>, receiver, &TestObject::slot1));
    EXPECT_TRUE(connect(dispatcher, &DDispatcher::output<2>, receiver, &TestObject::slot2));
    EXPECT_TRUE(connect(dispatcher, &DDispatcher::output<3>, receiver, &TestObject::slot2));
    EXPECT_TRUE(dispatcher->start());
    sender->sig1(* new DLogMsg(DLogMsg::Debug, "sender", "This is debug message."));
    sender->sig1(*new DLogMsg(DLogMsg::Info, "sender", "This is info message."));
    sender->sig1(*new DLogMsg(DLogMsg::Warning, "sender", "This is warning message."));
    sender->sig1(*new DLogMsg(DLogMsg::Error, "sender", "This is error message."));
    sleep(1);
    dispatcher->stop();
    EXPECT_EQ(receiver->getSlot1Count(), 2);
    EXPECT_EQ(receiver->getSlot2Count(), 2);
}

TEST(DDispatcherTest, perf_lock)
{
    auto dispatcher = makeObject<DDispatcher>();
    dispatcher->setLockfree(false);
    EXPECT_TRUE(dispatcher->addRoutePath(new TestObject::PerfMsg(), &DDispatcher::output<0>));

    auto obj = makeObject<TestObject>();
    obj->setPerfNum(1000000);
    EXPECT_TRUE(connect(obj, &TestObject::perfSender, dispatcher, &DDispatcher::input));
    EXPECT_TRUE(connect(dispatcher, &DDispatcher::output<0>, obj, &TestObject::perfReceiver));
    EXPECT_TRUE(dispatcher->start());
    
    TestObject::PerfMsg msg;
    msg.setValue(100);
    obj->perfSender(msg);
    sleep(1);
    dispatcher->stop();
    obj->showPerfResult();
}

TEST(DDispatcherTest, perf_lockfree)
{
    auto dispatcher = makeObject<DDispatcher>();
    dispatcher->setLockfree(true);
    EXPECT_TRUE(dispatcher->addRoutePath(new TestObject::PerfMsg(), &DDispatcher::output<0>));

    auto obj = makeObject<TestObject>();
    obj->setPerfNum(1000000);
    EXPECT_TRUE(connect(obj, &TestObject::perfSender, dispatcher, &DDispatcher::input));
    EXPECT_TRUE(connect(dispatcher, &DDispatcher::output<0>, obj, &TestObject::perfReceiver));
    EXPECT_TRUE(dispatcher->start());
    
    TestObject::PerfMsg msg;
    msg.setValue(100);
    obj->perfSender(msg);
    sleep(1);
    dispatcher->stop();
    obj->showPerfResult();
}