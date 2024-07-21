#include <gtest/gtest.h>

#include <algorithm>
#include <limits>
#include <utility>
#include <vector>

//! @brief First attempt to get all matrix lucky nums (min in row, max in col)
//! @param[in] matrix Reference to an M x N matrix of distinct numbers
static std::vector<int> luckyNumbersFA(
    const std::vector<std::vector<int>>& matrix)
{
    //! @details https://leetcode.com/problems/lucky-numbers-in-a-matrix
    //!
    //!          Time complexity O(M * N)
    //!          Space complexity O(M + N)

    std::vector<int>                 lucky_nums {};
    std::vector<int>                 max_in_col(matrix[0].size());
    std::vector<std::pair<int, int>> min_in_row {};
    min_in_row.reserve(matrix.size());

    for (const auto& row : matrix)
    {
        min_in_row.emplace_back(row[0], 0);

        for (int col = 0; col < std::ssize(row); ++col)
        {
            if (row[col] < min_in_row.back().first)
            {
                min_in_row.back() = {row[col], col};
            }
            max_in_col[col] = std::max(max_in_col[col], row[col]);
        }
    }

    for (const auto& [min_val, col_idx] : min_in_row)
    {
        if (min_val == max_in_col[col_idx])
        {
            lucky_nums.push_back(min_val);
        }
    }

    return lucky_nums;

} // static std::vector<int> luckyNumbersFA( ...

//! @brief Simulation discussion solution
//! @param[in] matrix Reference to an M x N matrix of distinct numbers
static std::vector<int> luckyNumbersDS1(
    const std::vector<std::vector<int>>& matrix)
{
    //! @details https://leetcode.com/problems/lucky-numbers-in-a-matrix
    //!
    //!          Time complexity O(N * M) where N = num rows, M = num cols. To
    //!          store min of each row, require N * M operations and same for
    //!          storing max of each column. Need to iterate over each integer
    //!          to find lucky numbers.
    //!          Space complexity O(N + M) for row_min and col_max of sizes N
    //!          and M respectively.

    const auto num_rows = static_cast<int>(matrix.size());
    const auto num_cols = static_cast<int>(matrix[0].size());

    std::vector<int> row_min {};
    row_min.reserve(matrix.size());
    for (int row = 0; row < num_rows; ++row)
    {
        int r_min {std::numeric_limits<int>::max()};
        for (int col = 0; col < num_cols; ++col)
        {
            r_min = std::min(r_min, matrix[row][col]);
        }
        row_min.push_back(r_min);
    }

    std::vector<int> col_max {};
    col_max.reserve(matrix[0].size());
    for (int col = 0; col < num_cols; ++col)
    {
        int c_max {std::numeric_limits<int>::min()};
        for (int row = 0; row < num_rows; ++row)
        {
            c_max = std::max(c_max, matrix[row][col]);
        }
        col_max.push_back(c_max);
    }

    std::vector<int> lucky_numbers {};

    for (int row = 0; row < num_rows; ++row)
    {
        for (int col = 0; col < num_cols; ++col)
        {
            if (matrix[row][col] == row_min[row]
                && matrix[row][col] == col_max[col])
            {
                lucky_numbers.push_back(matrix[row][col]);
            }
        }
    }

    return lucky_number;

} // static std::vector<int> luckyNumbersDS1( ...

//! @brief Greedy discussion solution
//! @param[in] matrix Reference to an M x N matrix of distinct numbers
static std::vector<int> luckyNumbersDS2(
    const std::vector<std::vector<int>>& matrix)
{
    //! @details https://leetcode.com/problems/lucky-numbers-in-a-matrix
    //!
    //!          Time complexity O(N * M) where N = num rows, M = num cols. To
    //!          find r_min_max and c_max_min, iterate over each int in matrix.
    //!          Space complexity O(1). No extra space apart from few variables.

    const auto num_rows = static_cast<int>(matrix.size());
    const auto num_cols = static_cast<int>(matrix[0].size());

    //! Maximum of minimum elements from each row
    int r_min_max {std::numeric_limits<int>::min()};
    for (int row = 0; row < num_rows; ++row)
    {
        int r_min {std::numeric_limits<int>::max()};
        for (int col = 0; col < num_cols; ++col)
        {
            r_min = std::min(r_min, matrix[row][col]);
        }
        r_min_max = std::max(r_min_max, r_min);
    }

    //! Minimum of maximum elements from each column
    int c_max_min {std::numeric_limits<int>::max()};
    for (int col = 0; col < num_cols; ++col)
    {
        int c_max {std::numeric_limits<int>::min()};
        for (int row = 0; row < num_rows; ++row)
        {
            c_max = std::max(c_max, matrix[row][col]);
        }
        c_max_min = std::min(c_max_min, c_max);
    }

    if (r_min_max == c_max_min)
    {
        return {r_min_max};
    }

    return {};

} // static std::vector<int> luckyNumbersDS2( ...

TEST(LuckyNumbersTest, SampleTest1)
{
    const std::vector<std::vector<int>> matrix {
        {3, 7, 8}, {9, 11, 13}, {15, 16, 17}};
    const std::vector<int> expected_output {15};

    EXPECT_EQ(expected_output, luckyNumbersFA(matrix));
    EXPECT_EQ(expected_output, luckyNumbersDS1(matrix));
    EXPECT_EQ(expected_output, luckyNumbersDS2(matrix));
}

TEST(LuckyNumbersTest, SampleTest2)
{
    const std::vector<std::vector<int>> matrix {
        {1, 10, 4, 2}, {9, 3, 8, 7}, {15, 16, 17, 12}};
    const std::vector<int> expected_output {12};

    EXPECT_EQ(expected_output, luckyNumbersFA(matrix));
    EXPECT_EQ(expected_output, luckyNumbersDS1(matrix));
    EXPECT_EQ(expected_output, luckyNumbersDS2(matrix));
}

TEST(LuckyNumbersTest, SampleTest3)
{
    const std::vector<std::vector<int>> matrix {{7, 8}, {1, 2}};
    const std::vector<int>              expected_output {7};

    EXPECT_EQ(expected_output, luckyNumbersFA(matrix));
    EXPECT_EQ(expected_output, luckyNumbersDS1(matrix));
    EXPECT_EQ(expected_output, luckyNumbersDS2(matrix));
}
