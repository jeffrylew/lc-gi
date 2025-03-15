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

//! @brief Brute force discussion solution
//! @param[in] nums Vector of integers
//! @return Product of subarray with the largest product
static int maxProductDS1(std::vector<int> nums)
{
    //! @details leetcode.com/problems/maximum-product-subarray/editorial
    //!
    //!          Time complexity O(N ^ 2) where N = nums.size(). Quadratic
    //!          runtime is needed to check every possible subarray following
    //!          every element in nums.
    //!          Space complexity O(1) for integer variables

    if (nums.empty())
    {
        return 0;
    }

    const auto nums_size = static_cast<int>(std::ssize(nums));
    int        max_product {nums[0]};

    for (int start_idx = 0; start_idx < nums_size; ++start_idx)
    {
        int curr_product {1};

        for (int prod_idx = start_idx; prod_idx < nums_size; ++prod_idx)
        {
            curr_product *= nums[prod_idx];
            max_product = std::max(max_product, curr_product);
        }
    }

    return max_product;

} // static int maxProductDS1( ...

//! @brief Dynamic programming discussion solution
//! @param[in] nums Vector of integers
//! @return Product of subarray with the largest product
static int maxProductDS2(std::vector<int> nums)
{
    //! @details leetcode.com/problems/maximum-product-subarray/editorial
    //!
    //!          Time complexity O(N) where N = nums.size() to iterate through
    //!          nums once.
    //!          Space complexity O(1) for integer variables.

    if (nums.empty())
    {
        return 0;
    }

    //! Maximum product up to current number. Tracks product of positive numbers
    int curr_max_product {nums[0]};

    //! Minimum product up to current number. Handles negative numbers
    int curr_min_product {nums[0]};
    int max_product {curr_max_product};

    for (int idx = 1; idx < std::ssize(nums); ++idx)
    {
        const int curr_num {nums[idx]};

        //! Take max value among
        //! - curr_num: If accumulated product is worse than current number e.g.
        //!             when preceded by zero [0, 4] or negative number [-3, 4]
        //! - curr_max_product * curr_num: Picked when accumulated product has
        //!                                been steadily increasing (positive)
        //! - curr_min_product * curr_num: Picked if current number is negative
        //!                                and accumulated product was negative
        const int max_so_far {std::max({curr_num,
                                        curr_max_product * curr_num,
                                        curr_min_product * curr_num})};

        curr_min_product = std::min({curr_num,
                                     curr_max_product * curr_num,
                                     curr_min_product * curr_num});

        //! Update curr_max_product after curr_min_product to avoid overwriting
        curr_max_product = max_so_far;

        //! Update max_product with the maximum product found so far
        max_product = std::max(max_product, curr_max_product);
    }

    return max_product;

} // static int maxProductDS2( ...

TEST(MaxProductTest, SampleTest1)
{
    // EXPECT_EQ(6, maxProductFA({2, 3, -2, 4}));
    EXPECT_EQ(6, maxProductDS1({2, 3, -2, 4}));
    EXPECT_EQ(6, maxProductDS2({2, 3, -2, 4}));
}

TEST(MaxProductTest, SampleTest2)
{
    // EXPECT_EQ(0, maxProductFA({-2, 0, -1}));
    EXPECT_EQ(0, maxProductDS1({-2, 0, -1}));
    EXPECT_EQ(0, maxProductDS2({-2, 0, -1}));
}
