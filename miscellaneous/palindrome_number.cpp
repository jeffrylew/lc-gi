#include <gtest/gtest.h>

#include <vector>

//! @brief First attempt to check if int x is a palindrome
//! @param[in] x Integer to check
//! @return True if x is a palindrome, else false
static bool isPalindromeFA(int x)
{
    //! @details https://leetcode.com/problems/palindrome-number/
    //!
    //!          Time complexity O(N) where N is number of digits.
    //!          Space complexity O(N) for digits vector.

    if (x < 0)
    {
        return false;
    }

    std::vector<int> digits {};
    while (x > 0)
    {
        digits.push_back(x % 10);
        x /= 10;
    }

    int left {};
    int right {static_cast<int>(std::ssize(digits) - 1L)};
    while (left <= right)
    {
        if (digits[left] != digits[right])
        {
            return false;
        }

        ++left;
        --right;
    }

    return true;

} // static bool isPalindromeFA( ...

TEST(IsPalindromeTest, SampleTest1)
{
    EXPECT_TRUE(isPalindromeFA(121));
}

TEST(IsPalindromeTest, SampleTest2)
{
    EXPECT_FALSE(isPalindromeFA(-121));
}

TEST(IsPalindromeTest, SampleTest3)
{
    EXPECT_FALSE(isPalindromeFA(10));
}
