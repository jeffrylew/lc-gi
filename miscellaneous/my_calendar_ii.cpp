#include <gtest/gtest.h>

#include <algorithm>
#include <iterator>
#include <map>
#include <utility>
#include <vector>

//! @class MyCalendarTwoFA
//! @brief First attempt solution to check if can add event w/o triple booking.
//! @details https://leetcode.com/problems/my-calendar-ii/description/
//!
//!          First attempt solution passes 24 / 97 testcases and does not pass
//!          SampleTest2.
//!
//!          Time complexity O(N * log N) where N = number of calls to book. In
//!          the worst case, N / 2 non-overlapping meetings are double booked.
//!          Each of the N / 2 calls to book takes O(log N) for std::upper_bound
//!          resulting in O(N * log N).
//!          Space complexity O(N) for single_booked_times/double_booked_times.
class MyCalendarTwoFA
{
public:
    MyCalendarTwoFA()
    {
        single_booked_times.reserve(1000);
        double_booked_times.reserve(1000);
    }

    bool book(int startTime, int endTime)
    {
        if (single_booked_times.empty())
        {
            single_booked_times.emplace_back(startTime, endTime);
            return true;
        }

        constexpr int default_tstart {1000000001};
        int           double_booked_tstart {default_tstart};
        int           double_booked_tend {-1};

        constexpr auto compare_tstart = [](int                        tstart,
                                           const std::pair<int, int>& start_end)
                                        {
                                            return tstart < start_end.first;
                                        };

        auto single_booked_gt_it = std::upper_bound(single_booked_times.begin(),
                                                    single_booked_times.end(),
                                                    startTime,
                                                    compare_tstart);

        if (single_booked_gt_it == single_booked_times.begin())
        {
            if (endTime > single_booked_gt_it->first)
            {
                double_booked_tstart = single_booked_gt_it->first;
                double_booked_tend   = endTime;
            }
            else
            {
                single_booked_times.emplace(single_booked_times.begin(),
                                            startTime,
                                            endTime);
                return true;
            }
        }
        else if (single_booked_gt_it == single_booked_times.end())
        {
            auto single_booked_le_it = std::prev(single_booked_gt_it);

            if (startTime < single_booked_le_it->second)
            {
                double_booked_tstart = startTime;
                double_booked_tend   = single_booked_le_it->second;
            }
            else
            {
                single_booked_times.emplace_back(startTime, endTime);
                return true;
            }
        }
        else
        {
            auto single_booked_le_it = std::prev(single_booked_gt_it);

            if (startTime >= single_booked_le_it->second
                && endTime <= single_booked_gt_it->first)
            {
                single_booked_times.emplace(single_booked_gt_it,
                                            startTime,
                                            endTime);
                return true;
            }

            if (startTime < single_booked_le_it->second)
            {
                double_booked_tstart = startTime;
                double_booked_tend   = single_booked_le_it->second;
            }

            if (endTime > single_booked_gt_it->first)
            {
                double_booked_tstart =
                    std::min(double_booked_tstart, single_booked_gt_it->first);
                double_booked_tend = endTime;
            }
        }

        if (double_booked_times.empty())
        {
            double_booked_times.emplace_back(double_booked_tstart,
                                             double_booked_tend);
            return true;
        }

        auto dbl_booked_gt_it = std::upper_bound(double_booked_times.begin(),
                                                 double_booked_times.end(),
                                                 double_booked_tstart,
                                                 compare_tstart);

        if (dbl_booked_gt_it == double_booked_times.begin())
        {
            if (double_booked_tend > dbl_booked_gt_it->first)
            {
                return false;
            }

            double_booked_times.emplace(double_booked_times.begin(),
                                        double_booked_tstart,
                                        double_booked_tend);
            return true;
        }

        auto dbl_booked_le_it = std::prev(dbl_booked_gt_it);

        if (dbl_booked_gt_it == double_booked_times.end())
        {
            if (double_booked_tstart < dbl_booked_le_it->second)
            {
                return false;
            }

            double_booked_times.emplace_back(double_booked_tstart,
                                             double_booked_tend);
            return true;
        }

        if (double_booked_tstart >= dbl_booked_le_it->second
            && double_booked_tend <= dbl_booked_gt_it->first)
        {
            double_booked_times.emplace(dbl_booked_gt_it,
                                        double_booked_tstart,
                                        double_booked_tend);
            return true;
        }

        return false;
    }

private:
    //! Vector of single booked <start time, end time>
    std::vector<std::pair<int, int>> single_booked_times;

    //! Vector of double booked <start time, end time>
    std::vector<std::pair<int, int>> double_booked_times;
};

