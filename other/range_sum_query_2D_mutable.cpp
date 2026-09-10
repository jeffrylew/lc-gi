#include <gtest/gtest.h>

#include <vector>

//! @class NumMatrixFA
//! @brief First attempt solution to implement NumMatrix class
//! @details https://leetcode.com/explore/interview/card/google/66/others-4/477/
//!
//!          Time complexity O(R * C) where R = the number of rows and C = the
//!          number of columns in matrix. Initializing the prefix_sums and
//!          matrix_copy 2D vectors in the constructor costs O(R * C). update()
//!          costs O(C) in the worst case when col = 0 is passed, since all cols
//!          are iterated for the input row. sumRegion() costs O(R) in the worst
//!          case when row1 = 0 and row2 = matrix.size().
class NumMatrixFA
{
public:
    NumMatrixFA(const std::vector<std::vector<int>>& matrix)
        : num_rows {static_cast<int>(std::ssize(matrix))}
        , num_cols {static_cast<int>(std::ssize(matrix[0]))}
        , matrix_copy {matrix}
        , prefix_sums(num_rows, std::vector<int>(1 + num_cols, 0))
    {
        for (int row = 0; row < num_rows; ++row)
        {
            for (int col = 0; col < num_cols; ++col)
            {
                prefix_sums[row][col + 1] =
                    prefix_sums[row][col] + matrix[row][col];
            }
        }
    }

    void update(int row, int col, int val)
    {
        const int delta_val {val - matrix_copy[row][col]};
        matrix_copy[row][col] = val;

        for (int prefix_col = col + 1; prefix_col <= num_cols; ++prefix_col)
        {
            prefix_sums[row][prefix_col] += delta_val;
        }
    }

    int sumRegion(int row1, int col1, int row2, int col2)
    {
        int region_sum {};

        for (int row = row1; row <= row2; ++row)
        {
            region_sum += prefix_sums[row][col2 + 1] - prefix_sums[row][col1];
        }

        return region_sum;
    }

private:
    int num_rows {};
    int num_cols {};

    std::vector<std::vector<int>> matrix_copy;
    std::vector<std::vector<int>> prefix_sums;
};

//! @class NumMatrixDS1
//! @brief Brute force discussion solution
//! @details https://leetcode.com/problems/range-sum-query-2d-mutable/editorial/
class NumMatrixDS1
{
public:
    NumMatrixDS1(const std::vector<std::vector<int>>& matrix)
        : matrix_copy {matrix}
    {
    }

    void update(int row, int col, int val)
    {
        matrix_copy[row][col] = val;
    }

    int sumRegion(int row1, int col1, int row2, int col2)
    {
        int region_sum {};

        for (int row = row1; row <= row2; ++row)
        {
            for (int col = col1; col <= col2; ++col)
            {
                region_sum += matrix_copy[row][col];
            }
        }

        return region_sum;
    }

private:
    std::vector<std::vector<int>> matrix_copy;
};

TEST(NumMatrixTest, SampleTest1)
{
    const std::vector<std::vector<int>> matrix {
        {3, 0, 1, 4, 2},
        {5, 6, 3, 2, 1},
        {1, 2, 0, 1, 5},
        {4, 1, 0, 1, 7},
        {1, 0, 3, 0, 5}};

    NumMatrixFA num_matrix_fa {matrix};

    /*
     Indices:
      0  1  2  3  4  5
     Prefix sums:
     00 03 03 04 08 10
     00 05 11 14 16 17
     00 01 03 03 04 09 -> prefix_sums[2][4] - prefix_sums[2][1] = 4 - 1 = 3
     00 04 05 05 06 13 -> prefix_sums[3][4] - prefix_sums[3][1] = 6 - 4 = 2
     00 01 01 04 04 09 -> prefix_sums[4][4] - prefix_sums[4][1] = 4 - 1 = 3
                                                                  Total = 8
     */
    EXPECT_EQ(8, num_matrix_fa.sumRegion(2, 1, 4, 3));

    /*
     Indices:
     0 1 2 3 4    0 1 2 3 4    0  1  2  3  4  5     0  1  2  3  4  5
     matrix:                  Prefix sums:
     3 0 1 4 2    3 0 1 4 2   00 03 03 04 08 10    00 03 03 04 08 10
     5 6 3 2 1    5 6 3 2 1   00 05 11 14 16 17    00 05 11 14 16 17
     1 2 0 1 5 -> 1 2 0 1 5   00 01 03 03 04 09 -> 00 01 03 03 04 09
     4 1 0 1 7    4 1 2 1 7   00 04 05 05 06 13    00 04 05 07 08 15
     1 0 3 0 5    1 0 3 0 5   00 01 01 04 04 09    00 01 01 04 04 09
     */
    num_matrix_fa.update(3, 2, 2);
    EXPECT_EQ(10, num_matrix_fa.sumRegion(2, 1, 4, 3));

    NumMatrixDS1 num_matrix_ds1 {matrix};
    EXPECT_EQ(8, num_matrix_ds1.sumRegion(2, 1, 4, 3));
    num_matrix_ds1.update(3, 2, 2);
    EXPECT_EQ(10, num_matrix_ds1.sumRegion(2, 1, 4, 3));
}

TEST(NumMatrixTest, SampleTest2)
{
    /*
     Indices
      0   1   2
     Prefix sums:
     00  02  06
     00 -03  02
     */
    const std::vector<std::vector<int>> matrix {{2, 4}, {-3, 5}};

    NumMatrixFA num_matrix_fa {matrix};

    /*
     Indices:
      0  1     0  1    0   1   2     0   1   2
     matrix:          Prefix sums:
      2  4 ->  2  3   00  02  06 -> 00  02  05
     -3  5    -3  5   00 -03  02    00 -03  02
     */
    num_matrix_fa.update(0, 1, 3);

    /*
     Indices:
      0  1     0  1    0   1   2     0   1   2
     matrix:          Prefix sums:
      2  3 ->  2  3   00  02  05 -> 00  02  05
     -3  5    -3 -3   00 -03  02    00 -03 -06
     */
    num_matrix_fa.update(1, 1, -3);

    /*
     Indices:
      0  1     0  1    0   1   2     0   1   2
     matrix:          Prefix sums:
      2  3 ->  2  1   00  02  05 -> 00  02  03
     -3 -3    -3 -3   00 -03 -06    00 -03 -06
     */
    num_matrix_fa.update(0, 1, 1);

    /*
     Indices
      0   1   2
     Prefix sums:
     00  02  03 -> prefix_sums[0][2] - prefix_sums[0][0] = 3 - 0  = 3
     00 -03 -06 -> prefix_sums[1][2] - prefix_sums[1][0] = -6 - 0 = -6
                                                            Total = -3
     */
    EXPECT_EQ(-3, num_matrix_fa.sumRegion(0, 0, 1, 1));

    NumMatrixDS1 num_matrix_ds1 {matrix};
    num_matrix_ds1.update(0, 1, 3);
    num_matrix_ds1.update(1, 1, -3);
    num_matrix_ds1.update(0, 1, 1);
    EXPECT_EQ(-3, num_matrix_ds1.sumRegion(0, 0, 1, 1));
}
