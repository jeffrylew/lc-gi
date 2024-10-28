#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <unordered_map>

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

TEST(LengthOfLongestSubstringTwoDistinctTest, SampleTest1)
{
    EXPECT_EQ(3, lengthOfLongestSubstringTwoDistinctFA("eceba"));
}

TEST(LengthOfLongestSubstringTwoDistinctTest, SampleTest2)
{
    EXPECT_EQ(5, lengthOfLongestSubstringTwoDistinctFA("ccaabbb"));
}

TEST(LengthOfLongestSubstringTwoDistinctTest, SampleTest3)
{
    EXPECT_EQ(1, lengthOfLongestSubstringTwoDistinctFA("a"));
}

TEST(LengthOfLongestSubstringTwoDistinctTest, SampleTest4)
{
    EXPECT_NE(10, lengthOfLongestSubstringTwoDistinctFA("abccbbcccaaacaca"));
    EXPECT_EQ(14, lengthOfLongestSubstringTwoDistinctFA("abccbbcccaaacaca"));
}
