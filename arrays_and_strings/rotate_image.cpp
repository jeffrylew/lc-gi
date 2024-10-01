#include <gtest/gtest.h>

#include <vector>

//! @brief First attempt to rotate image by 90 degrees clockwise
//! @param[in, out] matrix Reference to n x n 2D matrix representing an image
static void rotateFA(std::vector<std::vector<int>>& matrix)
{
    //! @todo

} // static void rotateFA( ...

TEST(RotateTest, SampleTest1)
{
    std::vector<std::vector<int>> matrixFA {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    const std::vector<std::vector<int>> expected_output {
        {7, 4, 1}, {8, 5, 2}, {9, 6, 3}};

    rotateFA(matrixFA);
    EXPECT_EQ(expected_output, matrixFA);
}

TEST(RotateTest, SampleTest2)
{
    std::vector<std::vector<int>> matrixFA {
        {5, 1, 9, 11}, {2, 4, 8, 10}, {13, 3, 6, 7}, {15, 14, 12, 16}};

    const std::vector<std::vector<int>> expected_output {
        {15, 13, 2, 5}, {14, 3, 4, 1}, {12, 6, 8, 9}, {16, 7, 10, 11}};

    rotateFA(matrixFA);
    EXPECT_EQ(expected_output, matrixFA);
}
