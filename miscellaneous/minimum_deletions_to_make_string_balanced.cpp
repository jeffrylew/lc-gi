#include <gtest/gtest.h>

#include <algorithm>
#include <stack>
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

//! @brief Combined pass discussion solution
//! @param[in] s std::string containing 'a' or 'b'
//! @return Min number of deletions to make s balanced
static int minimumDeletionsDS2(std::string s)
{
    //! @details https://leetcode.com/problems
    //!          /minimum-deletions-to-make-string-balanced
    //!
    //!          Time complexity O(N) where N = s.size(). The algorithm performs
    //!          two linear passes over the string.
    //!          Space complexity O(N). Use one vector of size N to store counts

    const auto s_len = static_cast<int>(std::ssize(s));

    std::vector<int> count_a(s_len, 0);

    //! First pass: Compute count_a, which stores the number
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

    //! Second pass: Compute minimum deletions on the fly
    int min_deletions {s_len};
    int b_count {};
    for (int idx = 0; idx < s_len; ++idx)
    {
        min_deletions = std::min(min_deletions, count_a[idx] + b_count);

        if (s[idx] == 'b')
        {
            ++b_count;
        }
    }

    return min_deletions;

} // static int minimumDeletionsDS2( ...

//! @brief Two-variable method discussion solution
//! @param[in] s std::string containing 'a' or 'b'
//! @return Min number of deletions to make s balanced
static int minimumDeletionsDS3(std::string s)
{
    //! @details https://leetcode.com/problems
    //!          /minimum-deletions-to-make-string-balanced
    //!
    //!          Time complexity O(N) where N = s.size(). The algorithm performs
    //!          two linear passes over the string.
    //!          Space complexity O(1). Use only constant space variables.

    const auto s_len = static_cast<int>(std::ssize(s));

    //! First pass: Count the number of 'a'
    auto a_count = static_cast<int>(std::count(s.begin(), s.end(), 'a'));

    //! Second pass: Iterate through string to compute min deletions
    int min_deletions {s_len};
    int b_count {};
    for (int idx = 0; idx < s_len; ++idx)
    {
        if (s[idx] == 'a')
        {
            --a_count;
        }

        min_deletions = std::min(min_deletions, a_count + b_count);

        if (s[idx] == 'b')
        {
            ++b_count;
        }
    }

    return min_deletions;

} // static int minimumDeletionsDS3( ...

//! @brief Stack (one pass) discussion solution
//! @param[in] s std::string containing 'a' or 'b'
//! @return Min number of deletions to make s balanced
static int minimumDeletionsDS4(std::string s)
{
    //! @details https://leetcode.com/problems
    //!          /minimum-deletions-to-make-string-balanced
    //!
    //!          Time complexity O(N) where N = s.size(). The algorithm performs
    //!          a single linear pass over the string with push and pop in O(1).
    //!          Space complexity O(N). The algorithm uses a stack that may grow
    //!          up to the size of the string in the worst case when no
    //!          deletions are needed.

    std::stack<char> char_stack {};
    int              delete_count {};

    //! Iterate through each character in the string
    for (const char curr_char : s)
    {
        //! If top of stack is 'b' and current char is 'a'
        if (!char_stack.empty() && char_stack.top() == 'b' && curr_char == 'a')
        {
            //! Remove 'b' from stack
            char_stack.pop();

            //! Increment deletion count
            ++delete_count;
        }
        else
        {
            //! Push current character onto stack
            char_stack.push(curr_char);
        }
    }

    return delete_count;

} // static int minimumDeletionsDS4( ...

//! @brief Dynamic programming (one pass) discussion solution
//! @param[in] s std::string containing 'a' or 'b'
//! @return Min number of deletions to make s balanced
static int minimumDeletionsDS5(std::string s)
{
    //! @details https://leetcode.com/problems
    //!          /minimum-deletions-to-make-string-balanced
    //!
    //!          Time complexity O(N) where N = s.size(). THe algorithm performs
    //!          a single linear pass over the string with updates to dp vector.
    //!          Space complexity O(N). The algorithm requires space for dp.

    const auto s_len = static_cast<int>(std::ssize(s));

    std::vector<int> dp(s_len + 1, 0);
    int              b_count {};

    //! Let dp[i] be min number of deletions to balance the substring s[0, i)
    //! While traversing string, update dp based on current char and prior state
    for (int idx = 0; idx < s_len; ++idx)
    {
        if (s[idx] == 'b')
        {
            dp[idx + 1] = dp[idx];
            ++b_count;
        }
        else
        {
            //! Two cases: Remove 'a' or keep 'a'
            //!   Remove 'a': dp[idx] + 1. If choose to delete current 'a', need
            //!               one more deletion than what was required for prior
            //!               substring dp[idx]. We are reducing num 'a's to
            //!               match num existing 'b's.
            //!   Keep 'a': Keep current 'a' and remove all 'b's that came prior
            //!             The number of 'b's seen so far is b_count, which is
            //!             the number of deletions needed to keep this 'a'. We
            //!             are ensuring all 'a's come before 'b's.
            dp[idx + 1] = std::min(dp[idx] + 1, b_count);
        }
    }

    return dp[s_len];

} // static int minimumDeletionsDS5( ...

TEST(MinimumDeletionsTest, SampleTest1)
{
    EXPECT_EQ(2, minimumDeletionsDS1("aababbab"));
    EXPECT_EQ(2, minimumDeletionsDS2("aababbab"));
    EXPECT_EQ(2, minimumDeletionsDS3("aababbab"));
    EXPECT_EQ(2, minimumDeletionsDS4("aababbab"));
    EXPECT_EQ(2, minimumDeletionsDS5("aababbab"));
}

TEST(MinimumDeletionsTest, SampleTest2)
{
    EXPECT_EQ(2, minimumDeletionsDS1("bbaaaaabbb"));
    EXPECT_EQ(2, minimumDeletionsDS2("bbaaaaabbb"));
    EXPECT_EQ(2, minimumDeletionsDS3("bbaaaaabbb"));
    EXPECT_EQ(2, minimumDeletionsDS4("bbaaaaabbb"));
    EXPECT_EQ(2, minimumDeletionsDS5("bbaaaaabbb"));
}
