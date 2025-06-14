#include <gtest/gtest.h>

#include <algorithm>
#include <deque>
#include <iterator>
#include <stack>
#include <vector>

//! @brief First attempt to compute how much water can be trapped after rain
//! @param[in] height Vector of N non-negative ints representing elevation map
//! @return Units of rain water that can be trapped
static int trapFA(const std::vector<int>& height)
{
    //! @details https://leetcode.com/explore/interview/card/google/59
    //!          /array-and-strings/341/
    //!
    //!          Passes 222/324 test cases. Does not pass SampleTest3.
    //!          Time complexity O(N), where N = Number of heights. We loop
    //!          through the vector at most twice.
    //!          Space complexity O(N) for height_deque and remaining_heights.

    int water_units {};

    //! Deque of heights
    std::deque<int> height_deque;

    for (const int curr_height : height)
    {
        while (!height_deque.empty()
               && curr_height > height_deque.back()
               && curr_height >= height_deque.front())
        {
            const int water_level {std::min(height_deque.front(), curr_height)};

            if (water_level > 0 && water_level != height_deque.back())
            {
                water_units += water_level - height_deque.back();
            }

            height_deque.pop_back();
        }

        height_deque.push_back(curr_height);
    }

    std::vector<int> remaining_heights(std::move_iterator(height_deque.begin()),
                                       std::move_iterator(height_deque.end()));
    height_deque.clear();

    for (const int curr_height : remaining_heights)
    {
        while (!height_deque.empty() && curr_height > height_deque.back())
        {
            water_units += curr_height - height_deque.back();
            height_deque.pop_back();
        }

        height_deque.push_back(curr_height);
    }

    return water_units;

} // static int trapFA( ...

//! @brief Brute force discussion solution
//! @param[in] height Vector of N non-negative ints representing elevation map
//! @return Units of rain water that can be trapped
static int trapDS1(const std::vector<int>& height)
{
    //! @details https://leetcode.com/problems/trapping-rain-water/editorial/
    //!
    //!          Time complexity O(N ^ 2). For each element of height vector, we
    //!          iterate the left and right hand sides.
    //!          Space complexity O(1).

    int water_units {};

    const auto num_heights = static_cast<int>(std::ssize(height));

    for (int curr_idx = 1; curr_idx < num_heights - 1; ++curr_idx)
    {
        int left_max {};
        int right_max {};

        //! Find the max height on the left side
        for (int lhs_idx = curr_idx; lhs_idx >= 0; --lhs_idx)
        {
            left_max = std::max(left_max, height[lhs_idx]);
        }

        //! Find the max height on the right side
        for (int rhs_idx = curr_idx; rhs_idx < num_heights; ++rhs_idx)
        {
            right_max = std::max(right_max, height[rhs_idx]);
        }

        num_heights += std::min(left_max, right_max) - height[curr_idx];
    }

    return water_units;
}

//! @brief Dynamic programming discussion solution
//! @param[in] height Vector of N non-negative ints representing elevation map
//! @return Units of rain water that can be trapped
static int trapDS2(const std::vector<int>& height)
{
    //! @details https://leetcode.com/problems/trapping-rain-water/editorial/
    //!
    //!          Time complexity O(N) where N = number of heights. We store the
    //!          max heights using two iterations of O(N). Then we update the
    //!          final water_units using the stored values in O(N).
    //!          Space complexity O(N) for left_max and right_max vectors.

    if (height.empty())
    {
        return 0;
    }

    int water_units {};

    const auto num_heights = static_cast<int>(std::ssize(height));

    std::vector<int> left_max(height.size());
    std::vector<int> right_max(height.size());

    left_max[0] = height[0];
    for (int lhs_idx = 1; lhs_idx < num_heights; ++lhs_idx)
    {
        left_max[lhs_idx] = std::max(height[lhs_idx], left_max[lhs_idx - 1]);
    }

    right_max[num_heights - 1] = height[num_heights - 1];
    for (int rhs_idx = num_heights - 2; rhs_idx >= 0; --rhs_idx)
    {
        right_max[rhs_idx] = std::max(height[rhs_idx], right_max[rhs_idx + 1]);
    }

    for (int idx = 1; idx < num_heights - 1; ++idx)
    {
        water_units += std::min(left_max[idx], right_max[idx]) - height[idx];
    }

    return water_units;
}

//! @brief Using stacks discussion solution
//! @param[in] height Vector of N non-negative ints representing elevation map
//! @return Units of rain water that can be trapped
static int trapDS3(const std::vector<int>& height)
{
    //! @details https://leetcode.com/problems/trapping-rain-water/editorial/
    //!
    //!          Time complexity O(N) where N = height.size(). Each bar can be
    //!          touched at most twice (due to insertion/deletion from stack)
    //!          per iteration. Insertion and deletion takes O(1) time.
    //!          Space complexity O(N). Stack can take up to O(N) space in the
    //!          case of a stair-like or flat structure.

    int water_units {};
    int curr_idx {};

    std::stack<int> idx_stack {};

    while (curr_idx < std::ssize(height))
    {
        while (!idx_stack.empty() && height[curr_idx] > height[idx_stack.top()])
        {
            const int top_idx {idx_stack.top()};
            idx_stack.pop();

            if (idx_stack.empty())
            {
                break;
            }

            const int distance {curr_idx - idx_stack.top() - 1};
            const int bounded_height {
                std::min(height[curr_idx], height[idx_stack.top()])
                - height[top_idx]};
            water_units += distance * bounded_height;
        }

        idx_stack.push(curr_idx++);
    }

    return water_units;
}

TEST(TrapTest, SampleTest1)
{
    const std::vector<int> height {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};

    EXPECT_EQ(6, trapFA(height));
    EXPECT_EQ(6, trapDS1(height));
    EXPECT_EQ(6, trapDS2(height));
    EXPECT_EQ(6, trapDS3(height));
}

TEST(TrapTest, SampleTest2)
{
    //           -
    // - 1 2 3 4 -
    // - 5 6 - 7 -
    // - - 8 - - -
    // - - 9 - - -
    const std::vector<int> height {4, 2, 0, 3, 2, 5};

    EXPECT_EQ(9, trapFA(height));
    EXPECT_EQ(9, trapDS1(height));
    EXPECT_EQ(9, trapDS2(height));
    EXPECT_EQ(9, trapDS3(height));
}

TEST(TrapTest, SampleTest3)
{
    //   -
    //   - 1 2 -
    //   - 3 4 -
    //   - 5 - -
    //   - 6 - -
    //   - 7 - -
    //   - - - -
    const std::vector<int> height {0, 7, 1, 4, 6};

    EXPECT_NE(7, trapFA(height));
    EXPECT_EQ(5, trapFA(height));
    EXPECT_EQ(7, trapDS1(height));
    EXPECT_EQ(7, trapDS2(height));
    EXPECT_EQ(7, trapDS3(height));
}
