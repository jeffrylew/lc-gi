#include <gtest/gtest.h>

#include <vector>

//! @brief First attempt to merge overlapping intervals
//! @param[in] intervals Reference to vector of intervals
//! @return Vector of non-overlapping intervals that cover all input intervals
static std::vector<std::vector<int>> mergeFA(
    const std::vector<std::vector<int>>& intervals)
{
    //! @details https://leetcode.com/explore/interview/card/google/63
    //!          /sorting-and-searching-4/450/

    //! @todo

} // static std::vector<std::vector<int>> mergeFA( ...

TEST(MergeTest, SampleTest1)
{
    const std::vector<std::vector<int>> intervals {
        {1, 3}, {2, 6}, {8, 10}, {15, 18}};

    const std::vector<std::vector<int>> expected_output {
        {1, 6}, {8, 10}, {15, 18}};

    EXPECT_EQ(expected_output, mergeFA(intervals));
}

TEST(MergeTest, SampleTest2)
{
    const std::vector<std::vector<int>> intervals {{1, 4}, {4, 5}};
    const std::vector<std::vector<int>> expected_output {{1, 5}};

    EXPECT_EQ(expected_output, mergeFA(intervals));
}
