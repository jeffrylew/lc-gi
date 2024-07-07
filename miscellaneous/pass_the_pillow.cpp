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

//! @brief Math discussion solution
//! @param[in] n    Number of people in the line
//! @param[in] time Number of seconds elapsed, every second the pillow is passed
//! @return Index of the person holding the pillow after time seconds
static int passThePillowDS2(int n, int time)
{
    //! @details https://leetcode.com/problems/pass-the-pillow/
    //!
    //!          Time complexity O(1), always perform a fixed number of
    //!          operations regardless of the size of the input.
    //!          Space complexity O(1), use a fixed number of variables.

    //! Calculate the number of complete rounds of pillow passing
    const int full_rounds {time / (n - 1)};

    //! Calculate the remaining time after complete rounds
    const int extra_time {time % (n - 1)};

    //! Determine the position of the person holding the pillow
    //! If full_rounds is even, the pillow is moving forwards
    //! If full_rounds is odd, the pillow is moving backwards
    if (full_rounds % 2 == 0)
    {
        //! Position when moving forwards, add 1 since count positions from 1
        return extra_time + 1;
    }

    //! Position when moving backwards
    return n - extra_time;
}

TEST(PassThePillowTest, SampleTest1)
{
    EXPECT_EQ(2, passThePillowFA(4, 5));
    EXPECT_EQ(2, passThePillowDS1(4, 5));
    EXPECT_EQ(2, passThePillowDS2(4, 5));
}

TEST(PassThePillowTest, SampleTest2)
{
    EXPECT_EQ(3, passThePillowFA(3, 2));
    EXPECT_EQ(3, passThePillowDS1(3, 2));
    EXPECT_EQ(3, passThePillowDS2(3, 2));
}
