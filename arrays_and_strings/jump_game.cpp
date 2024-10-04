#include <gtest/gtest.h>

#include <vector>

//! @brief First attempt to see if can reach last index
//! @param[in] nums Vector of max jump lengths
//! @return True if can reach the last index, else false
static bool canJumpFA(std::vector<int> nums)
{
    //! @todo Idea is track max index reachable at each index
}

TEST(CanJumpTest, SampleTest1)
{
    EXPECT_TRUE(canJumpFA({2, 3, 1, 1, 4}));
}

TEST(CanJumpTest, SampleTest2)
{
    EXPECT_FALSE(canJumpFA({3, 2, 1, 0, 4}));
}
