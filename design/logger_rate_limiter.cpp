#include <gtest/gtest.h>

#include <string>

//! @class LoggerFA
//! @brief First attempt solution
class LoggerFA
{
public:
    Logger()
    {

    }

    bool shouldPrintMessage(int timestamp, std::string message)
    {
        //! @todo
    }
};

TEST(LoggerTest, SampleTest1)
{
    LoggerFA logger;

    EXPECT_TRUE(logger.shouldPrintMessage(1, "foo"));
    EXPECT_TRUE(logger.shouldPrintMessage(2, "bar"));
    EXPECT_FALSE(logger.shouldPrintMessage(3, "foo"));
    EXPECT_FALSE(logger.shouldPrintMessage(8, "bar"));
    EXPECT_FALSE(logger.shouldPrintMessage(10, "foo"));
    EXPECT_TRUE(logger.shouldPrintMessage(11, "foo"));
}