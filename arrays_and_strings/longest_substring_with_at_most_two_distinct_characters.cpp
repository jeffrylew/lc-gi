#include <gtest/gtest.h>

#include <algorithm>
#include <limits>
#include <string>
#include <unordered_map>
#include <utility>

//! @brief First attempt to get length of longest substring
//! @param[in] s String to search substring in
//! @return Length of longest substring that contains at most two distinct chars
static int lengthOfLongestSubstringTwoDistinctFA(std::string s)
{
    //! @details First attempt solution does not pass SampleTest4

    const auto s_len = static_cast<int>(std::ssize(s));

    int max_len {1};
    int left {};

    std::unordered_map<char, int> char_earliest_idx {};
    char_earliest_idx[s[left]] = 0;

    for (int right = 1; right < s_len; ++right)
    {
        const char prev_char {s[right - 1]};
        const char curr_char {s[right]};

        if (!char_earliest_idx.contains(curr_char))
        {
            char_earliest_idx[curr_char] = right;
        }

        if (std::ssize(char_earliest_idx) > 2)
        {
            left = char_earliest_idx[prev_char];

            for (const auto& [unique_char, earliest_idx] : char_earliest_idx)
            {
                if (unique_char == curr_char || unique_char == prev_char)
                {
                    continue;
                }

                if (earliest_idx > left)
                {
                    //! left = earliest_idx + 1 fails "ababaccccc"
                    //! Outputs 9 instead of 7
                    left = right - 1;
                    break;
                }
            }

            char_earliest_idx.clear();
            char_earliest_idx[prev_char] = left;
            char_earliest_idx[curr_char] = right;
        }

        max_len = std::max(max_len, right - left + 1);
    }

    return max_len;

} // static int lengthOfLongestSubstringTwoDistinctFA( ...

//! @brief Sliding window discussion solution
//! @param[in] s String to search substring in
//! @return Length of longest substring that contains at most two distinct chars
static int lengthOfLongestSubstringTwoDistinctDS(std::string s)
{
    //! @details https://leetcode.com/problems
    //!          /longest-substring-with-at-most-two-distinct-characters
    //!
    //!          Time complexity O(N) where N = number of chars in input string
    //!          Space complexity O(1) since additional space is used only for a
    //!          hashmap with at most 3 elements

    const auto s_len = static_cast<int>(std::ssize(s));
    if (s_len < 3)
    {
        return s_len;
    }

    int max_len {2};

    //! Sliding window left pointer
    int left {};

    //! Map of <char, rightmost position in sliding window>
    std::unordered_map<char, int> char_pos {};

    for (int right = 0; right < s_len; ++right)
    {
        //! Sliding window contains less than 3 chars
        char_pos[s[right]] = right;

        //! Sliding window contains 3 chars
        if (std::ssize(char_pos) == 3)
        {
            int del_idx {std::numeric_limits<int>::max()};

            for (const auto& char_idx : char_pos)
            {
                del_idx = std::min(del_idx, char_idx.second);
            }

            //! Delete leftmost character
            char_pos.erase(s[del_idx]);
            left = del_idx + 1;
        }

        max_len = std::max(max_len, right - left + 1);
    }

    return max_len;

} // static int lengthOfLongestSubstringTwoDistinctDS( ...

TEST(LengthOfLongestSubstringTwoDistinctTest, SampleTest1)
{
    EXPECT_EQ(3, lengthOfLongestSubstringTwoDistinctFA("eceba"));
    EXPECT_EQ(3, lengthOfLongestSubstringTwoDistinctDS("eceba"));
}

TEST(LengthOfLongestSubstringTwoDistinctTest, SampleTest2)
{
    EXPECT_EQ(5, lengthOfLongestSubstringTwoDistinctFA("ccaabbb"));
    EXPECT_EQ(5, lengthOfLongestSubstringTwoDistinctDS("ccaabbb"));
}

TEST(LengthOfLongestSubstringTwoDistinctTest, SampleTest3)
{
    EXPECT_EQ(1, lengthOfLongestSubstringTwoDistinctFA("a"));
    EXPECT_EQ(1, lengthOfLongestSubstringTwoDistinctDS("a"));
}

TEST(LengthOfLongestSubstringTwoDistinctTest, SampleTest4)
{
    EXPECT_NE(10, lengthOfLongestSubstringTwoDistinctFA("abccbbcccaaacaca"));
    EXPECT_EQ(14, lengthOfLongestSubstringTwoDistinctFA("abccbbcccaaacaca"));
    EXPECT_EQ(10, lengthOfLongestSubstringTwoDistinctDS("abccbbcccaaacaca"));
}