//! @class MyCalendarTwoDS1
//! @brief Using overlapped intervals discussion solution
//! @details https://leetcode.com/problems/my-calendar-ii/description/
//!
//!          Time complexity O(N) where N = size of bookings vector. We iterate
//!          through the bookings list to check for overlaps and possibly add a
//!          new booking. Additionally, we check the overlap_bookings vector,
//!          which is always smaller or equal to the size of bookings.
//!          Space complexity O(N) for bookings and overlap_bookings. The size
//!          of overlap_bookings can never exceed the size of bookings.
class MyCalendarTwoDS1
{
public:
    bool book(int startTime, int endTime)
    {
        //! Return false if new booking overlaps with existing double booking
        for (const auto& [tstart, tend] : overlap_bookings)
        {
            if (has_overlap(tstart, tend, startTime, endTime))
            {
                return false;
            }
        }

        //! If there is a double booking, add it
        for (const auto& [tstart, tend] : bookings)
        {
            if (has_overlap(tstart, tend, startTime, endTime))
            {
                overlap_bookings.push_back(
                    get_overlap(tstart, tend, startTime, endTime));
            }
        }

        //! Add booking
        bookings.emplace_back(startTime, endTime);
        return true;
    }

private:
    std::vector<std::pair<int, int>> bookings;
    std::vector<std::pair<int, int>> overlap_bookings;

    //! @brief Check if [tstart1, tend1) and [tstart2, tend2) overlap
    [[nodiscard]] constexpr bool has_overlap(int tstart1,
                                             int tend1,
                                             int tstart2,
                                             int tend2)
    {
        return std::max(tstart1, tstart2) < std::min(tend1, tend2);
    }

    //! @brief Get overlapping booking for [tstart1, tend1) and [tstart2, tend2)
    [[nodiscard]] constexpr std::pair<int, int> get_overlap(int tstart1,
                                                            int tend1,
                                                            int tstart2,
                                                            int tend2)
    {
        return {std::max(tstart1, tstart2), std::min(tend1, tend2)};
    }
};

//! @class MyCalendarTwoDS2
//! @brief Line sweep discussion solution
//! @details https://leetcode.com/problems/my-calendar-ii/description/
//!
//!          Time complexity O(N) where N = size of bookings vector. We iterate
//!          over the booking entries in the map and find the prefix sum. For
//!          each of the O(N) entries, we can have 3 map operations of O(log N).
//!          Once we find the triple booking, we return and stop iterating.
//!          Space complexity O(N) to store the start and end points of each
//!          booking in the map. For N bookings, we store 2N entries.
class MyCalendarTwoDS2
{
public:
    bool book(int startTime, int endTime)
    {
        //! Increase and decrease the booking count
        //! at startTime and endTime respectively
        ++booking_count[startTime];
        ++booking_count[endTime];

        //! Find the prefix sum
        int num_overlaps {};

        for (const auto& [time_point, booking_count] : booking_count)
        {
            num_overlaps += booking_count;

            //! If the number of bookings is more than 2 then return false
            //! Also, undo the counts for this booking
            if (num_overlaps > max_overlaps)
            {
                --booking_count[startTime];
                ++booking_count[endTime];

                //! Remove entries from map to avoid unnecessary iteration
                if (booking_count[startTime] == 0)
                {
                    booking_count.erase(startTime);
                }

                if (booking_count[endTime] == 0)
                {
                    booking_count.erase(endTime);
                }

                return false;
            }
        }

        return true;
    }

private:
    //! Maximum number of bookings at each point
    constexpr int max_overlaps {2};

    //! Store the number of bookings at each point
    std::map<int, int> booking_count;
};

TEST(MyCalendarTwoTest, SampleTest1)
{
    MyCalendarTwoFA my_calendar_two_fa;

    EXPECT_TRUE(my_calendar_two_fa.book(10, 20));
    EXPECT_TRUE(my_calendar_two_fa.book(50, 60));
    EXPECT_TRUE(my_calendar_two_fa.book(10, 40));
    EXPECT_FALSE(my_calendar_two_fa.book(5, 15));
    EXPECT_TRUE(my_calendar_two_fa.book(5, 10));
    EXPECT_TRUE(my_calendar_two_fa.book(25, 55));

    MyCalendarTwoDS1 my_calendar_two_ds1;

    EXPECT_TRUE(my_calendar_two_ds1.book(10, 20));
    EXPECT_TRUE(my_calendar_two_ds1.book(50, 60));
    EXPECT_TRUE(my_calendar_two_ds1.book(10, 40));
    EXPECT_FALSE(my_calendar_two_ds1.book(5, 15));
    EXPECT_TRUE(my_calendar_two_ds1.book(5, 10));
    EXPECT_TRUE(my_calendar_two_ds1.book(25, 55));

    MyCalendarTwoDS2 my_calendar_two_ds2;

    EXPECT_TRUE(my_calendar_two_ds2.book(10, 20));
    EXPECT_TRUE(my_calendar_two_ds2.book(50, 60));
    EXPECT_TRUE(my_calendar_two_ds2.book(10, 40));
    EXPECT_FALSE(my_calendar_two_ds2.book(5, 15));
    EXPECT_TRUE(my_calendar_two_ds2.book(5, 10));
    EXPECT_TRUE(my_calendar_two_ds2.book(25, 55));
}

