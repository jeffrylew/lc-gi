#include <gtest/gtest.h>

#include <string>
#include <unordered_map>

//! @brief First attempt to determine if strings are isomorphic
//! @param[in] s First string
//! @param[in] t Second string
//! @return True if characters in s can be replaced to get t
static bool isIsomorphicFA(std::string s, std::string t)
{
    //! @details leetcode.com/explore/featured/card/google/66/others-4/3098
    //!
    //!          Time complexity O(N) where N = s.size() = t.size().
    //!          Space complexity O(N) for the two maps.

    //! Map of char in s to char in t
    std::unordered_map<char, char> s_to_t_map;

    //! Map of char in t to char in s
    std::unordered_map<char, char> t_to_s_map;

    int t_idx {};
    for (const char s_ch : s)
    {
        const char t_ch {t[t_idx]};
        ++t_idx;

        auto [s_ch_it, s_ch_was_inserted] = s_to_t_map.try_emplace(s_ch, t_ch);
        if (!s_ch_was_inserted && s_ch_it->second != t_ch)
        {
            return false;
        }

        auto [t_ch_it, t_ch_was_inserted] = t_to_s_map.try_emplace(t_ch, s_ch);
        if (!t_ch_was_inserted && t_ch_it->second != s_ch)
        {
            return false;
        }
    }

    return true;
}

TEST(IsIsomorphicTest, SampleTest1)
{
    EXPECT_TRUE(isIsomorphicFA("egg", "add"));
}

TEST(IsIsomorphicTest, SampleTest2)
{
    EXPECT_FALSE(isIsomorphicFA("f11", "b23"));
}

TEST(IsIsomorphicTest, SampleTest3)
{
    EXPECT_TRUE(isIsomorphicFA("paper", "title"));
}

TEST(IsIsomorphicTest, SampleTest4)
{
    EXPECT_FALSE(isIsomorphicFA("badc", "baba"));
}
