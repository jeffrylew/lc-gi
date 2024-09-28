#include <gtest/gtest.h>

#include <string>

//! @brief First attempt to get index of needle in haystack
//! @param[in] haystack std::string to search for needle in
//! @param[in] needle   std::string to find occurrence of
//! @return Index of first occurrence of needle in haystack
static int strStrFA(std::string haystack, std::string needle)
{
    const auto pos = haystack.find(needle);
    return pos == std::string::npos ? -1 : static_cast<int>(pos);
}

TEST(StrStrTest, SampleTest1)
{
    EXPECT_EQ(0, strStrFA("sadbutsad", "sad"));
}

TEST(StrStrTest, SampleTest2)
{
    EXPECT_EQ(-1, strStrFA("leetcode", "leeto"));
}
