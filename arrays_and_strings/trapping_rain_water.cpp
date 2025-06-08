#include <gtest/gtest.h>

#include <algorithm>
#include <queue>
#include <vector>

//! @brief First attempt to compute how much water can be trapped after rain
//! @param[in] Vector of N non-negative integers representing an elevation map
//! @return Units of rain water that can be trapped
static int trapFA(const std::vector<int>& height)
{
    int water_units {};

    //! Monotonically decreasing queue of heights based on their indices
    std::queue<int> height_indices;
    const auto      num_heights = static_cast<int>(std::ssize(height));

    for (int height_idx = 0; height_idx < num_heights; ++height_idx)
    {
        while (!height_indices.empty()
               && height[height_idx] > height[height_idx_stack.top()])
        {
            if (height[height_idx_stack.top()] > 0)
            {
                left_idx = std::min(left_idx, height_idx_stack.top());
            }

            height_idx_stack.pop();
        }

        if (left_idx != num_heights)
        {
            const int 
            water_units += (height_idx - )
        }

        height_idx_stack.push(height_idx);
    }

    return water_units;
}

TEST(TrapTest, SampleTest1)
{
    const std::vector<int> height {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};

    EXPECT_EQ(6, trapFA(height));
}

TEST(TrapTest, SampleTest2)
{
    const std::vector<int> height {4, 2, 0, 3, 2, 5};

    EXPECT_EQ(9, trapFA(height));
}
