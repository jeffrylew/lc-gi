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

//! @brief Sliding window discussion solution
//! @param[in] s String to get min window substring from
//! @param[in] t String where every character including duplicates is in window
//! @return Min window substring of s or empty string if no such window
static std::string minWindowDS1(std::string s, std::string t)
{
    //! @details https://leetcode.com/problems/minimum-window-substring
    //!
    //!          Time complexity O(S + T) where S = s.size() and T = t.size().
    //!          In the worst case, might visit every element of s twice, once
    //!          by left and once by right. Need O(T) to build t_char_count map.
    //!          Space complexity O(S + T). Need O(S) when window size is equal
    //!          to entire string s. Need O(T) when t only has unique chars.

    if (s.empty() || t.empty())
    {
        return {};
    }

    const auto s_size = static_cast<int>(std::ssize(s));
    const auto t_size = static_cast<int>(std::ssize(t));

    //! unordered_map keeps count of all unique chars in t
    std::unordered_map<char, int> t_char_count {};
    for (const char letter : t)
    {
        ++t_char_count[letter];
    }

    //! Number of unique chars in t that need to be present in desired window
    const auto required = static_cast<int>(std::ssize(t_char_count));

    //! left index of current window
    int left {};

    //! formed keeps track of how many unique chars from t are in current window
    //! taking into account the desired frequency
    int formed {};

    //! Map keeps count of all unique chars in current window
    std::unordered_map<char, int> window_counts {};

    //! Properties of min window substring
    int ans_window_size {-1};
    int ans_left {};
    int ans_right {};

    //! Keep expanding the window
    for (int right = 0; right < s_size; ++right)
    {
        //! Add one char from right to window
        char ch {s[right]};
        ++window_counts[ch];

        //! If frequency of current char added equals to
        //! desired count in t then increment formed count
        if (t_char_count.contains(ch) && window_counts[ch] == t_char_count[ch])
        {
            ++formed;
        }

        //! Contract window until it is no longer desirable
        while (left <= right && formed == required)
        {
            ch = s[left];

            //! Save smallest window until now
            const int current_window_size {right - left + 1};
            if (ans_window_size == -1 || current_window_size < ans_window_size)
            {
                ans_window_size = current_window_size;
                ans_left        = left;
                ans_right       = right;
            }

            //! Char at left index is no longer a part of window
            --window_counts[ch];
            if (t_char_count.contains(ch)
                && window_counts[ch] < t_char_count[ch])
            {
                --formed;
            }

            //! Increment left point to look for a new window
            ++left;

        } // while (left <= right && ...

    } // for (int right = 0; ...

    return ans_window_size == -1
        ? std::string {}
        : s.substr(ans_left, ans_right - ans_left + 1);

} // static std::string minWindowDS1( ...

TEST(MinWindowTest, SampleTest1)
{
    EXPECT_EQ("BANC", minWindowFA("ADOBECODEBANC", "ABC"));
    EXPECT_EQ("BANC", minWindowDS1("ADOBECODEBANC", "ABC"));
}

TEST(MinWindowTest, SampleTest2)
{
    EXPECT_EQ("a", minWindowFA("a", "a"));
    EXPECT_EQ("a", minWindowDS1("a", "a"));
}

TEST(MinWindowTest, SampleTest3)
{
    EXPECT_TRUE(minWindowFA("a", "aa").empty());
    EXPECT_TRUE(minWindowDS1("a", "aa").empty());
}

TEST(MinWindowTest, SampleTest4)
{
    EXPECT_TRUE(minWindowFA("a", "b").empty());
    EXPECT_TRUE(minWindowDS1("a", "b").empty());
}

TEST(MinWindowTest, SampleTest5)
{
    EXPECT_NE("baca", minWindowFA("acbbaca", "aba"));
    EXPECT_EQ("acbba", minWindowFA("acbbaca", "aba"));
    EXPECT_EQ("baca", minWindowDS1("acbbaca", "aba"));
}
