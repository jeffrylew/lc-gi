#include <gtest/gtest.h>

#include <algorithm>
#include <limits>
#include <string>
#include <unordered_map>

//! @brief First attempt to get min window substring of s
//! @param[in] s String to get min window substring from
//! @param[in] t String where every character including duplicates is in window
//! @return Min window substring of s or empty string if no such window
static std::string minWindowFA(std::string s, std::string t)
{
    //! @details https://leetcode.com/problems/minimum-window-substring
    //!
    //!          First attempt solution does get correct answer in SampleTest5

    const auto s_size = static_cast<int>(std::ssize(s));
    const auto t_size = static_cast<int>(std::ssize(t));

    if (t_size > s_size)
    {
        return {};
    }

    //! Count of each letter in t
    std::unordered_map<char, int> t_char_count {};
    for (const char letter : t)
    {
        ++t_char_count[letter];
    }

    //! Min window substring size with left and right index bounds
    int min_substr_size {std::numeric_limits<int>::max()};
    int min_substr_left {};
    int min_substr_right {};

    //! Current left index of window
    int left {};

    //! Current number of unused characters from t
    int total_t_count {t_size};

    for (int right = 0; right < s_size; ++right)
    {
        auto right_it = t_char_count.find(s[right]);

        if (right_it == t_char_count.end() && total_t_count == t_size)
        {
            //! Move onto next char in s. New window should end with a char in t
            continue;
        }

        if (right_it != t_char_count.end())
        {
            if (right_it->second > 0)
            {
                //! s[right] is char in t with non-zero count, decrement counts
                --(right_it->second);
                --total_t_count;
            }
            else if (right_it->first == s[left])
            {
                //! Char at left index in s is the same as char at right index
                //! Move left index to another char in t. Don't need to update
                //! t_char_count or total_t_count since state of counts from
                //! left index is transferred to right index
                ++left;
                while (left <= right && !t_char_count.contains(s[left]))
                {
                    ++left;
                }
            }
        }

        //! Found a window substring of s that uses all chars in t
        if (total_t_count == 0)
        {
            //! Ensure left index is a char in t since we want our current
            //! window substring to start with a char in t
            while (!t_char_count.contains(s[left]))
            {
                ++left;
            }

            //! If found a new min window substring, update parameters
            const int current_window_size {right - left + 1};
            if (current_window_size < min_substr_size)
            {
                min_substr_size  = current_window_size;
                min_substr_left  = left;
                min_substr_right = right;
            }

            //! Before incrementing left index, increment count of s[left] in
            //! t_char_count map and increment the total_t_count
            ++t_char_count[s[left]];
            ++total_t_count;
            ++left;

            //! Now move left index to another char in t since we want the
            //! next potential window substring to begin with a char in t
            while (left <= right && !t_char_count.contains(s[left]))
            {
                ++left;
            }
        }
    }

    if (min_substr_size == std::numeric_limits<int>::max())
    {
        return {};
    }

    return s.substr(min_substr_left, min_substr_size);

} // static std::string minWindowFA( ...

TEST(MinWindowTest, SampleTest1)
{
    EXPECT_EQ("BANC", minWindowFA("ADOBECODEBANC", "ABC"));
}

TEST(MinWindowTest, SampleTest2)
{
    EXPECT_EQ("a", minWindowFA("a", "a"));
}

TEST(MinWindowTest, SampleTest3)
{
    EXPECT_TRUE(minWindowFA("a", "aa").empty());
}

TEST(MinWindowTest, SampleTest4)
{
    EXPECT_TRUE(minWindowFA("a", "b").empty());
}

TEST(MinWindowTest, SampleTest5)
{
    EXPECT_NE("baca", minWindowFA("acbbaca", "aba"));
    EXPECT_EQ("acbba", minWindowFA("acbbaca", "aba"));
}
