#include <gtest/gtest.h>

#include <algorithm>
#include <iterator>
#include <set>
#include <utility>
#include <vector>

//! @class MyCalendarFA
//! @brief First attempt solution to check if can add event w/o double booking
//! @details https://leetcode.com/problems/my-calendar-i/description/
//!
//!          Time complexity O(log N) where N = number of calls to book. In the
//!          worst case when start_end_times has N non-intersecting ranges, the
//!          call to std::upper_bound will take O(log N). Inserting at the front
//!          of start_end_times takes O(N) and insertions elsewhere are also
//!          O(N) (other than at the end).
//!          Space complexity O(N) in the worst case when start_end_times must
//!          store all event intervals.
class MyCalendarFA
{
public:
    MyCalendarFA()
    {
        //! At most 1000 calls will be made (1000 start and end times)
        start_end_times.reserve(1000);
    }

    bool book(int startTime, int endTime)
    {
        if (start_end_times.empty())
        {
            start_end_times.emplace_back(startTime, endTime);
            return true;
        }

        auto gt_it = std::upper_bound(start_end_times.begin(),
                                      start_end_times.end(),
                                      startTime,
                                      [](int                        tstart,
                                         const std::pair<int, int>& start_end)
                                      {
                                          return tstart < start_end.first;
                                      });

        if (gt_it == start_end_times.begin())
        {
            const auto [greater_tstart, greater_tend] = *gt_it;

            if (endTime <= greater_tstart)
            {
                start_end_times.emplace(start_end_times.begin(),
                                        startTime,
                                        endTime);
                return true;
            }

            return false;
        }
        else if (gt_it == start_end_times.end())
        {
            if (startTime >= start_end_times.back().second)
            {
                start_end_times.emplace_back(startTime, endTime);
                return true;
            }

            return false;
        }

        auto le_it = std::prev(gt_it);

        if (startTime < le_it->second || endTime > gt_it->first)
        {
            return false;
        }

        start_end_times.emplace(gt_it, startTime, endTime);
        return true;
    }

private:
    //! Vector of <startTime, endTime>
    std::vector<std::pair<int, int>> start_end_times;

}; // class MyCalendarFA

//! @class MyCalendarDS1
//! @brief Brute force discussion solution to check if can add event
//! @details https://leetcode.com/problems/my-calendar-i/description/
//!
//!          Time complexity O(N ^ 2) where N = number of calls to book. For
//!          each new event, we process every previous event to decide.
//!          Space complexity O(N) for start_end_times.
class MyCalendarDS1
{
public:
    bool book(int startTime, int endTime)
    {
        for (const auto& [tstart, tend] : start_end_times)
        {
            if (startTime < tend && tstart < endTime)
            {
                return false;
            }
        }

        start_end_times.emplace_back(startTime, endTime);
        return true;
    }

private:
    //! Vector of <startTime, endTime>
    std::vector<std::pair<int, int>> start_end_times;
};

//! @class MyCalendarDS2
//! @brief Sorted list + binary search discussion solution
//! @details https://leetcode.com/problems/my-calendar-i/description/
//!
//!          Time complexity O(N * log N) where N = number of events booked. For
//!          each new event, we check that the event does not double book in
//!          O(log N) and then insert it in O(log N).
//!          Space complexity O(N) for start_end_times.
class MyCalendarDS2
{
public:
    bool book(int startTime, int endTime)
    {
        const auto event         = std::make_pair(startTime, endTime);
        const auto next_event_it = start_end_times.lower_bound(event);

        if (next_event_it != start_end_times.end()
            && next_event_it->first < endTime)
        {
            return false;
        }

        if (next_event_it != start_end_times.begin())
        {
            const auto prev_event_it = std::prev(next_event_it);
            if (prev_event_it->second > startTime)
            {
                return false;
            }
        }

        start_end_times.insert(event);
        return true;
    }

private:
    //! Set of <startTime, endTime>
    std::set<std::pair<int, int>> start_end_times;
};

TEST(MyCalendarTest, SampleTest1)
{
    MyCalendarFA my_calendar_fa;

    EXPECT_TRUE(my_calendar_fa.book(10, 20));
    EXPECT_FALSE(my_calendar_fa.book(15, 25));
    EXPECT_TRUE(my_calendar_fa.book(20, 30));

    MyCalendarDS1 my_calendar_ds1;

    EXPECT_TRUE(my_calendar_ds1.book(10, 20));
    EXPECT_FALSE(my_calendar_ds1.book(15, 25));
    EXPECT_TRUE(my_calendar_ds1.book(20, 30));

    MyCalendarDS2 my_calendar_ds2;

    EXPECT_TRUE(my_calendar_ds2.book(10, 20));
    EXPECT_FALSE(my_calendar_ds2.book(15, 25));
    EXPECT_TRUE(my_calendar_ds2.book(20, 30));
}
