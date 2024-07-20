#include <gtest/gtest.h>

#include <algorithm>
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

TEST(LuckyNumbersTest, SampleTest1)
{
    const std::vector<std::vector<int>> matrix {
        {3, 7, 8}, {9, 11, 13}, {15, 16, 17}};
    const std::vector<int> expected_output {15};

    EXPECT_EQ(expected_output, luckyNumbersFA(matrix));
}

TEST(LuckyNumbersTest, SampleTest2)
{
    const std::vector<std::vector<int>> matrix {
        {1, 10, 4, 2}, {9, 3, 8, 7}, {15, 16, 17, 12}};
    const std::vector<int> expected_output {12};

    EXPECT_EQ(expected_output, luckyNumbersFA(matrix));
}

TEST(LuckyNumbersTest, SampleTest3)
{
    const std::vector<std::vector<int>> matrix {{7, 8}, {1, 2}};
    const std::vector<int>              expected_output {7};

    EXPECT_EQ(expected_output, luckyNumbersFA(matrix));
}
