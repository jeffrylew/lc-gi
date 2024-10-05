#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

//! @brief First attempt to see if can reach last index
//! @param[in] nums Vector of max jump lengths
//! @return True if can reach the last index, else false
static bool canJumpFA(std::vector<int> nums)
{
    //! @details https://leetcode.com/problems/jump-game/description/
    //!
    //!          Time complexity O(N) where N = nums.size()
    //!          Space complexity O(1)

    const auto nums_size = static_cast<int>(std::ssize(nums));
    if (nums_size == 1)
    {
        return true;
    }

    if (nums[0] == 0)
    {
        return false;
    }

    for (int idx = 1; idx < nums_size - 1; ++idx)
    {
        nums[idx] = std::max(idx + nums[idx], nums[idx - 1]);

        if (nums[idx] < idx + 1)
        {
            return false;
        }
    }

    return nums[nums_size - 2] >= (nums_size - 1);
}

TEST(CanJumpTest, SampleTest1)
{
    EXPECT_TRUE(canJumpFA({2, 3, 1, 1, 4}));
}

TEST(CanJumpTest, SampleTest2)
{
    EXPECT_FALSE(canJumpFA({3, 2, 1, 0, 4}));
}
