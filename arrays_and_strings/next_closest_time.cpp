#include <gtest/gtest.h>

#include <set>
#include <string>

//! @brief First attempt to form next closest time by reusing current digits
//! @param[in] time std::string containing a time in the format "HH:MM"
//! @return st::string containing the next closest time by reusing digits
static std::string nextClosestTimeFA(std::string time)
{
    //! @details First attempt solution does not pass SampleTest4

    const char h1 {time[0]};
    const char h2 {time[1]};
    const char m1 {time[3]};
    const char m2 {time[4]};

    const std::set<char> digits {h1, h2, m1, m2};

    //! Get next digit larger than m2
    auto next_m2_it = digits.upper_bound(m2);
    if (next_m2_it != digits.end())
    {
        time[4] = *next_m2_it;
        return time;
    }

    //! m2 is the largest so set it to the smallest digit
    time[4] = *digits.begin();

    //! Update m1
    auto next_m1_it = digits.upper_bound(m1);
    if (next_m1_it != digits.end())
    {
        if (*next_m1_it < 6)
        {
            time[3] = *next_m1_it;
        }
        else
        {
            time[0] = *digits.begin();
            time[1] = *digits.begin();
            time[3] = *digits.begin();
        }

        return time;
    }
    else
    {
        time[3] = *digits.begin();
    }

    //! Update h2
    auto next_h2_it = digits.upper_bound(h2);
    if (next_h2_it != digits.end())
    {
        time[1] = *next_h2_it;
    }
    else
    {
        time[0] = *digits.begin();
        time[1] = *digits.begin();
    }

    return time;

} // static std::string nextClosestTimeFA( ...

TEST(NextClosestTimeTest, SampleTest1)
{
    EXPECT_EQ("19:39", nextClosestTimeFA("19:34"));
}

TEST(NextClosestTimeTest, SampleTest2)
{
    EXPECT_EQ("22:22", nextClosestTimeFA("23:59"));
}

TEST(NextClosestTimeTest, SampleTest3)
{
    EXPECT_EQ("15:11", nextClosestTimeFA("13:55"));
}

TEST(NextClosestTimeTest, SampleTest4)
{
    EXPECT_NE("22:32", nextClosestTimeFA("22:23"));
    EXPECT_EQ("22:22", nextClosestTimeFA("22:23"));
}
