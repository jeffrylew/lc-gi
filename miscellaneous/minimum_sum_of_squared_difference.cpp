#include <gtest/gtest.h>

#include <cmath>
#include <queue>
#include <utility>
#include <vector>

//! @brief First attempt to minimize sum of squared differences
//! @param[in] nums1 Vector of zero and positive ints
//! @param[in] nums2 Vector of zero and positive ints
//! @param[in] k1    The number of times +1 and -1 can be applied to nums1
//! @param[in] k2    The number of times +1 and -1 can be applied to nums2
//! @return The minimum sum of squared differences
static long long minSumSquareDiffFA(const std::vector<int>& nums1,
                                    const std::vector<int>& nums2,
                                    int                     k1,
                                    int                     k2)
{
    //! @details https://leetcode.com/problems/minimum-sum-of-squared-difference
    //!
    //!          First attempt solution passes 28 / 39 testcases but has TLE.

    auto compare_diffs =
        [](const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) {
            return std::abs(lhs.first - lhs.second)
                < std::abs(rhs.first - rhs.second);
        };

    std::priority_queue<
        std::pair<int, int>,
        std::vector<std::pair<int, int>>,
        decltype(compare_diffs)> max_diff_heap {compare_diffs};

    const auto num_diffs = static_cast<int>(std::ssize(nums1));
    for (int idx = 0; idx < num_diffs; ++idx)
    {
        max_diff_heap.emplace(nums1[idx], nums2[idx]);
    }

    while (!max_diff_heap.empty() && k1 > 0)
    {
        const auto [nums1_elem, nums2_elem] = max_diff_heap.top();
        max_diff_heap.pop();

        if (nums1_elem == nums2_elem)
        {
            break;
        }

        max_diff_heap.emplace(
            (nums1_elem > nums2_elem) ? nums1_elem - 1 : nums1_elem + 1,
            nums2_elem);
        --k1;
    }

    while (!max_diff_heap.empty() && k2 > 0)
    {
        const auto [nums1_elem, nums2_elem] = max_diff_heap.top();
        max_diff_heap.pop();

        if (nums1_elem == nums2_elem)
        {
            break;
        }

        max_diff_heap.emplace(
            nums1_elem,
            (nums1_elem > nums2_elem) ? nums2_elem + 1 : nums2_elem - 1);
        --k2;
    }

    long long min_sum_diffs {};
    while (!max_diff_heap.empty())
    {
        const auto [nums1_elem, nums2_elem] = max_diff_heap.top();
        max_diff_heap.pop();

        const auto curr_diff = static_cast<long long>(nums1_elem - nums2_elem);
        min_sum_diffs += curr_diff * curr_diff;
    }

    return min_sum_diffs;
}

TEST(MinSumSquareDiff, SampleTest1)
{
    const std::vector<int> nums1 {1, 2, 3, 4};
    const std::vector<int> nums2 {2, 10, 20, 19};
    constexpr int          k1 {};
    constexpr int          k2 {};

    EXPECT_EQ(579LL, minSumSquareDiffFA(nums1, nums2, k1, k2));
}

TEST(MinSumSquareDiff, SampleTest2)
{
    const std::vector<int> nums1 {1, 4, 10, 12};
    const std::vector<int> nums2 {5, 8, 6, 9};
    constexpr int          k1 {1};
    constexpr int          k2 {1};

    EXPECT_EQ(43LL, minSumSquareDiffFA(nums1, nums2, k1, k2));
}
