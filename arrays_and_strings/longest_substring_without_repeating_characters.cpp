#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <tuple>
#include <unordered_set>

//! @brief Find length of longest substring without repeating characters
//! @param[in] s std::string to find substring in
//! @return Length of longest substring in s without repeating characters
static int lengthOfLongestSubstringFA(std::string s)
{
    //! @details https://leetcode.com/problems/
    //!          longest-substring-without-repeating-characters
    //!
    //!          First attempt solution does not pass SampleTest4

    if (s.empty())
    {
        return 0;
    }

    const auto s_len = static_cast<int>(std::ssize(s));
    if (s_len == 1)
    {
        return 1;
    }

    int max_len {};
    int pos_left {};

    std::unordered_set<char> seen {};
    std::ignore = seen.insert(s.front());

    for (int pos_right = 1; pos_right < s_len; ++pos_right)
    {
        if (seen.contains(s[pos_right]))
        {
            while (pos_left < pos_right)
            {
                seen.erase(s[pos_left++]);
            }
        }

        std::ignore = seen.insert(s[pos_right]);
        max_len     = std::max(max_len, pos_right - pos_left + 1);
    }

    return max_len;
}

TEST(LengthOfLongestSubstringTest, SampleTest1)
{
    EXPECT_EQ(3, lengthOfLongestSubstringFA("abcabcbb"));
}

TEST(LengthOfLongestSubstringTest, SampleTest2)
{
    EXPECT_EQ(1, lengthOfLongestSubstringFA("bbbbb"));
}

TEST(LengthOfLongestSubstringTest, SampleTest3)
{
    EXPECT_EQ(3, lengthOfLongestSubstringFA("pwwkew"));
}

TEST(LengthOfLongestSubstringTest, SampleTest4)
{
    EXPECT_EQ(2, lengthOfLongestSubstringFA("dvdf"));
    EXPECT_NE(3, lengthOfLongestSubstringFA("dvdf"));
}
