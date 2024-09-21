#include <gtest/gtest.h>

#include <algorithm>
#include <iterator>
#include <vector>

//! @brief First attempt to remove val from nums in-place
//! @param[in, out] nums Reference to vector of ints to modify
//! @param[in]      val  Int to remove from nums
//! @return Number of elements in nums that are not equal to val
static int removeElementFA(std::vector<int>& nums, int val)
{
    //! @details https://leetcode.com/problems/remove-element/
    //!
    //!          Time complexity O(N) where N = nums.size()
    //!          Space complexity O(1)

    auto end_it = std::remove(nums.begin(), nums.end(), val);
    return static_cast<int>(std::distance(nums.begin(), end_it));
}

//! @brief Two pointers discussion solution to remove val from nums in-place
//! @param[in, out] nums Reference to vector of ints to modify
//! @param[in]      val  Int to remove from nums
//! @return Number of elements in nums that are not equal to val
static int removeElementDS1(std::vector<int>& nums, int val)
{
    //! @details https://leetcode.com/problems/remove-element/editorial/
    //!
    //!          Time complexity O(N) where N = nums.size(). Both slow_idx and
    //!          fast_idx traverse at most 2N steps.
    //!          Space complexity O(1)

    const auto nums_size = static_cast<int>(std::ssize(nums));

    int slow_idx {};

    for (int fast_idx = 0; fast_idx < nums_size; ++fast_idx)
    {
        if (nums[fast_idx] != val)
        {
            nums[slow_idx++] = nums[fast_idx];
        }
    }

    return slow_idx;
}

//! @brief Two pointers for rare element removal discussion solution
//! @param[in, out] nums Reference to vector of ints to modify
//! @param[in]      val  Int to remove from nums
//! @return Number of elements in nums that are not equal to val
static int removeElementDS2(std::vector<int>& nums, int val)
{
    //! @details https://leetcode.com/problems/remove-element/editorial/
    //!
    //!          Time complexity O(N), where N = nums.size(). Both idx and
    //!          num_steps traverse at most N steps. The number of assignment
    //!          operations is equal to the number of elements to remove so it
    //!          is more efficient if elements to remove are rare.
    //!          Space complexity O(1)

    auto num_steps = static_cast<int>(std::ssize(nums));

    int idx {};

    while (idx < num_steps)
    {
        if (nums[idx] == val)
        {
            //! Swap current element with the last one and dispose the last one
            //! In the next iteration, will check if this last element is the
            //! value we want to remove
            nums[idx] = nums[num_steps - 1];

            //! Reduce vector size by one
            --num_steps;
        }
        else
        {
            ++idx;
        }
    }

    return num_steps;
}

TEST(RemoveElementTest, SampleTest1)
{
    std::vector<int> numsFA {3, 2, 2, 3};

    auto numsDS1 = numsFA;
    auto numsDS2 = numsFA;

    EXPECT_EQ(2, removeElementFA(numsFA, 3));
    EXPECT_EQ(2, numsFA[0]);
    EXPECT_EQ(2, numsFA[1]);

    EXPECT_EQ(2, removeElementDS1(numsDS1, 3));
    EXPECT_EQ(2, numsDS1[0]);
    EXPECT_EQ(2, numsDS1[1]);

    EXPECT_EQ(2, removeElementDS2(numsDS2, 3));
    EXPECT_EQ(2, numsDS2[0]);
    EXPECT_EQ(2, numsDS2[1]);
}

TEST(RemoveElementTest, SampleTest2)
{
    std::vector<int> numsFA {0, 1, 2, 2, 3, 0, 4, 2};

    auto numsDS1 = numsFA;
    auto numsDS2 = numsFA;

    EXPECT_EQ(5, removeElementFA(numsFA, 2));
    EXPECT_EQ(0, numsFA[0]);
    EXPECT_EQ(1, numsFA[1]);
    EXPECT_EQ(3, numsFA[2]);
    EXPECT_EQ(0, numsFA[3]);
    EXPECT_EQ(4, numsFA[4]);

    EXPECT_EQ(5, removeElementDS1(numsDS1, 2));
    EXPECT_EQ(0, numsDS1[0]);
    EXPECT_EQ(1, numsDS1[1]);
    EXPECT_EQ(3, numsDS1[2]);
    EXPECT_EQ(0, numsDS1[3]);
    EXPECT_EQ(4, numsDS1[4]);

    EXPECT_EQ(5, removeElementDS2(numsDS2, 2));
    EXPECT_EQ(0, numsDS2[0]);
    EXPECT_EQ(1, numsDS2[1]);
    EXPECT_EQ(3, numsDS2[2]);
    EXPECT_EQ(0, numsDS2[3]);
    EXPECT_EQ(4, numsDS2[4]);
}
