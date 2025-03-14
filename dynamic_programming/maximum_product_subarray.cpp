#include <gtest/gtest.h>

#include <algorithm>
#include <limits>
#include <vector>

/*
[[nodiscard]] static int get_max_product_FA(
    const std::vector<int>&        nums,
    std::vector<std::vector<int>>& product_state,
    int                            left_idx,
    int                            right_idx)
{
    const auto nums_size = static_cast<int>(std::ssize(nums));

    if (right_idx >= nums_size)
    {
        return ;
    }

    if (product_state[left_idx][right_idx] != std::numeric_limits<int>::min())
    {
        return product_state[left_idx][right_idx];
    }

    int curr_product {1};
    int max_product {std::numeric_limits<int>::min()};

    for (int idx = curr_idx; idx < nums_size; ++idx)
    {

    }

} // static void get_max_product_FA( ...
*/

//! @brief First attempt to get product of subarray with largest product
//! @param[in] nums Vector of integers
//! @return Product of subarray with the largest product
static int maxProductFA(std::vector<int> nums)
{
    //! @details https://leetcode.com/explore/interview/card/google/64
    //!          /dynamic-programming-4/3087/

    /*
     * Brute force approach
     *
    const auto nums_size = static_cast<int>(std::ssize(nums));
    int        max_product {std::numeric_limits<int>::min()};

    for (int left_idx = 0; left_idx < nums_size; ++left_idx)
    {
        int curr_product {nums[left_idx]};

        if (curr_product > max_product)
        {
            max_product = curr_product;
        }

        for (int right_idx = left_idx + 1; right_idx < nums_size; ++right_idx)
        {
            curr_product *= nums[right_idx];
            if (curr_product > max_product)
            {
                max_product = curr_product;
            }
        }
    }

    return max_product;
    */

    /*
    std::vector<std::vector<int>> product_state(
        nums.size(),
        std::vector<int>(nums.size(), std::numeric_limits<int>::min()));

    get_max_product_FA(nums, product_state, 0, std::ssize(nums));

    return product_state.back();
    */

    /*
    int left_idx {};
    int curr_product {nums[left_idx]};
    int max_product {std::numeric_limits<int>::min()};

    for (int right_idx = 0; right_idx < std::ssize(nums); ++right_idx)
    {
        if (left_idx >= right_idx)
        {
            continue;
        }
        max_product = std::max(max_product, curr_product);

        while (curr_product < 0)
        {
            ++left_idx;
            curr_product *= nums[left_idx];
        }

        max_product = std::max(max_product, curr_product);
    }

    return max_product;
    */

} // static int maxProductFA( ...

TEST(MaxProductTest, SampleTest1)
{
    // EXPECT_EQ(6, maxProductFA({2, 3, -2, 4}));
}

TEST(MaxProductTest, SampleTest2)
{
    // EXPECT_EQ(0, maxProductFA({-2, 0, -1}));
}
