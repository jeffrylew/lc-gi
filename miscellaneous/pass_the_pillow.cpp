#include <gtest/gtest.h>

//! @brief First attempt to get index of person holding pillow after time secs
//! @param[in] n    Number of people in the line
//! @param[in] time Number of seconds elapsed, every second the pillow is passed
//! @return Index of the person holding the pillow after time seconds
static int passThePillowFA(int n, int time)
{
    //! @details https://leetcode.com/problems/pass-the-pillow/
    //!
    //!          Time complexity O(1)
    //!          Space complexity O(1)

    int idx_in_a_round {time % (2 * n - 2)};

    if (idx_in_a_round > n - 1)
    {
        idx_in_a_round = (2 * n - 2) - idx_in_a_round;
    }

    return 1 + idx_in_a_round;
}

TEST(PassThePillowTest, SampleTest1)
{
    EXPECT_EQ(2, passThePillowFA(4, 5));
}

TEST(PassThePillowTest, SampleTest2)
{
    EXPECT_EQ(3, passThePillowFA(3, 2));
}
