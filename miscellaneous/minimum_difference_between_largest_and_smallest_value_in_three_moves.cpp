#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <limits>
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

//! @brief Sort + Greedy deletion discussion solution
//! @param[in] nums Vector of integers between -10^9 and 10^9
//! @return Minimum difference between largest and smallest values of nums
static int minDifferenceDS1(std::vector<int> nums)
{
    //! @details https://leetcode.com/problems
    //!    /minimum-difference-between-largest-and-smallest-value-in-three-moves
    //!
    //!          Time complexity O(N * log N) where N = nums.size(). Sorting
    //!          takes O(N * log N). The loop runs for 4 iterations, taking O(1)
    //!          Space complexity O(N) or O(log N). When sorting nums, extra
    //!          space is used and depends on the programming language. Python
    //!          uses Timsort which combines merge sort and insertion sort and
    //!          uses O(N) additional space. In C++, it is a hybrid of Quick
    //!          Sort, Heap Sort, and Insertion Sort with a worst-case O(log N).

    const auto nums_size = static_cast<int>(nums.size());

    //! If vector has four or fewer elements, return 0
    if (nums_size <= 4)
    {
        return 0;
    }

    std::sort(nums.begin(), nums.end());

    int min_diff {std::numeric_limits<int>::max()};

    //! Four scenarios to compute the minimum difference
    //! - Remove three largest elements
    //! - Remove two largest and smallest elements
    //! - Remove largest and two smallest elements
    //! - Remove three smallest elements
    for (int left = 0; left < 4; ++left)
    {
        min_diff = std::min(nums[nums_size - 4 + left] - nums[left], min_diff);
    }

    return min_diff;

} // static int minDifferenceDS1( ...

//! @brief Partial sort + greedy deletion discussion solution
//! @param[in] nums Vector of integers between -10^9 and 10^9
//! @return Minimum difference between largest and smallest values of nums
static int minDifferenceDS2(std::vector<int> nums)
{
    //! @details https://leetcode.com/problems
    //!    /minimum-difference-between-largest-and-smallest-value-in-three-moves
    //!
    //!          Time complexity O(N) where N = nums.size(). std::partial_sort
    //!          takes O(N * log 4) = O(N) to sort the first and last four
    //!          elements. The for loop that runs for four iterations takes O(1)
    //!          Space complexity O(1). The algorithm uses constant space.

    const auto nums_size = static_cast<int>(nums.size());

    if (nums_size <= 4)
    {
        return 0;
    }

    //! Sort the first four elements
    std::partial_sort(nums.begin(), nums.begin() + 4, nums.end());

    //! Sort the last four elements
    std::partial_sort(nums.rbegin(),
                      nums.rbegin() + 4,
                      nums.rend(),
                      std::greater {});

    int min_diff {std::numeric_limits<int>::max()};

    //! Four scenarios to compute the minimum difference
    //! - Remove three largest elements
    //! - Remove two largest and smallest elements
    //! - Remove largest and two smallest elements
    //! - Remove three smallest elements
    for (int left = 0; left < 4; ++left)
    {
        min_diff = std::min(nums[nums_size - 4 + left] - nums[left], min_diff);
    }

    return min_diff;

} // static int minDifferenceDS2( ...

TEST(MinDifferenceTest, SampleTest1)
{
    EXPECT_EQ(0, minDifferenceFA({5, 3, 2, 4}));
    EXPECT_EQ(0, minDifferenceDS1({5, 3, 2, 4}));
    EXPECT_EQ(0, minDifferenceDS2({5, 3, 2, 4}));
}

TEST(MinDifferenceTest, SampleTest2)
{
    EXPECT_EQ(1, minDifferenceFA({1, 5, 0, 10, 14}));
    EXPECT_EQ(1, minDifferenceDS1({1, 5, 0, 10, 14}));
    EXPECT_EQ(1, minDifferenceDS2({1, 5, 0, 10, 14}));
}

TEST(MinDifferenceTest, SampleTest3)
{
    EXPECT_EQ(0, minDifferenceFA({3, 100, 20}));
    EXPECT_EQ(0, minDifferenceDS1({3, 100, 20}));
    EXPECT_EQ(0, minDifferenceDS2({3, 100, 20}));
}

TEST(MinDifferenceTest, SampleTest4)
{
    const std::vector<int> nums {9, 48, 92, 48, 81, 31};

    EXPECT_EQ(33, minDifferenceFA(nums));
    EXPECT_NE(17, minDifferenceFA(nums));
    EXPECT_EQ(17, minDifferenceDS1(nums));
    EXPECT_EQ(17, minDifferenceDS2(nums));
}
