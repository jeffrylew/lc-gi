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

//! @brief Simulation discussion solution
//! @param[in] n    Number of people in the line
//! @param[in] time Number of seconds elapsed, every second the pillow is passed
//! @return Index of the person holding the pillow after time seconds
static int passThePillowDS1(int n, int time)
{
    //! @details https://leetcode.com/problems/pass-the-pillow/
    //!
    //!          Time complexity O(T) where T = amount of time given. Algorithm
    //!          runs a loop T times with each iteration representing a second.
    //!          Space complexity O(1), algorithm only uses a fixed number of
    //!          variables which take up a constant amount of space.

    int current_pillow_position {1};
    int current_time {};

    //! direction = 1 indicates movement towards the right
    int direction {1};

    while (current_time < time)
    {
        if (0 < current_pillow_position + direction
            && current_pillow_position + direction <= n)
        {
            current_pillow_position += direction;
            ++current_time;
        }
        else
        {
            //! Reverse the direction if the next position is out of bounds
            direction *= -1;
        }
    }

    return current_pillow_position;
}

TEST(PassThePillowTest, SampleTest1)
{
    EXPECT_EQ(2, passThePillowFA(4, 5));
    EXPECT_EQ(2, passThePillowDS1(4, 5));
}

TEST(PassThePillowTest, SampleTest2)
{
    EXPECT_EQ(3, passThePillowFA(3, 2));
    EXPECT_EQ(3, passThePillowDS1(3, 2));
}
