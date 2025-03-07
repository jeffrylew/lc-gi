#include <gtest/gtest.h>

#include <algorithm>
#include <limits>
#include <vector>

//! @brief First attempt to get the sum of the subarray with the largest sum
//! @param[in] nums Reference to vector of integers
//! @return Sum of the subarray with the largest sum
static int maxSubArrayFA(const std::vector<int>& nums)
{
    //! @details https://leetcode.com/explore/interview/card/google/64
    //!          /dynamic-programming-4/3085/
    //!
    //!          Time complexity O(N), N = nums.size()
    //!          Space complexity O(1)
    //!
    //!          First attempt solution does not pass SampleTest4

    const auto nums_size = static_cast<int>(std::ssize(nums));
    if (nums_size == 1)
    {
        return nums[0];
    }

    int prev_sum {nums[0]};
    int min_sum {nums[0]};
    int max_sum {nums[0]};

    for (int prefix_sum_idx = 1; prefix_sum_idx < nums_size; ++prefix_sum_idx)
    {
        const int prefix_sum {prev_sum + nums[prefix_sum_idx]};

        if (prefix_sum < min_sum)
        {
            min_sum = prefix_sum;
        }
        else if (prefix_sum > max_sum)
        {
            max_sum = prefix_sum;
        }

        prev_sum = prefix_sum;
    }

    return min_sum < 0 ? max_sum - min_sum : max_sum;

} // static int maxSubArrayFA( ...

[[nodiscard]] static void get_max_subarr_sum_SA(
    int                            left_idx,
    int                            right_idx,
    std::vector<std::vector<int>>& subarr_sums)
{
    //! @note Decided to look at discussion solutions

} // [[nodiscard]] static int get_max_subarr_sum_SA( ...

//! @brief Second attempt to get the sum of the subarray with the largest sum
//! @param[in] nums Reference to vector of integers
//! @return Sum of the subarray with the largest sum
static int maxSubArraySA(const std::vector<int>& nums)
{
    //! @details https://leetcode.com/explore/interview/card/google/64
    //!          /dynamic-programming-4/3085/

    const auto nums_size = static_cast<int>(std::ssize(nums));
    if (nums_size == 1)
    {
        return nums[0];
    }

    std::vector<int> prefix_sum(nums.size());
    prefix_sum[0] = nums[0];
    for (int idx = 1; idx < nums_size; ++idx)
    {
        prefix_sum[idx] = prefix_sum[idx - 1] + nums[idx];
    }

    std::vector<std::vector<int>> subarr_sums(
        nums.size(), std::vector<int>(nums.size(), -1E5));

    get_max_subarr_sum_SA(0, nums_size - 1, subarr_sums);

    return subarr_sums[0][nums_size - 1];

} // static int maxSubArraySA( ...

//! @brief Optimized brute force discussion solution
//! @param[in] nums Reference to vector of integers
//! @return Sum of the subarray with the largest sum
static int maxSubArrayDS1(const std::vector<int>& nums)
{
    //! @details https://leetcode.com/problems/maximum-subarray/editorial/
    //!
    //!          Time complexity O(N ^ 2) where N = nums.size(). Two nested for
    //!          loops iterate through nums.
    //!          Space complexity O(1) for integer variables.

    const auto nums_size = static_cast<int>(std::ssize(nums));

    int max_subarray_sum {std::numeric_limits<int>::min()};

    for (int start_idx = 0; start_idx < nums_size; ++start_idx)
    {
        int curr_subarray_sum {};

        for (int subarr_idx = start_idx; subarr_idx < nums_size; ++subarr_idx)
        {
            curr_subarray_sum += nums[subarr_idx];
            max_subarray_sum = std::max(max_subarray_sum, curr_subarray_sum);
        }
    }

    return max_subarray_sum;

} // static int maxSubArrayDS1( ...

