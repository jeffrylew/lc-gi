#include <gtest/gtest.h>

#include <vector>

//! @brief First attempt to get length of longest increasing path in matrix
//! @param[in] matrix Reference to 2D vector of m x n ints
//! @return Length of longest increasing path in matrix
static int longestIncreasingPathFA(const std::vector<std::vector<int>>& matrix)
{
    //! @details leetcode.com/problems/longest-increasing-path-in-a-matrix

    //! @todo

} // static int longestIncreasingPathFA( ...

TEST(LongestIncreasingPathTest, SampleTest1)
{
    const std::vector<std::vector<int>> matrix {
        {9, 9, 4}, {6, 6, 8}, {2, 1, 1}};

    EXPECT_EQ(4, longestIncreasingPathFA(matrix));
}

TEST(LongestIncreasingPathTest, SampleTest2)
{
    const std::vector<std::vector<int>> matrix {
        {3, 4, 5}, {3, 2, 6}, {2, 2, 1}};

    EXPECT_EQ(4, longestIncreasingPathFA(matrix));
}

TEST(LongestIncreasingPathTest, SampleTest3)
{
    const std::vector<std::vector<int>> matrix {{1}};

    EXPECT_EQ(1, longestIncreasingPathFA(matrix));
}
