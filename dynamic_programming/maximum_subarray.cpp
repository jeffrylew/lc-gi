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

//! @brief Recursive helper to get max sum for vector between left_idx/right_idx
//! @param[in] nums      Reference to vector of ints to check subarray sums from
//! @param[in] left_idx  Left index of subarray
//! @param[in] right_idx Right index of subarray
//! @return Max sum for subarray between left_idx and right_idx in nums
[[nodiscard]] static int get_max_subarr_sum(const std::vector<int>& nums,
                                            int                     left_idx,
                                            int                     right_idx)
{
    //! @details https://leetcode.com/problems/maximum-subarray/editorial/
    //!
    //!          Time complexity O(N * log N) where N = nums.size(). The first
    //!          call visits every element of nums. Then we split the vector in
    //!          half and call get_max_subarr_sum on each half. Both of these
    //!          calls iterate through every element in their half, which is
    //!          every element in nums when combined. Thus, we iterate over all
    //!          elements in nums log N times since that is how many times the
    //!          vector can be split in half.
    //!          Space complexity O(log N) for the recursion stack. Every time
    //!          the vector is split in half, another call of get_max_subarr_sum
    //!          is added to the stack until calls are resolved by the base case
    //!          which is an empty vector that occurs after log N calls.

    //! Base case: empty vector
    if (left_idx > right_idx)
    {
        return std::numeric_limits<int>::min();
    }

    const int mid_idx {left_idx + (right_idx - left_idx) / 2};

    int curr_sum {};
    int max_left_sum {};
    int max_right_sum {};

    //! Iterate from the middle to the beginning
    for (int idx = mid_idx - 1; idx >= left_idx; --idx)
    {
        curr_sum += nums[idx];
        max_left_sum = std::max(max_left_sum, curr_sum);
    }

    //! Reset curr_sum and iterate from the middle to the end
    curr_sum = 0;
    for (int idx = mid_idx + 1; idx <= right_idx; ++idx)
    {
        curr_sum += nums[idx];
        max_right_sum = std::max(max_right_sum, curr_sum);
    }

    //! max_combined_sum uses the middle element
    //! and the best possible sum from each half
    const int max_combined_sum {max_left_sum + nums[mid_idx] + max_right_sum};

    //! Find the best possible subarray from both halves
    const int left_half_sum {get_max_subarr_sum(nums, left_idx, mid_idx - 1)};
    const int right_half_sum {get_max_subarr_sum(nums, mid_idx + 1, right_idx)};

    //! The largest sum of the three is the answer
    return std::max({left_half_sum, max_combined_sum, right_half_sum});

} // [[nodiscard]] static int get_max_subarr_sum( ...

//! @brief Divide and conquer discussion solution
//! @param[in] nums Reference to vector of integers
//! @return Sum of the subarray with the largest sum
static int maxSubArrayDS3(const std::vector<int>& nums)
{
    //! @details https://leetcode.com/problems/maximum-subarray/editorial/

    const auto nums_size = static_cast<int>(std::ssize(nums));

    return get_max_subarr_sum(nums, 0, nums_size - 1);

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
