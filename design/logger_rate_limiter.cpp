#include <gtest/gtest.h>

#include <queue>
#include <string>
#include <unordered_map>
#include <utility>

//! @class LoggerFA
//! @brief First attempt solution to check if a unique message can be printed
//! @details https://leetcode.com/explore/interview/card/google/65/design-4/3093
//!
//!          Time complexity amortised O(1). msg_time_queue could store all
//!          messages in the stream up to the last. Each queue push is O(1) and
//!          if the final message expires all elements in the queue then
//!          msg_time_queue.pop() will be called O(N) times where N = number of
//!          messages in the stream. However, the pop calls are amortised O(1)
//!          over all messages in the stream. msg_time_map operations are O(1).
//!          Space complexity O(N) where N = number of messages in the stream.
//!          In the worst case, msg_time_map/msg_time_queue store O(N) elements.
class LoggerFA
{
public:
    Logger() = default;

    bool shouldPrintMessage(int timestamp, std::string message)
    {
        while (!msg_time_queue.empty()
               && msg_time_queue.front()->second <= timestamp)
        {
            msg_time_map.erase(msg_time_queue.front());
            msg_time_queue.pop();
        }

        if (!msg_time_map.contains(message))
        {
            auto [itr, success] =
                msg_time_map.try_emplace(std::move(message), timestamp + 10);

            msg_time_queue.push(std::move(itr));

            return success;
        }

        return false;
    }

private:
    //! Map of <message, timestamp>
    std::unordered_map<std::string, int> msg_time_map;

    //! Queue of iterators to elements in msg_time_map
    std::queue<decltype(msg_time_map)::iterator> msg_time_queue;
};

//! @class LoggerDS1
//! @brief Queue + Set discussion solution
//! @details https://leetcode.com/problems/logger-rate-limiter/editorial
//!
//!          Time complexity amortised O(1) per call. In the worst case, a call
//!          might remove up to N obsolete messages from the queue, where N is
//!          the size of msg_queue. However, each message is added and removed
//!          at most once so over multiple calls, the total work is linear and
//!          the average (amortized) cost per call remains constant.
//!          Space complexity O(N) where N = msg_queue.size(). We keep incoming
//!          messages in both the queue and set. The upper bound on space is 2N
//!          when there are no duplicates at all.
class LoggerDS1
{
public:
    bool shouldPrintMessage(int timestamp, std::string message)
    {
        while (!msg_queue.empty())
        {
            const auto& [oldest_msg, oldest_tstamp] = msg_queue.front();

            if (timestamp - oldest_tstamp >= 10)
            {
                msg_set.erase(oldest_msg);
                msg_queue.pop();
            }
            else
            {
                break;
            }
        }

        if (msg_set.count(message) == 0)
        {
            msg_queue.emplace(message, timestamp);
            msg_set.insert(std::move(message));
            return true;
        }

        return false;
    }

private:
    std::unordered_set<std::string> msg_set;

    std::queue<std::pair<std::string, int>> msg_queue;
};

TEST(LoggerTest, SampleTest1)
{
    LoggerFA logger_fa;

    EXPECT_TRUE(logger_fa.shouldPrintMessage(1, "foo"));
    EXPECT_TRUE(logger_fa.shouldPrintMessage(2, "bar"));
    EXPECT_FALSE(logger_fa.shouldPrintMessage(3, "foo"));
    EXPECT_FALSE(logger_fa.shouldPrintMessage(8, "bar"));
    EXPECT_FALSE(logger_fa.shouldPrintMessage(10, "foo"));
    EXPECT_TRUE(logger_fa.shouldPrintMessage(11, "foo"));

    LoggerDS1 logger_ds1;

    EXPECT_TRUE(logger_ds1.shouldPrintMessage(1, "foo"));
    EXPECT_TRUE(logger_ds1.shouldPrintMessage(2, "bar"));
    EXPECT_FALSE(logger_ds1.shouldPrintMessage(3, "foo"));
    EXPECT_FALSE(logger_ds1.shouldPrintMessage(8, "bar"));
    EXPECT_FALSE(logger_ds1.shouldPrintMessage(10, "foo"));
    EXPECT_TRUE(logger_ds1.shouldPrintMessage(11, "foo"));
}