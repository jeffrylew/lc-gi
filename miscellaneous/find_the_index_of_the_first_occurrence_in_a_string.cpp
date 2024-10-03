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
    //!
    //!          Time complexity O(N * M) where N = haystack.size() and
    //!          M = needle.size(). For every winow_start, we have to iterate at
    //!          most M times. There are N - M + 1 such window_starts. Thus, we
    //!          have O((N - M + 1) * M), which is O(N * M). An example of the
    //!          worst case is when needle is "aaaaab" and haystack is all a's
    //!          (e.g. "aaaaaaaaaa"). We always have to check the last character
    //!          of needle to conclude the current M-substring is not equal to
    //!          needle. Thus, have to iterate M times for every window_start.
    //!          Space complexity O(1) for a handful of constant space variables

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
