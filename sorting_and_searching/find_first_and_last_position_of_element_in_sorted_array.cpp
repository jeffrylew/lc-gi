#include <gtest/gtest.h>

#include <vector>

//! @brief First attempt to find start and end position of target value
//! @param[in] nums   Vector of sorted ints in non-decreasing order
//! @param[in] target Value to find start and end position of
//! @return Vector containing start and end position of target, or [-1, -1]
static std::vector<int> searchRangeFA(std::vector<int> nums, int target)
{
    //! @details https://leetcode.com/explore/interview/card/google/63
    //!          /sorting-and-searching-4/3081/

    

} // static std::vector<int> searchRangeFA( ...

TEST(SearchRangeTest, SampleTest1)
{
    const std::vector<int> expected_output {3, 4};

    EXPECT_EQ(expected_output, searchRangeFA({5, 7, 7, 8, 8, 10}, 8));
}

TEST(SearchRangeTest, SampleTest2)
{
    const std::vector<int> expected_output {-1, -1};

    EXPECT_EQ(expected_output, searchRangeFA({5, 7, 7, 8, 8, 10}, 6));
}

TEST(SearchRangeTest, SampleTest3)
{
    const std::vector<int> expected_output {-1, -1};

    EXPECT_EQ(expected_output, searchRangeFA({}, 0));
}
