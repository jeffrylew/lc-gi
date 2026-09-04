#include <gtest/gtest.h>

//! @class MyCalendarTwoFA
//! @brief First attempt to add events that don't cause triple bookings
//! @details https://leetcode.com/explore/interview/card/google/66/others-4/3101
class MyCalendarTwoFA
{
public:
    MyCalendarTwoFA()
    {

    }

    //! @brief Check if event can be added without causing a triple booking
    //! @param[in]  startTime Event start time, inclusive
    //! @param[out] endTime   Event stop time, exclusive
    //! @return True if the event can be added without causing a triple booking
    bool book(int startTime, int endTime)
    {
        //! @todo
    }
};

TEST(MyCalendarTwoTest, SampleTest1)
{
    MyCalendarTwoFA my_calendar_two_fa;

    EXPECT_TRUE(my_calendar_two_fa.book(10, 20)); // Event can be booked
    EXPECT_TRUE(my_calendar_two_fa.book(50, 60)); // Event can be booked
    EXPECT_TRUE(my_calendar_two_fa.book(10, 40)); // Event can be double booked
    EXPECT_FALSE(my_calendar_two_fa.book(5, 15)); // Results in triple booking
    EXPECT_TRUE(my_calendar_two_fa.book(5, 10));  // Excludes double booked 10

    //! The time in [25, 40) will be double booked with the third event
    //! The time [40, 50) will be single booked
    //! The time [50, 55) will be double booked with the second event
    EXPECT_TRUE(my_calendar_two_fa.book(25, 55));
}
