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
    if (nums.empty())
    {
        return {};
    }

    const auto nums_size = static_cast<int>(std::ssize(nums));

    std::vector<std::vector<int>> missing_ranges {};

    int left {lower};

    for (int right_idx = 0; right_idx < nums_size; ++right_idx)
    {
        //! @todo Continue
    }

    return missing_ranges;

} // static std::vector<std::vector<int>> findMissingRangesFA( ...

TEST(FindMissingRangesTest, SampleTest1)
{
    const std::vector<int> nums {0, 1, 3, 50, 75};
    constexpr int          lower {};
    constexpr int          upper {99};

    const std::vector<std::vector<int>> expected_output {
        {2, 2}, {4, 49}, {51, 74}, {76, 99}};

    EXPECT_EQ(expected_output, findMissingRangesFA(nums, lower, upper));
}

TEST(FindMissingRangesTest, SampleTest2)
{
    const std::vector<int> nums {-1};
    constexpr int          lower {-1};
    constexpr int          upper {-1};

    EXPECT_TRUE(findMissingRangesFA(nums, lower, upper).empty());
}
