#include <gtest/gtest.h>

#include <string>

//! @brief First attempt to get min length string that will unlock the safe
//! @param[in] n Number of digits in password, 1 <= n <= 4
//! @param[in] k Range of each digit in password, 1 <= k <= 10
//! @return Minimum length string that will unlock the safe at some point
static std::string crackSafeFA(int n, int k)
{
    //! @details https://leetcode.com/problems/cracking-the-safe/description/

} // static std::string crackSafeFA( ...

TEST(CrackSafeTest, SampleTest1)
{
    EXPECT_EQ("10", crackSafeFA(1, 2));
}

TEST(CrackSafeTest, SampleTest2)
{
    EXPECT_EQ("01100", crackSafeFA(2, 2));
}
