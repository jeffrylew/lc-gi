#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

//! @brief First attempt to get number of query strings that are stretchy
//! @param[in] s     std::string to check query against
//! @param[in] words Vector of query strings to check stretchiness of
//! @return Number of query strings that are stretchy
static int expressiveWordsFA(std::string s, std::vector<std::string> words)
{
    //! @details https://leetcode.com/explore/interview/card/google/59
    //!          /array-and-strings/3056/
    //!
    //!          First attempt solution does not pass SampleTest1

    const auto is_stretchy = [&s](std::string_view word) {
        int s_idx {};
        int word_idx {};

        while (word_idx < std::ssize(word))
        {
            if (s[s_idx] == word[word_idx])
            {
                ++s_idx;
                ++word_idx;
                continue;
            }

            if (word_idx == 0)
            {
                return false;
            }

            if (std::ssize(s) < s_idx + 3)
            {
                return false;
            }

            if (std::all_of(s.begin() + s_idx,
                            s.begin() + s_idx + 4,
                            [&](char ch) {
                                return ch == word[word_idx - 1];
                            }))
            {
                s_idx += 4;
            }
            else
            {
                return false;
            }
        }

        return true;
    };

    int num_stretchy_strings {};

    for (const auto& word : words)
    {
        if (is_stretchy(word))
        {
            ++num_stretchy_strings;
        }
    }

    return num_stretchy_strings;

} // static int expressiveWordsFA( ...

TEST(ExpressiveWordsTest, SampleTest1)
{
    EXPECT_NE(1, expressiveWordsFA("heeellooo", {"hello", "hi", "helo"}));
    EXPECT_EQ(0, expressiveWordsFA("heeellooo", {"hello", "hi", "helo"}));
}

TEST(ExpressiveWordsTest, SampleTest2)
{
    EXPECT_EQ(3, expressiveWordsFA("zzzzzyyyyy", {"zzyy", "zy", "zyy"}));
}
