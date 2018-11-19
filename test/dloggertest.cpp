#include "gtest/gtest.h"
#include "dlogger.h"
#include "dobject.h"
#include <chrono>
#include <thread>
#include "dobjecttest.h"

using namespace ding;
using namespace std::chrono_literals;

TEST(TLoggerTest, messagesBeforeStart)
{
    std::vector<std::string> buffer;
    auto logger = makeObject<DLogger>("TLoggerTest");
    logger->setDefaultProcessor([&buffer](const DLogMsg &msg) { buffer.emplace_back(msg.getContent()); });
    auto testObj = makeObject<TestObject>();
    EXPECT_TRUE(testObj->isSignal(&TestObject::logging));
    EXPECT_TRUE(testObj->connect(&TestObject::logging, logger, &DLogger::input));
    testObj->loggingI("The first message.");
    testObj->loggingI("The second message.");
    testObj->loggingI("The third message.");
    EXPECT_TRUE(logger->start());
    testObj->loggingI("The forth message.");
    std::this_thread::sleep_for(500ms);
    logger->stop();

    EXPECT_EQ(buffer.size(),4U);
}
