#include <gtest/gtest.h>

#include <algorithm>
#include <set>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

//! @brief First attempt to get all triplets that sum to zero
//! @param[in] nums Vector of integers of size in [3, 3000]
//! @return Vector of unique triplets with zero sum
static std::vector<std::vector<int>> threeSumFA(std::vector<int> nums)
{
    //! @details https://leetcode.com/problems/3sum/description/
    //!
    //!          First attempt solution passes 80 / 313 test cases.
    //!          Does not pass SampleTest4

    std::sort(nums.begin(), nums.end());

    int left {};
    if (nums[left] > 0)
    {
        return {};
    }

    const auto nums_size = static_cast<int>(std::ssize(nums));
    int        right {nums_size - 1};
    if (nums[right] < 0)
    {
        return {};
    }

    std::unordered_map<int, std::vector<int>> num_idx {};
    for (int idx = 0; idx < nums_size; ++idx)
    {
        num_idx[nums[idx]].push_back(idx);
    }

    std::vector<std::vector<int>>       all_triplets {};
    std::set<std::tuple<int, int, int>> unique_triplets {};

    while (left < right)
    {
        const int mid_num {0 - nums[left] - nums[right]};

        if (num_idx.contains(mid_num)
            && std::any_of(num_idx[mid_num].begin(),
                           num_idx[mid_num].end(),
                           [&](int idx) {
                               return idx != left && idx != right;
                           }))
        {
            std::vector<int> triplet_vec {nums[left], mid_num, nums[right]};
            std::sort(triplet_vec.begin(), triplet_vec.end());

            auto triplet = std::make_tuple(triplet_vec[0],
                                           triplet_vec[1],
                                           triplet_vec[2]);
            if (!unique_triplets.contains(triplet))
            {
                all_triplets.push_back(std::move(triplet_vec));
                unique_triplets.insert(std::move(triplet));
            }

            --right;
            continue;
        }

        mid_num <= 0 ? --right : ++left;
    }

    return all_triplets;

} // static std::vector<std::vector<int>> threeSumFA( ...

TEST(ThreeSumTest, SampleTest1)
{
    const std::vector<std::vector<int>> expected_output {
        {-1, -1, 2}, {-1, 0, 1}};

    EXPECT_EQ(expected_output, threeSumFA({-1, 0, 1, 2, -1, -4}));
}

TEST(ThreeSumTest, SampleTest2)
{
    const std::vector<std::vector<int>> expected_output {{0, 1, 1}};

    EXPECT_EQ(expected_output, threeSumFA({0, 1, 1}));
}

TEST(ThreeSumTest, SampleTest3)
{
    const std::vector<std::vector<int>> expected_output {{0, 0, 0}};

    EXPECT_EQ(expected_output, threeSumFA({0, 0, 0}));
}

TEST(ThreeSumTest, SampleTest4)
{
    const std::vector<int> nums {-1, 0, 1, 2, -1, -4, -2, -3, 3, 0, 4};

    const std::vector<std::vector<int>> expected_output {
        {-4, 0, 4},
        {-4, 1, 3},
        {-3, -1, 4},
        {-3, 0, 3},
        {-3, 1, 2},
        {-2, -1, 3},
        {-2, 0, 2},
        {-1, -1, 2},
        {-1, 0, 1}};

    const std::vector<std::vector<int>> first_attempt_output {
        {-4, 0, 4},
        {-4, 1, 3},
        {-3, 1, 2},
        {-3, 0, 3},
        {-3, -1, 4}};

    EXPECT_NE(expected_output, threeSumFA(nums));
    EXPECT_EQ(first_attempt_output, threeSumFA(nums));
}
