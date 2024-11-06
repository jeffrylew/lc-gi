#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <string_view>
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

//! @brief Helper function verifies if a valid substring of substr_size exists
static bool is_valid_DS1(std::string_view s, int substr_size, int k)
{
    //! Count distinct chars in substring from [right - substr_size, right - 1]
    std::unordered_map<char, int> char_count_in_substr {};

    //! substr_size will be in [k, s.size()]
    for (int idx = 0; idx < substr_size; ++idx)
    {
        ++char_count_in_substr[s[idx]];
    }

    if (std::ssize(char_count_in_substr) <= k)
    {
        return true;
    }

    //! Iterate right index from substr_size to s.size() - 1
    for (int right = substr_size; right < std::ssize(s); ++right)
    {
        ++char_count_in_substr[s[right]];

        const char prev_left_bound_char {s[right - substr_size]};
        --char_count_in_substr[prev_left_bound_char];

        if (char_count_in_substr[prev_left_bound_char] == 0)
        {
            char_count_in_substr.erase(prev_left_bound_char);
        }

        if (std::ssize(char_count_in_substr) <= k)
        {
            return true;
        }
    }

    return false;

} // static bool is_valid_DS1( ...

//! @brief Binary Search + Fixed Size Sliding Window discussion solution
//! @param[in] s std::string to search for longest substring in
//! @param[in] k Number of distinct chars at most that substring can have
//! @return Length of longest substring that contains at most k distinct chars
static int lengthOfLongestSubstringKDistinctDS1(std::string s, int k)
{
    //! @details https://leetcode.com/problems
    //!          /longest-substring-with-at-most-k-distinct-characters/editorial
    //!
    //!          Time complexity O(N * log N) where N = s.size(). Set the search
    //!          space as [k, N] so it takes at most O(log N) for binary search.
    //!          At each step, iterate over s which takes O(N).
    //!          Space complexity O(N). Use hash map char_count_in_substr which
    //!          contains at most O(N) distinct chars.

    const auto s_len = std::ssize(s);
    if (k >= s_len)
    {
        return s_len;
    }

    int min_substr_size {k};
    int max_substr_size {s_len};

    while (min_substr_size < max_substr_size)
    {
        const int mid_substr_size {
            min_substr_size + (max_substr_size - min_substr_size + 1) / 2};

        if (is_valid_DS1(s, mid_substr_size, k))
        {
            min_substr_size = mid_substr_size;
        }
        else
        {
            max_substr_size = mid_substr_size - 1;
        }
    }

    return min_substr_size;

} // static int lengthOfLongestSubstringKDistinctDS1( ...

TEST(LengthOfLongestSubstringKDistinctTest, SampleTest1)
{
    EXPECT_EQ(3, lengthOfLongestSubstringKDistinctFA("eceba", 2));
    EXPECT_EQ(3, lengthOfLongestSubstringKDistinctDS1("eceba", 2));
}

TEST(LengthOfLongestSubstringDistinctTest, SampleTest2)
{
    EXPECT_EQ(2, lengthOfLongestSubstringKDistinctFA("aa", 1));
    EXPECT_EQ(2, lengthOfLongestSubstringKDistinctDS1("aa", 1));
}
