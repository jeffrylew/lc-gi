#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

//! @brief First attempt to rotate image by 90 degrees clockwise
//! @param[in, out] matrix Reference to n x n 2D matrix representing an image
static void rotateFA(std::vector<std::vector<int>>& matrix)
{
    //! @details https://leetcode.com/problems/rotate-image/description/
    //!
    //!          Time complexity O(N ^ 2) where N = matrix.size()
    //!          Space complexity O(1)
    //!
    //! swap_start_idx is index in each row of matrix to start performing swaps
    //! For example,
    //!     matrix:  row 0   row 1   row 2
    //!    indices:  0 1 2   3 4 5   6 7 8
    //!   elements: [1 2 3 | 4 5 6 | 7 8 9]
    //!             -----------------------
    //! row 0 swap: [1 2 3 | 4 5 6 | 7 8 9]
    //!                ^ swap_start_idx
    //!             [1 4 3 | 2 5 6 | 7 8 9]
    //!             [1 4 7 | 2 5 6 | 3 8 9]
    //!             -----------------------
    //! row 1 swap: [1 4 7 | 2 5 6 | 3 8 9]
    //!                          ^ swap_start_idx shifted over
    //!             [1 4 7 | 2 5 8 | 3 6 9]
    //!
    //! Once swap_start_idx is at index 5, it cannot be shifted for the last row
    //! corresponding to indices 6 through 8 in elements. Just need to reverse
    //! order of numbers in index ranges [0, 2], [3, 5], and [6, 8] to get
    //!             [7 4 1 | 8 5 2 | 9 6 3]
    //!
    //! A 4 x 4 example:
    //!     matrix:     row 0         row 1         row 2         row 3
    //!    indices:  00 01 02 03   04 05 06 07   08 09 10 11   12 13 14 15
    //!   elements: [05 01 09 11 | 02 04 08 10 | 13 03 06 07 | 15 14 12 16]
    //!             -------------------------------------------------------
    //! row 0 swap: [05 01 09 11 | 02 04 08 10 | 13 03 06 07 | 15 14 12 16]
    //!                  ^ swap_start_idx
    //!             [05 02 09 11 | 01 04 08 10 | 13 03 06 07 | 15 14 12 16]
    //!             [05 02 13 11 | 01 04 08 10 | 09 03 06 07 | 15 14 12 16]
    //!             [05 02 13 15 | 01 04 08 10 | 09 03 06 07 | 11 14 12 16]
    //!             -------------------------------------------------------
    //! row 1 swap: [05 02 13 15 | 01 04 08 10 | 09 03 06 07 | 11 14 12 16]
    //!                                   ^ swap_start_idx
    //!             [05 02 13 15 | 01 04 03 10 | 09 08 06 07 | 11 14 12 16]
    //!             [05 02 13 15 | 01 04 03 14 | 09 08 06 07 | 11 10 12 16]
    //!             -------------------------------------------------------
    //! row 2 swap: [05 02 13 15 | 01 04 03 14 | 09 08 06 07 | 11 10 12 16]
    //!                                                    ^ swap_start_idx
    //!             [05 02 13 15 | 01 04 03 14 | 09 08 06 12 | 11 10 07 16]
    //!
    //! Finally, reverse the orders of numbers within each row at the end to get
    //!             [15 13 02 05 | 14 03 04 01 | 12 06 08 09 | 16 07 10 11]

    const auto matrix_size = static_cast<int>(std::ssize(matrix));
    if (matrix_size == 1)
    {
        return;
    }

    //! @note As a memory optimization, can remove swap_start_idx and
    //!       ++swap_start_idx. Replace swap_start_idx with curr_row_idx + 1
    int swap_start_idx {1};

    for (int curr_row_idx = 0; curr_row_idx < matrix_size - 1; ++curr_row_idx)
    {
        int next_row_idx {curr_row_idx + 1};

        for (int col_swap_idx = swap_start_idx;
             col_swap_idx < matrix_size;
             ++col_swap_idx)
        {
            std::swap(matrix[curr_row_idx][col_swap_idx],
                      matrix[next_row_idx++][swap_start_idx - 1]);
        }

        ++swap_start_idx;
    }

    for (auto& row : matrix)
    {
        std::reverse(row.begin(), row.end());
    }

} // static void rotateFA( ...

