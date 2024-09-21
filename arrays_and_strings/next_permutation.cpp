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

//! @brief Single pass discussion solution to find next permutation of nums
//! @param[in, out] nums Reference to vector of ints to find next permutation of
static void nextPermutationDS(std::vector<int>& nums)
{
    //! @details https://leetcode.com/problems/next-permutation/editorial/
    //!
    //!          Time complexity O(N) where N = nums.size(). In the worst case,
    //!          only two scans of the whole vector are needed.
    //!          Space complexity O(1)

    const auto nums_size = static_cast<int>(std::ssize(nums.size()));

    int pivot {nums_size - 1};

    //! Iterate from right to left
    //! Find successive numbers that satisfy nums[pivot - 1] < nums[pivot]
    //! nums[pivot - 1] is the digit to swap with if the pivot is not zero.
    //! No rearrangements to the right of nums[pivot - 1] can create a larger
    //! permutation since that subarray consists of descending numbers.
    while (pivot > 0 && nums[pivot - 1] >= nums[pivot])
    {
        --pivot;
    }

    if (pivot != 0)
    {
        int next_bigger_num_idx {nums_size - 1};

        //! Iterate from right to left to find best digit to swap with
        //! i.e. Find next digit just bigger than nums[pivot - 1], satisfying
        //!      nums[pivot - 1] < nums[next_bigger_num_idx]
        //! Can also use binary search here since numbers in
        //! [pivot, nums_size - 1] are descending
        while (nums[pivot - 1] >= nums[next_bigger_num_idx])
        {
            --next_bigger_num_idx;
        }

        //! Swap so correct number is at index pivot - 1. This does not change
        //! the descending order of the subarray to the right of pivot - 1
        std::swap(nums[pivot - 1], nums[next_bigger_num_idx]);
    }

    //! Reverse sequence after (pivot - 1) until the end to minimize value and
    //! get next smallest lexicographic permutation
    std::reverse(nums.begin() + pivot, nums.end());

} // static void nextPermutationDS( ...

TEST(NextPermutationTest, SampleTest1)
{
    const std::vector<int> expected_output {1, 3, 2};
    std::vector<int>       numsFA {1, 2, 3};

    auto numsDS = numsFA;

    nextPermutationFA(numsFA);
    EXPECT_EQ(expected_output, numsFA);

    nextPermutationDS(numsDS);
    EXPECT_EQ(expected_output, numsDS);
}

TEST(NextPermutationTest, SampleTest2)
{
    const std::vector<int> expected_output {1, 2, 3};
    std::vector<int>       numsFA {3, 2, 1};

    auto numsDS = numsFA;

    nextPermutationFA(numsFA);
    EXPECT_NE(expected_output, numsFA);

    nextPermutationDS(numsDS);
    EXPECT_EQ(expected_output, numsDS);
}

TEST(NextPermutationTest, SampleTest3)
{
    const std::vector<int> expected_output {1, 5, 1};
    std::vector<int>       numsFA {1, 1, 5};

    auto numsDS = numsFA;

    nextPermutationFA(numsFA);
    EXPECT_EQ(expected_output, numsFA);

    nextPermutationDS(numsDS);
    EXPECT_EQ(expected_output, numsDS);
}
