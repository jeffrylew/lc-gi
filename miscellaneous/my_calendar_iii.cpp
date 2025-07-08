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

//! @class MyCalendarThreeDS1
//! @brief Sweep-line algorithm discussion solution
//! @details https://leetcode.com/problems/my-calendar-iii/editorial/
//!
//!          Time complexity O(N ^ 2) where N = number of calls to book. For
//!          each new event, we update changes at two time points in O(log N).
//!          Then we traverse differential_at_time_point in O(N) for each of the
//!          N calls to book().
//!          Space complexity O(N) for the differential_at_time_point map.
class MyCalendarThreeDS1
{
public:
    int book(int startTime, int endTime)
    {
        int num_intervals_at_current_time {};
        int max_intersections {};

        ++differential_at_time_point[startTime];
        --differential_at_time_point[endTime];

        for (const auto& [time, delta_count] : differential_at_time_point)
        {
            num_intervals_at_current_time += delta_count;
            max_intersections = std::max(max_intersections,
                                         num_intervals_at_current_time);
        }

        return max_intersections;
    }

private:
    std::map<int, int> differential_at_time_point;
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

    MyCalendarThreeDS1 my_calendar_three_ds1;

    EXPECT_EQ(1, my_calendar_three_ds1.book(10, 20));
    EXPECT_EQ(1, my_calendar_three_ds1.book(50, 60));
    EXPECT_EQ(2, my_calendar_three_ds1.book(10, 40));
    EXPECT_EQ(3, my_calendar_three_ds1.book(5, 15));
    EXPECT_EQ(3, my_calendar_three_ds1.book(5, 10));
    EXPECT_EQ(3, my_calendar_three_ds1.book(25, 55));
}
