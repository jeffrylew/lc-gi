#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <unordered_map>

//! @brief First attempt to get length of longest substring
//! @param[in] s std::string to search for longest substring in
//! @param[in] k Number of distinct chars at most that substring can have
//! @return Length of longest substring that contains at most k distinct chars
static int lengthOfLongestSubstringKDistinctFA(std::string s, int k)
{
    //! @details https://leetcode.com/problems
    //!          /longest-substring-with-at-most-k-distinct-characters
    //!
    //!          Time complexity O(N) where N = s.size(). The left and right
    //!          pointers can move O(N) at most each. Searching for the least
    //!          recently seen char in the char_pos map takes O(k) where k <= N.
    //!          Thus, searching performed within the for loop is amortized O(1)
    //!          Space complexity O(k) since the char_pos map will contain at
    //!          most k chars.

    if (k == 0)
    {
        return 0;
    }

    const auto s_size = static_cast<int>(std::ssize(s));
    if (s_size < k)
    {
        return s_size;
    }

    //! Length of longest substring that contains at most k distinct chars
    int max_len {};

    //! Index of left boundary of substring
    int left {};

    //! Map of distinct char and right-most occurrence in s
    std::unordered_map<char, int> char_pos {};

    for (int right = 0; right < s_size; ++right)
    {
        char_pos[s[right]] = right;

        if (std::size(char_pos) > k)
        {
            auto min_pos_it =
                std::min_element(char_pos.begin(),
                                 char_pos.end(),
                                 [](const auto& lhs, const auto& rhs) {
                                    return lhs.second < rhs.second;
                                 });

            //! left boundary starts after least recently seen char so add 1
            left = min_pos_it->second + 1;

            //! Remove least recently seen char from map
            char_pos.erase(min_pos_it);
        }

        max_len = std::max(max_len, right - left + 1);
    }

    return max_len;

} // static int lengthOfLongestSubstringKDistinctFA( ...

TEST(LengthOfLongestSubstringKDistinctTest, SampleTest1)
{
    EXPECT_EQ(3, lengthOfLongestSubstringKDistinctFA("eceba", 2));
}

TEST(LengthOfLongestSubstringDistinctTest, SampleTest2)
{
    EXPECT_EQ(2, lengthOfLongestSubstringKDistinctFA("aa", 1));
}
