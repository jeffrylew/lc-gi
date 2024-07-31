#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <vector>

//! @brief Three-pass count method discussion solution
//! @param[in] s std::string containing 'a' or 'b'
//! @return Min number of deletions to make s balanced
static int minimumDeletionsDS1(std::string s)
{
    //! @details https://leetcode.com/problems
    //!          /minimum-deletions-to-make-string-balanced
    //!
    //!          Time complexity O(N) where N = s.size(). THe algorithm performs
    //!          three linear passes over the string.
    //!          Space complexity O(N). Use two vectors of size N to store count

    const auto s_len = static_cast<int>(std::ssize(s));

    std::vector<int> count_a(s_len, 0);
    std::vector<int> count_b(s_len, 0);

    //! First pass: Compute count_b, which stores the number
    //! of 'b' characters to the left of the current position
    int b_count {};
    for (int idx = 0; idx < s_len; ++idx)
    {
        count_b[idx] = b_count;
        if (s[idx] == 'b')
        {
            ++b_count;
        }
    }

    //! Second pass: Compute count_a, which stores the number
    //! of 'a' characters to the right of the current position
    int a_count {};
    for (int idx = s_len - 1; idx >= 0; --idx)
    {
        count_a[idx] = a_count;
        if (s[idx] == 'a')
        {
            ++a_count;
        }
    }

    //! Third pass: Iterate through string to find min deletions
    int min_deletions {s_len};
    for (int idx = 0; idx < s_len; ++idx)
    {
        min_deletions = std::min(min_deletions, count_a[idx] + count_b[idx]);
    }

    return min_deletions;

} // static int minimumDeletionsDS1( ...

TEST(MinimumDeletionsTest, SampleTest1)
{
    EXPECT_EQ(2, minimumDeletionsDS1("aababbab"));
}

TEST(MinimumDeletionsTest, SampleTest2)
{
    EXPECT_EQ(2, minimumDeletionsDS1("bbaaaaabbb"));
}
