#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

//! @brief First attempt to get min diff after performing at most three moves
//! @param[in] nums Vector of integers between -10^9 and 10^9
//! @return Minimum difference between largest and smallest values of nums
static int minDifferenceFA(std::vector<int> nums)
{
    //! @details https://leetcode.com/problems
    //!    /minimum-difference-between-largest-and-smallest-value-in-three-moves
    //!
    //!          Solution does not pass SampleTest4

    const auto nums_size = static_cast<int>(nums.size());
    if (nums_size < 4)
    {
        return 0;
    }

    std::sort(nums.begin(), nums.end());

    int left {};
    int right {nums_size - 1};
    int nmoves {3};

    while (nmoves > 0)
    {
        const int left_diff {nums[left + 1] - nums[left]};
        const int right_diff {nums[right] - nums[right - 1]};

        if (left_diff < right_diff)
        {
            --right;
        }
        else
        {
            ++left;
        }

        --nmoves;
    }

    return nums[right] - nums[left];

} // static int minDifferenceFA( ...

TEST(MinDifferenceTest, SampleTest1)
{
    EXPECT_EQ(0, minDifferenceFA({5, 3, 2, 4}));
}

TEST(MinDifferenceTest, SampleTest2)
{
    EXPECT_EQ(1, minDifferenceFA({1, 5, 0, 10, 14}));
}

TEST(MinDifferenceTest, SampleTest3)
{
    EXPECT_EQ(0, minDifferenceFA({3, 100, 20}));
}

TEST(MinDifferenceTest, SampleTest4)
{
    const std::vector<int> nums {9, 48, 92, 48, 81, 31};

    EXPECT_EQ(33, minDifferenceFA(nums));
    EXPECT_NE(17, minDifferenceFA(nums));
}
