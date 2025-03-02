#include <gtest/gtest.h>

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

TEST(MaxSubArrayTest, SampleTest1)
{
    //! nums:       -1  1 -3 4 -1 2 1 -5 4
    //! Prefix sum: -2 -1 -4 0 -1 1 2 -3 1
    constexpr std::vector<int> nums {-2, 1, -3, 4, -1, 2, 1, -5, 4};

    EXPECT_EQ(6, maxSubArrayFA(nums));
}

TEST(MaxSubArrayTest, SampleTest2)
{
    constexpr std::vector<int> nums {1};

    EXPECT_EQ(1, maxSubArrayFA(nums));
}

TEST(MaxSubArrayTest, SampleTest3)
{
    //! nums:       5 4 -1  7  8
    //! Prefix sum: 5 9  8 15 23
    constexpr std::vector<int> nums {5, 4, -1, 7, 8};

    EXPECT_EQ(23, maxSubArrayFA(nums));
}

TEST(MaxSubArrayTest, SampleTest4)
{
    //! nums:       -2 -1
    //! Prefix sum: -2 -3
    constexpr std::vector<int> nums {-2, -1};

    EXPECT_NE(-1, maxSubArrayFA(nums));
    EXPECT_EQ(1, maxSubArrayFA(nums));
}