//! @brief Dynamic programming (Kadane's Algorithm) discussion solution
//! @param[in] nums Reference to vector of integers
//! @return Sum of the subarray with the largest sum
static int maxSubArrayDS2(const std::vector<int>& nums)
{
    //! @details https://leetcode.com/problems/maximum-subarray/editorial/
    //!
    //!          Time complexity O(N) where N = nums.size(). We iterate through
    //!          each element of nums once.
    //!          Space complexity O(1) for integer variables

    //! Initialize sums with the first element
    int curr_subarray_sum {nums[0]};
    int max_subarray_sum {nums[0]};

    //! Any subarray whose sum is positive is worth keeping. Whenever the sum is
    //! negative, the subarray is not worth keeping.
    for (int idx = 1; idx < std::ssize(nums); ++idx)
    {
        //! If curr_subarray_sum is negative, reset it to nums[idx].
        //! Using max() leads to nums[idx] > curr_subarray_sum + nums[idx].
        //! Otherwise, keep adding to it.
        curr_subarray_sum = std::max(nums[idx], curr_subarray_sum + nums[idx]);
        max_subarray_sum  = std::max(max_subarray_sum, curr_subarray_sum);
    }

    return max_subarray_sum;

} // static int maxSubArrayDS2( ...

//! @brief Divide and conquer discussion solution
//! @param[in] nums Reference to vector of integers
//! @return Sum of the subarray with the largest sum
static int maxSubArrayDS3(const std::vector<int>& nums)
{
    //! @details https://leetcode.com/problems/maximum-subarray/editorial/

    //! @todo

} // static int maxSubArrayDS3( ...

TEST(MaxSubArrayTest, SampleTest1)
{
    //! nums:       -1  1 -3 4 -1 2 1 -5 4
    //! Prefix sum: -2 -1 -4 0 -1 1 2 -3 1
    constexpr std::vector<int> nums {-2, 1, -3, 4, -1, 2, 1, -5, 4};

    EXPECT_EQ(6, maxSubArrayFA(nums));
    // EXPECT_EQ(6, maxSubArraySA(nums));
    EXPECT_EQ(6, maxSubArrayDS1(nums));
    EXPECT_EQ(6, maxSubArrayDS2(nums));
    EXPECT_EQ(6, maxSubArrayDS3(nums));
}

TEST(MaxSubArrayTest, SampleTest2)
{
    constexpr std::vector<int> nums {1};

    EXPECT_EQ(1, maxSubArrayFA(nums));
    // EXPECT_EQ(1, maxSubArraySA(nums));
    EXPECT_EQ(1, maxSubArrayDS1(nums));
    EXPECT_EQ(1, maxSubArrayDS2(nums));
    EXPECT_EQ(1, maxSubArrayDS3(nums));
}

TEST(MaxSubArrayTest, SampleTest3)
{
    //! nums:       5 4 -1  7  8
    //! Prefix sum: 5 9  8 15 23
    constexpr std::vector<int> nums {5, 4, -1, 7, 8};

    EXPECT_EQ(23, maxSubArrayFA(nums));
    // EXPECT_EQ(23, maxSubArraySA(nums));
    EXPECT_EQ(23, maxSubArrayDS1(nums));
    EXPECT_EQ(23, maxSubArrayDS2(nums));
    EXPECT_EQ(23, maxSubArrayDS3(nums));
}

TEST(MaxSubArrayTest, SampleTest4)
{
    //! nums:       -2 -1
    //! Prefix sum: -2 -3
    constexpr std::vector<int> nums {-2, -1};

    EXPECT_NE(-1, maxSubArrayFA(nums));
    EXPECT_EQ(1, maxSubArrayFA(nums));
    // EXPECT_EQ(-1, maxSubArraySA(nums));
    EXPECT_EQ(-1, maxSubArrayDS1(nums));
    EXPECT_EQ(-1, maxSubArrayDS2(nums));
    EXPECT_EQ(-1, maxSubArrayDS3(nums));
}
