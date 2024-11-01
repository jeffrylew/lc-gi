#include <gtest/gtest.h>

#include <vector>

//! @brief First attempt to get shortest sorted list of ranges
//! @param[in] nums  Reference to vector of sorted unique integers in the range
//! @param[in] lower Lower bound of inclusive range [lower, upper]
//! @param[in] upper Upper bound of inclusive range [lower, upper]
//! @return Shortest sorted list of ranges that exactly covers all missing nums
static std::vector<std::vector<int>>
    findMissingRangesFA(const std::vector<int>& nums, int lower, int upper)
{
    //! @details https://leetcode.com/problems/missing-ranges
    //!
    //!          Time complexity O(N) where N = nums.size()
    //!          Space complexity O(1) excluding output vector

    if (nums.empty())
    {
        return {{lower, upper}};
    }

    std::vector<std::vector<int>> missing_ranges {};

    int prev {lower};

    if (nums.front() - lower != 0)
    {
        missing_ranges.push_back({lower, nums.front() - 1});
        prev = nums.front();
    }

    for (const int curr : nums)
    {
        if (curr - prev < 2)
        {
            prev = curr;
            continue;
        }

        missing_ranges.push_back({prev + 1, curr - 1});
        prev = curr;
    }

    if (upper - prev != 0)
    {
        missing_ranges.push_back({prev + 1, upper});
    }

    return missing_ranges;

} // static std::vector<std::vector<int>> findMissingRangesFA( ...

//! @brief Linear scan discussion solution
//! @param[in] nums  Reference to vector of sorted unique integers in the range
//! @param[in] lower Lower bound of inclusive range [lower, upper]
//! @param[in] upper Upper bound of inclusive range [lower, upper]
//! @return Shortest sorted list of ranges that exactly covers all missing nums
static std::vector<std::vector<int>>
    findMissingRangesDS(const std::vector<int>& nums, int lower, int upper)
{
    //! @details https://leetcode.com/problems/missing-ranges/editorial
    //!
    //!          Time complexity O(N) where N = nums.size(). Iterate over all
    //!          elements of nums and check whether an element differs by 1 or
    //!          greater from its succeeding element.
    //!          Space complexity O(1) ignoring space consumed by input/output

    std::vector<std::vector<int>> missing_ranges {};
    if (nums.empty())
    {
        missing_ranges.push_back({lower, upper});
        return missing_ranges;
    }

    const auto nums_size = static_cast<int>(std::ssize(nums));

    //! Check for any missing numbers between the lower bound and nums.front()
    if (lower < nums.front())
    {
        missing_ranges.push_back({lower, nums.front() - 1});
    }

    //! Check for any missing numbers between successive elements of nums
    for (int idx = 0; idx < nums_size - 1; ++idx)
    {
        if (nums[idx + 1] - nums[idx] <= 1)
        {
            continue;
        }

        missing_ranges.push_back({nums[idx] + 1, nums[idx + 1] - 1});
    }

    //! Check for missing numbers between last element of nums and upper bound
    if (upper > nums.back())
    {
        missing_ranges.push_back({nums.back() + 1, upper});
    }

    return missing_ranges;

} // static std::vector<std::vector<int>> findMissingRangesDS( ...

TEST(FindMissingRangesTest, SampleTest1)
{
    const std::vector<int> nums {0, 1, 3, 50, 75};
    constexpr int          lower {};
    constexpr int          upper {99};

    const std::vector<std::vector<int>> expected_output {
        {2, 2}, {4, 49}, {51, 74}, {76, 99}};

    EXPECT_EQ(expected_output, findMissingRangesFA(nums, lower, upper));
    EXPECT_EQ(expected_output, findMissingRangesDS(nums, lower, upper));
}

TEST(FindMissingRangesTest, SampleTest2)
{
    const std::vector<int> nums {-1};
    constexpr int          lower {-1};
    constexpr int          upper {-1};

    EXPECT_TRUE(findMissingRangesFA(nums, lower, upper).empty());
    EXPECT_TRUE(findMissingRangesDS(nums, lower, upper).empty());
}

TEST(FindMissingRangesTest, SampleTest3)
{
    const std::vector<int> nums {};
    constexpr int          lower {1};
    constexpr int          upper {1};

    const std::vector<std::vector<int>> expected_output {{1, 1}};

    EXPECT_EQ(expected_output, findMissingRangesFA(nums, lower, upper));
    EXPECT_EQ(expected_output, findMissingRangesDS(nums, lower, upper));
}

TEST(FindMissingRangesTest, SampleTest4)
{
    const std::vector<int> nums {-1};
    constexpr int          lower {-2};
    constexpr int          upper {-1};

    const std::vector<std::vector<int>> expected_output {{-2, -2}};

    EXPECT_EQ(expected_output, findMissingRangesFA(nums, lower, upper));
    EXPECT_EQ(expected_output, findMissingRangesDS(nums, lower, upper));
}

TEST(FindMissingRangesTest, SampleTest5)
{
    const std::vector<int> nums {-1};
    constexpr int          lower {-1};
    constexpr int          upper {};

    const std::vector<std::vector<int>> expected_output {{0, 0}};

    EXPECT_EQ(expected_output, findMissingRangesFA(nums, lower, upper));
    EXPECT_EQ(expected_output, findMissingRangesDS(nums, lower, upper));
}

TEST(FindMissingRangesTest, SampleTest6)
{
    const std::vector<int> nums {2};
    constexpr int          lower {};
    constexpr int          upper {9};

    const std::vector<std::vector<int>> expected_output {{0, 1}, {3, 9}};

    EXPECT_EQ(expected_output, findMissingRangesFA(nums, lower, upper));
    EXPECT_EQ(expected_output, findMissingRangesDS(nums, lower, upper));
}
