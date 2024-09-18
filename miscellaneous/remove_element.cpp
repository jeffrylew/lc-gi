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

TEST(RemoveElementTest, SampleTest1)
{
    std::vector<int> numsFA {3, 2, 2, 3};

    EXPECT_EQ(2, removeElementFA(numsFA, 3));
    EXPECT_EQ(2, numsFA[0]);
    EXPECT_EQ(2, numsFA[1]);
}

TEST(RemoveElementTest, SampleTest2)
{
    std::vector<int> numsFA {0, 1, 2, 2, 3, 0, 4, 2};

    EXPECT_EQ(5, removeElementFA(numsFA, 2));
    EXPECT_EQ(0, numsFA[0]);
    EXPECT_EQ(1, numsFA[1]);
    EXPECT_EQ(3, numsFA[2]);
    EXPECT_EQ(0, numsFA[3]);
    EXPECT_EQ(4, numsFA[4]);
}
