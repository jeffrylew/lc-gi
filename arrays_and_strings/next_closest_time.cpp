#include <gtest/gtest.h>

#include <string>

//! @brief First attempt to form next closest time by reusing current digits
//! @param[in] time std::string containing a time in the format "HH:MM"
//! @return st::string containing the next closest time by reusing digits
static std::string nextClosestTimeFA(std::string time)
{
    //! @todo

} // static std::string nextClosestTimeFA( ...

TEST(NextClosestTimeTest, SampleTest1)
{
    EXPECT_EQ("19:39", nextClosestTimeFA("19:34"));
}

TEST(NextClosestTimeTest, SampleTest2)
{
    EXPECT_EQ("22:22", nextClosestTimeFA("23:59"));
}
