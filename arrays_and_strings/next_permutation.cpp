#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

//! @brief First attempt solution to find next permutation of nums
//! @param[in, out] nums Reference to vector of ints to find next permutation of
static void nextPermutationFA(std::vector<int>& nums)
{
    const auto nums_size = static_cast<int>(std::ssize(nums));
    if (nums_size < 2)
    {
        return;
    }

    int left {nums_size - 2};
    int right {nums_size - 1};

    if (nums_size == 2)
    {
        std::swap(nums[left], nums[right]);
        return;
    }

    if (nums[left] <= nums[right])
    {
        std::swap(nums[left], nums[right]);
        return;
    }

    while (left > 0)
    {
        //! Combination of comparisons, shifting left/right, and swapping here
    }

} // static void nextPermutationFA( ...

TEST(NextPermutationTest, SampleTest1)
{
    const std::vector<int> expected_output {1, 3, 2};
    std::vector<int>       numsFA {1, 2, 3};

    nextPermutationFA(numsFA);
    EXPECT_EQ(expected_output, numsFA);
}

TEST(NextPermutationTest, SampleTest2)
{
    const std::vector<int> expected_output {1, 2, 3};
    std::vector<int>       numsFA {3, 2, 1};

    nextPermutationFA(numsFA);
    EXPECT_NE(expected_output, numsFA);
}

TEST(NextPermutationTest, SampleTest3)
{
    const std::vector<int> expected_output {1, 5, 1};
    std::vector<int>       numsFA {1, 1, 5};

    nextPermutationFA(numsFA);
    EXPECT_EQ(expected_output, numsFA);
}
