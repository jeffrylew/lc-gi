#include <gtest/gtest.h>

#include <map>

//! @class MyCalendarThreeFA
//! @brief First attempt solution to get maximum k intersections between events
//! @details https://leetcode.com/problems/my-calendar-iii/description
//!
//!          Time complexity O(N) where N = number of calls to book. For N calls
//!          to book(), there will be 2N entries in the time_counts map. During
//!          each call to book(), we iterate through time_counts in O(N).
//!          Space complexity O(N) for the time_counts map.
class MyCalendarThreeFA
{
public:
    int book(int startTime, int endTime)
    {
        ++time_counts[startTime];
        --time_counts[endTime];

        int num_intersections {};

        for (const auto& [time, count] : time_counts)
        {
            num_intersections += count;
            max_intersections = std::max(max_intersections, num_intersections);
        }

        return max_intersections;
    }

private:
    //! Map of startTime/endTime and their positive/negative counts respectively
    std::map<int, int> time_counts;

    int max_intersections {};
};

TEST(MyCalendarThreeTest, SampleTest1)
{
    MyCalendarThreeFA my_calendar_three_fa;

    EXPECT_EQ(1, my_calendar_three_fa.book(10, 20));
    EXPECT_EQ(1, my_calendar_three_fa.book(50, 60));
    EXPECT_EQ(2, my_calendar_three_fa.book(10, 40));
    EXPECT_EQ(3, my_calendar_three_fa.book(5, 15));
    EXPECT_EQ(3, my_calendar_three_fa.book(5, 10));
    EXPECT_EQ(3, my_calendar_three_fa.book(25, 55));
}
