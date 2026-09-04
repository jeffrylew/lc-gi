#include <gtest/gtest.h>

#include <algorithm>
#include <utility>
#include <vector>

//! @brief Check if bookings [start1, end1) and [start2, end2) overlap
//! @param[in] start1 Start time (inclusive) of first interval
//! @param[in] end1   End time (exclusive) of first interval
//! @param[in] start2 Start time (inclusive) of second interval
//! @param[in] end2   End time (exclusive) of second interval
//! @return True if the bookings overlap, else false
constexpr bool overlaps(int start1, int end1, int start2, int end2)
{
    return std::max(start1, start2) < std::min(end1, end2);
}

//! @brief Get overlapping booking between [start1, end1) and [start2, end2)
//! @param[in] start1 Start time (inclusive) of first interval
//! @param[in] end1   End time (exclusive) of first interval
//! @param[in] start2 Start time (inclusive) of second interval
//! @param[in] end2   End time (exclusive) of second interval
//! @return Overlapping booking between the input intervals
constexpr std::pair<int, int>
    get_overlap(int start1, int end1, int start2, int end2)
{
    return {std::max(start1, start2), std::min(end1, end2)};
}

//! @class MyCalendarTwoDS1
//! @brief Using overlapped intervals discussion solution
//! @details https://leetcode.com/explore/interview/card/google/66/others-4/3101
class MyCalendarTwoDS1
{
public:
    //! @brief Check if event can be added without causing a triple booking
    //! @param[in]  startTime Event start time, inclusive
    //! @param[out] endTime   Event stop time, exclusive
    //! @return True if the event can be added without causing a triple booking
    bool book(int startTime, int endTime)
    {
        //! Check if new booking overlaps with an existing double booking
        for (const auto& [two_booked_start, two_booked_end] : double_bookings)
        {
            if (overlaps(two_booked_start, two_booked_end, startTime, endTime))
            {
                return false;
            }
        }

        //! Add double booking if new booking overlaps with single booking
        for (const auto& [one_booked_start, one_booked_end] : single_bookings)
        {
            if (overlaps(one_booked_start, one_booked_end, startTime, endTime))
            {
                auto booking_overlap = get_overlap(one_booked_start,
                                                   one_booked_end,
                                                   startTime,
                                                   endTime);
                double_bookings.push_back(std::move(booking_overlap));
            }
        }

        //! Add the booking to the vector of single bookings
        single_bookings.emplace_back(startTime, endTime);
        return true;
    }

private:
    //! Vector of <startTime, endTime> for single bookings
    std::vector<std::pair<int, int>> single_bookings;

    //! Vector of <startTime, endTime> for double bookings
    std::vector<std::pair<int, int>> double_bookings;    
};

TEST(MyCalendarTwoTest, SampleTest1)
{
    MyCalendarTwoDS1 my_calendar_two_ds1;

    EXPECT_TRUE(my_calendar_two_ds1.book(10, 20)); // Event can be booked
    EXPECT_TRUE(my_calendar_two_ds1.book(50, 60)); // Event can be booked
    EXPECT_TRUE(my_calendar_two_ds1.book(10, 40)); // Event can be double booked
    EXPECT_FALSE(my_calendar_two_ds1.book(5, 15)); // Results in triple booking
    EXPECT_TRUE(my_calendar_two_ds1.book(5, 10));  // Excludes double booked 10

    //! The time in [25, 40) will be double booked with the third event
    //! The time [40, 50) will be single booked
    //! The time [50, 55) will be double booked with the second event
    EXPECT_TRUE(my_calendar_two_ds1.book(25, 55));
}
