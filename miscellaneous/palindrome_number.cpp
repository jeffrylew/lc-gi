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

//! @brief Discussion solution to check if int x is a palindrome
//! @param[in] x Integer to check
//! @return True if x is a palindrome, else false
static bool isPalindromeDS(int x)
{
    //! @details https://leetcode.com/problems/palindrome-number/
    //!
    //!          Time complexity O(log n). Divide input by 10 every iteration.
    //!          Space complexity O(1)

    //! Special cases: When x < 0, x is not a palindrome
    //! If the last digit is 0, the first digit of the number needs to be 0.
    //! Only 0 satisfies this property
    if (x < 0 || (x % 10 == 0 && x != 0))
    {
        return false;
    }

    int reverted_number {};
    while (x > reverted_number)
    {
        reverted_number = reverted_number * 10 + x % 10;
        x /= 10;
    }

    //! When the length is an odd number, can get rid of the middle digit by
    //! reverted_number / 10. For example, when input is 12321 we have x = 12
    //! and reverted_number = 123 at the end of the while loop. Can get rid of
    //! middle digit since it doesn't matter (always equals itself).
    return x == reverted_number || x == reverted_number / 10;

} // static bool isPalindromeDS( ...

TEST(IsPalindromeTest, SampleTest1)
{
    EXPECT_TRUE(isPalindromeFA(121));
    EXPECT_TRUE(isPalindromeDS(121));
}

TEST(IsPalindromeTest, SampleTest2)
{
    EXPECT_FALSE(isPalindromeFA(-121));
    EXPECT_FALSE(isPalindromeDS(-121));
}

TEST(IsPalindromeTest, SampleTest3)
{
    EXPECT_FALSE(isPalindromeFA(10));
    EXPECT_FALSE(isPalindromeDS(10));
}