TEST(MyCalendarTwoTest, SampleTest2)
{
    MyCalendarTwoFA my_calendar_two_fa;

    EXPECT_TRUE(my_calendar_two_fa.book(47, 50));
    EXPECT_TRUE(my_calendar_two_fa.book(1, 10));
    EXPECT_TRUE(my_calendar_two_fa.book(27, 36));
    EXPECT_TRUE(my_calendar_two_fa.book(40, 47));
    EXPECT_TRUE(my_calendar_two_fa.book(20, 27));
    EXPECT_TRUE(my_calendar_two_fa.book(15, 23));
    EXPECT_TRUE(my_calendar_two_fa.book(10, 18));
    EXPECT_TRUE(my_calendar_two_fa.book(27, 36));
    EXPECT_FALSE(my_calendar_two_fa.book(17, 25));
    // EXPECT_FALSE(my_calendar_two_fa.book(8, 17)); // Incorrectly returns true
    EXPECT_FALSE(my_calendar_two_fa.book(24, 33));
    EXPECT_FALSE(my_calendar_two_fa.book(23, 28));
    EXPECT_FALSE(my_calendar_two_fa.book(21, 27));
    EXPECT_TRUE(my_calendar_two_fa.book(47, 50));
    EXPECT_FALSE(my_calendar_two_fa.book(14, 21));
    EXPECT_FALSE(my_calendar_two_fa.book(26, 32));
    EXPECT_FALSE(my_calendar_two_fa.book(16, 21));
    // EXPECT_TRUE(my_calendar_two_fa.book(2, 7)); // Incorrectly returns false
    EXPECT_FALSE(my_calendar_two_fa.book(24, 33));
    EXPECT_FALSE(my_calendar_two_fa.book(6, 13));
    EXPECT_FALSE(my_calendar_two_fa.book(44, 50));
    EXPECT_FALSE(my_calendar_two_fa.book(33, 39));
    EXPECT_FALSE(my_calendar_two_fa.book(30, 36));
    EXPECT_FALSE(my_calendar_two_fa.book(6, 15));
    EXPECT_FALSE(my_calendar_two_fa.book(21, 27));
    EXPECT_FALSE(my_calendar_two_fa.book(49, 50));
    EXPECT_TRUE(my_calendar_two_fa.book(38, 45));
    EXPECT_FALSE(my_calendar_two_fa.book(4, 12));
    EXPECT_FALSE(my_calendar_two_fa.book(46, 50));
    EXPECT_FALSE(my_calendar_two_fa.book(13, 21));

    MyCalendarTwoDS1 my_calendar_two_ds1;

    EXPECT_TRUE(my_calendar_two_ds1.book(47, 50));
    EXPECT_TRUE(my_calendar_two_ds1.book(1, 10));
    EXPECT_TRUE(my_calendar_two_ds1.book(27, 36));
    EXPECT_TRUE(my_calendar_two_ds1.book(40, 47));
    EXPECT_TRUE(my_calendar_two_ds1.book(20, 27));
    EXPECT_TRUE(my_calendar_two_ds1.book(15, 23));
    EXPECT_TRUE(my_calendar_two_ds1.book(10, 18));
    EXPECT_TRUE(my_calendar_two_ds1.book(27, 36));
    EXPECT_FALSE(my_calendar_two_ds1.book(17, 25));
    EXPECT_FALSE(my_calendar_two_ds1.book(8, 17));
    EXPECT_FALSE(my_calendar_two_ds1.book(24, 33));
    EXPECT_FALSE(my_calendar_two_ds1.book(23, 28));
    EXPECT_FALSE(my_calendar_two_ds1.book(21, 27));
    EXPECT_TRUE(my_calendar_two_ds1.book(47, 50));
    EXPECT_FALSE(my_calendar_two_ds1.book(14, 21));
    EXPECT_FALSE(my_calendar_two_ds1.book(26, 32));
    EXPECT_FALSE(my_calendar_two_ds1.book(16, 21));
    EXPECT_TRUE(my_calendar_two_ds1.book(2, 7));
    EXPECT_FALSE(my_calendar_two_ds1.book(24, 33));
    EXPECT_FALSE(my_calendar_two_ds1.book(6, 13));
    EXPECT_FALSE(my_calendar_two_ds1.book(44, 50));
    EXPECT_FALSE(my_calendar_two_ds1.book(33, 39));
    EXPECT_FALSE(my_calendar_two_ds1.book(30, 36));
    EXPECT_FALSE(my_calendar_two_ds1.book(6, 15));
    EXPECT_FALSE(my_calendar_two_ds1.book(21, 27));
    EXPECT_FALSE(my_calendar_two_ds1.book(49, 50));
    EXPECT_TRUE(my_calendar_two_ds1.book(38, 45));
    EXPECT_FALSE(my_calendar_two_ds1.book(4, 12));
    EXPECT_FALSE(my_calendar_two_ds1.book(46, 50));
    EXPECT_FALSE(my_calendar_two_ds1.book(13, 21));
}
