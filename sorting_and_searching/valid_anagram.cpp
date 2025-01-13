#include <gtest/gtest.h>

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



} // static bool isAnagramFA( ...

TEST(IsAnagramTest, SampleTest1)
{
    EXPECT_TRUE(isAnagramFA("anagram", "nagaram"));
}

TEST(IsAnagramTest, SampleTest2)
{
    EXPECT_FALSE(isAnagramFA("rat", "car"));
}