//! @brief Rotate groups of four cells discussion solution
//! @param[in, out] matrix Reference to n x n 2D matrix representing an image
static void rotateDS1(std::vector<std::vector<int>>& matrix)
{
    //! @details https://leetcode.com/problems/rotate-image/description/
    //!
    //!          Time complexity O(N ^ 2) where N = matrix.size()
    //!          Space complexity O(1)

    const auto N = static_cast<int>(std::ssize(matrix));

    for (int i = 0; i < (N + 1) / 2; ++i)
    {
        for (int j = 0; j < N / 2; ++j)
        {
            //! Save bottom left corner (initially).
            const int temp {matrix[N - 1 - j][i]};

            //! Set bottom left corner to bottom right corner (initially)
            matrix[N - 1 - j][i] = matrix[N - 1 - i][N - j - 1];

            //! Set bottom right corner to top right corner (initially)
            matrix[N - 1 - i][N - j - 1] = matrix[j][N - 1 - i];

            //! Set top right corner to top left corner (initially)
            matrix[j][N - 1 - i] = matrix[i][j];

            //! Set top left corner to initial bottom left corner value
            matrix[i][j] = temp;
        }
    }

} // static void rotateDS1( ...

//! @brief Reverse on diagonal then reverse left to right discussion solution
//! @param[in, out] matrix Reference to n x n 2D matrix representing an image
static void rotateDS2(std::vector<std::vector<int>>& matrix)
{
    //! @details https://leetcode.com/problems/rotate-image/description/
    //!
    //!          Time complexity O(M) where M = number of cells in the grid.
    //!          Transposing the matrix takes O(M) since we move the value of
    //!          each cell once. Reversing each row has a cost of O(M) since we
    //!          move the value of each cell once.
    //!          Space complexity O(1)

    const auto N = static_cast<int>(std::ssize(matrix));

    const auto transpose = [&] {
        for (int i = 0; i < N; ++i)
        {
            for (int j = i + 1; j < N; ++j)
            {
                std::swap(matrix[j][i], matrix[i][j]);
            }
        }
    };

    const auto reflect = [&] {
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N / 2; ++j)
            {
                std::swap(matrix[i][j], matrix[i][N - j - 1]);
            }
        }
    };

    transpose(matrix);
    reflect(matrix);

} // static void rotateDS2( ...

TEST(RotateTest, SampleTest1)
{
    std::vector<std::vector<int>> matrixFA {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    auto matrixDS1 = matrixFA;
    auto matrixDS2 = matrixFA;

    const std::vector<std::vector<int>> expected_output {
        {7, 4, 1}, {8, 5, 2}, {9, 6, 3}};

    rotateFA(matrixFA);
    EXPECT_EQ(expected_output, matrixFA);

    rotateDS1(matrixDS1);
    EXPECT_EQ(expected_output, matrixDS1);

    rotateDS2(matrixDS2);
    EXPECT_EQ(expected_output, matrixDS2);
}

TEST(RotateTest, SampleTest2)
{
    std::vector<std::vector<int>> matrixFA {
        {5, 1, 9, 11}, {2, 4, 8, 10}, {13, 3, 6, 7}, {15, 14, 12, 16}};

    auto matrixDS1 = matrixFA;
    auto matrixDS2 = matrixFA;

    const std::vector<std::vector<int>> expected_output {
        {15, 13, 2, 5}, {14, 3, 4, 1}, {12, 6, 8, 9}, {16, 7, 10, 11}};

    rotateFA(matrixFA);
    EXPECT_EQ(expected_output, matrixFA);

    rotateDS1(matrixDS1);
    EXPECT_EQ(expected_output, matrixDS1);

    rotateDS2(matrixDS2);
    EXPECT_EQ(expected_output, matrixDS2);
}
