#include <gtest/gtest.h>

#include <string>

//! @brief First attempt to get length of longest substring
//! @param[in] s std::string to search for longest substring in
//! @param[in] k Number of distinct chars at most that substring can have
//! @return Length of longest substring that contains at most k distinct chars
static int lengthOfLongestSubstringKDistinctFA(std::string s, int k)
{
    //! @todo

} // static int lengthOfLongestSubstringKDistinctFA( ...

TEST(LengthOfLongestSubstringKDistinctTest, SampleTest1)
{
    EXPECT_EQ(3, lengthOfLongestSubstringKDistinctFA("eceba", 2));
}

TEST(LengthOfLongestSubstringDistinctTest, SampleTest2)
{
    EXPECT_EQ(2, lengthOfLongestSubstringKDistinctFA("aa", 1));
}
