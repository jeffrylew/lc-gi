#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <string>
#include <unordered_map>

//! @brief First attempt to check if t is an anagram of s, else false
//! @param[in] s First string
//! @param[in] t Second string to check if is an anagram
//! @return True if t is an anagram of s, else false
static bool isAnagramFA(std::string s, std::string t)
{
    //! @details https://leetcode.com/explore/interview/card/google/63
    //!          /sorting-and-searching-4/3082/
    //!
    //!          Time complexity O(N) where N = s.size()
    //!          Space complexity O(N)

    std::unordered_map<char, int> s_char_counts {};

    for (const char ch : s)
    {
        ++s_char_counts[ch];
    }

    for (const char ch : t)
    {
        if (s_char_counts.empty() || !s_char_counts.contains(ch))
        {
            return false;
        }

        if (--s_char_counts[ch] == 0)
        {
            s_char_counts.erase(ch);
        }
    }

    return s_char_counts.empty();

} // static bool isAnagramFA( ...

//! @brief Sorting discussion solution
//! @param[in] s First string
//! @param[in] t Second string to check if is an anagram
//! @return True if t is an anagram of s, else false
static bool isAnagramDS1(std::string s, std::string t)
{
    //! @details https://leetcode.com/problems/valid-anagram/editorial/
    //!
    //!          Time complexity O(N * log N) where N = s.size(). Sorting costs
    //!          O(N * log N) and comparing two strings costs O(N).
    //!          Space complexity O(1) depending on the implementation.

    if (std::ssize(s) != std::ssize(t))
    {
        return false;
    }

    std::sort(s.begin(), s.end());
    std::sort(t.begin(), t.end());

    return s == t;

} // static bool isAnagramDS1( ...

//! @brief Frequency counter discussion solution
//! @param[in] s First string
//! @param[in] t Second string to check if is an anagram
//! @return True if t is an anagram of s, else false
static bool isAnagramDS2(std::string s, std::string t)
{
    //! @details https://leetcode.com/problems/valid-anagram/editorial/
    //!
    //!          Time complexity O(N) because accessing char_count is a constant
    //!          time operation.
    //!          Space complexity O(1) since the size of char_count is fixed.

    if (std::ssize(s) != std::ssize(t))
    {
        return false;
    }

    std::array<int, 26> char_count {};
    for (const char ch : s)
    {
        ++char_count[static_cast<std::size_t>(ch - 'a')];
    }

    for (const char ch : t)
    {
        const auto ch_idx = static_cast<std::size_t>(ch - 'a');

        --char_count[ch_idx];
        if (char_count[ch_idx] < 0)
        {
            return false;
        }
    }

    return true;

} // static bool isAnagramDS2( ...

TEST(IsAnagramTest, SampleTest1)
{
    EXPECT_TRUE(isAnagramFA("anagram", "nagaram"));
    EXPECT_TRUE(isAnagramDS1("anagram", "nagaram"));
    EXPECT_TRUE(isAnagramDS2("anagram", "nagaram"));
}

TEST(IsAnagramTest, SampleTest2)
{
    EXPECT_FALSE(isAnagramFA("rat", "car"));
    EXPECT_FALSE(isAnagramDS1("rat", "car"));
    EXPECT_FALSE(isAnagramDS2("rat", "car"));
}
