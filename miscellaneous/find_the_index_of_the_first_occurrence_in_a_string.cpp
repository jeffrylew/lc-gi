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

//! @brief Sliding window discussion solution
//! @param[in] haystack std::string to search for needle in
//! @param[in] needle   std::string to find occurrence of
//! @return Index of first occurrence of needle in haystack
static int strStrDS1(std::string haystack, std::string needle)
{
    //! @details https://leetcode.com/problems
    //!          /find-the-index-of-the-first-occurrence-in-a-string

    const auto haystack_size = static_cast<int>(std::ssize(haystack));
    const auto needle_size   = static_cast<int>(std::ssize(needle));

    for (int window_start = 0;
         window_start <= haystack_size - needle_size;
         ++window_start)
    {
        for (int idx = 0; idx < needle_size; ++idx)
        {
            if (needle[idx] != haystack[window_start + idx])
            {
                break;
            }

            if (idx == needle_size - 1)
            {
                return window_start;
            }
        }
    }

    return -1;

} // static int strStrDS1( ...

TEST(StrStrTest, SampleTest1)
{
    EXPECT_EQ(0, strStrFA("sadbutsad", "sad"));
    EXPECT_EQ(0, strStrDS1("sadbutsad", "sad"));
}

TEST(StrStrTest, SampleTest2)
{
    EXPECT_EQ(-1, strStrFA("leetcode", "leeto"));
    EXPECT_EQ(-1, strStrDS1("leetcode", "leeto"));
}
