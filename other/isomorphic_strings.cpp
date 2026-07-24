#include <gtest/gtest.h>

#include <array>
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

//! @brief Character mapping with dictionary discussion solution
//! @param[in] s First string
//! @param[in] t Second string
//! @return True if characters in s can be replaced to get t
static bool isIsomorphicDS1(std::string s, std::string t)
{
    //! @details https://leetcode.com/problems/isomorphic-strings/editorial/

    std::array<int, 256U> map_s_to_t {};
    std::array<int, 256U> map_t_to_s {};

    for (int idx = 0; idx < std::ssize(s); ++idx)
    {
        const char ch_s {s[idx]};
        const char ch_t {t[idx]};

        //! Case 1: No mapping exists in either dictionary
        if (map_s_to_t[ch_s] == 0 && map_t_to_s[ch_t] == 0)
        {
            map_s_to_t[ch_s] = ch_t;
            map_t_to_s[ch_t] = ch_s;
        }
        else if (map_s_to_t[ch_s] != ch_t || map_t_to_s[ch_t] != ch_s)
        {
            //! Case 2: Either mapping doesn't exist in one of the dictionaries
            //!         or mapping exists but doesn't match in either dictionary
            return false;
        }
    }

    return true;
}

TEST(IsIsomorphicTest, SampleTest1)
{
    EXPECT_TRUE(isIsomorphicFA("egg", "add"));
    EXPECT_TRUE(isIsomorphicDS1("egg", "add"));
}

TEST(IsIsomorphicTest, SampleTest2)
{
    EXPECT_FALSE(isIsomorphicFA("f11", "b23"));
    EXPECT_FALSE(isIsomorphicDS1("f11", "b23"));
}

TEST(IsIsomorphicTest, SampleTest3)
{
    EXPECT_TRUE(isIsomorphicFA("paper", "title"));
    EXPECT_TRUE(isIsomorphicDS1("paper", "title"));
}

TEST(IsIsomorphicTest, SampleTest4)
{
    EXPECT_FALSE(isIsomorphicFA("badc", "baba"));
    EXPECT_FALSE(isIsomorphicDS1("badc", "baba"));
}
