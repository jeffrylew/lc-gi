#include <gtest/gtest.h>

#include <set>
#include <string>

//! @brief First attempt to form next closest time by reusing current digits
//! @param[in] time std::string containing a time in the format "HH:MM"
//! @return st::string containing the next closest time by reusing digits
static std::string nextClosestTimeFA(std::string time)
{
    const char h1 {time[0]};
    const char h2 {time[1]};
    const char m1 {time[3]};
    const char m2 {time[4]};

    const std::set<char> digits {h1, h2, m1, m2};

    //! Get next digit larger than m2 or set to smallest digit if m2 is largest
    auto next_m2_it = digits.upper_bound(m2);
    char next_m2 {next_m2_it != digits.end() ? *next_m2_it :*digits.begin()};

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
