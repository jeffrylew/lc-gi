#include <gtest/gtest.h>

#include <vector>

//! @brief First attempt to get the length of the longest consecutive sequence
//! @param[in] nums Reference to vector of unsorted integers
//! @return Length of the longest consecutive elements sequence
static int longestConsecutiveFA(const std::vector<int>& nums)
{
    //! @details https://leetcode.com/problems/longest-consecutive-sequence

    //! @todo
}

TEST(LongestConsecutiveTest, SampleTest1)
{
    const std::vector<int> nums {100, 4, 200, 1, 3, 2};

    EXPECT_EQ(4, longestConsecutiveFA(nums));
}

TEST(LongestConsecutiveTest, SampleTest2)
{
    const std::vector<int> nums {0, 3, 7, 2, 5, 8, 4, 6, 0, 1};

    EXPECT_EQ(9, longestConsecutiveFA(nums));
}

TEST(LongestConsecutiveTest, SampleTest3)
{
    const std::vector<int> nums {1, 0, 1, 2};

    EXPECT_EQ(3, longestConsecutiveFA(nums));
}
