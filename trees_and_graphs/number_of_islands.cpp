#include <gtest/gtest.h>

#include <functional>
#include <queue>
#include <utility>
#include <vector>

//! @brief Get number of islands from an M x N grid of 1s (land) and 0s (water)
//! @param[in] grid Reference to an M x N 2D grid of 1s (land) and 0s (water)
//! @return Number of islands
static int numIslandsFA(const std::vector<std::vector<char>>& grid)
{
    //! @details https://leetcode.com/problems/number-of-islands/description
    //!
    //!          First attempt solution has Time Limit Exceeded for SampleTest3
    //!          but passes 39 / 49 test cases.
    //!
    //!          Updating solution to mark each neighbor with '0' before adding
    //!          to coords queue passes all test cases.

    auto       grid_cpy = grid;
    const auto num_rows = static_cast<int>(std::ssize(grid));
    const auto num_cols = static_cast<int>(std::ssize(grid[0]));

    const auto is_valid = [&](int row_in, int col_in) -> bool {
        return row_in >= 0 
            && row_in < num_rows
            && col_in >= 0
            && col_in < num_cols;
    };

    int num_islands {};

    const std::vector<std::pair<int, int>> directions {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    std::queue<std::pair<int, int>> coords {};

    for (int row = 0; row < num_rows; ++row)
    {
        for (int col = 0; col < num_cols; ++col)
        {
            if (grid_cpy[row][col] == '0')
            {
                continue;
            }

            ++num_islands;

            grid_cpy[row][col] = '0';
            coords.emplace(row, col);

            while (!coords.empty())
            {
                const auto [curr_row, curr_col] = coords.front();
                coords.pop();

                for (const auto& [drow, dcol] : directions)
                {
                    const int next_row {curr_row + drow};
                    const int next_col {curr_col + dcol};

                    if (!is_valid(next_row, next_col)
                        || grid_cpy[next_row][next_col] == '0')
                    {
                        continue;
                    }

                    grid_cpy[next_row][next_col] = '0';
                    coords.emplace(next_row, next_col);
                }
            }
        }
    }

    return num_islands;

} // static int numIslandsFA( ...

//! @brief DFS discussion solution
//! @param[in] grid Reference to an M x N 2D grid of 1s (land) and 0s (water)
//! @return Number of islands
static int numIslandsDS1(const std::vector<std::vector<char>>& grid)
{
    //! @details https://leetcode.com/problems/number-of-islands/description
    //!
    //!          Time complexity O(M * N) where M = number of rows and
    //!          N = number of cols
    //!          Space complexity O(M * N) in the case that the grid map is
    //!          filled with lands where DFS is M x N deep. This also excludes
    //!          the use of grid_cpy.

    auto grid_cpy = grid;

    const auto num_rows = static_cast<int>(grid_cpy.size());
    if (num_rows == 0)
    {
        return 0;
    }
    const auto num_cols = static_cast<int>(grid_cpy[0].size());

    const std::vector<std::pair<int, int>> directions {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    const auto is_valid = [&](int row, int col) {
        return row >= 0 && row < num_rows  && col >= 0 && col < num_cols;
    };
    
    std::function<void(int, int)> dfs = [&](int row_in, int col_in) {
        grid_cpy[row_in][col_in] = '0';

        for (const auto& [drow, dcol] : directions)
        {
            const int next_row {row_in + drow};
            const int next_col {col_in + dcol};

            if (is_valid(next_row, next_col)
                && grid_cpy[next_row][next_col] == '1')
            {
                dfs(next_row, next_col);
            }
        }

        /*
         * Actual solution from editorial, except grid used instead of grid_cpy
         *
        grid_cpy[row_in][col_in] = '0';

        if (row_in - 1 >= 0 && grid_cpy[row_in - 1][col_in] == '1')
        {
            dfs(row_in - 1, col_in);
        }
        if (row_in + 1 < num_rows && grid_cpy[row_in + 1][col_in] == '1')
        {
            dfs(row_in + 1, col_in);
        }
        if (col_in - 1 >= 0 && grid_cpy[row_in][col_in - 1] == '1')
        {
            dfs(row_in, col_in - 1);
        }
        if (col_in + 1 < num_cols && grid_cpy[row_in][col_in + 1] == '1')
        {
            dfs(row_in, col_in + 1);
        }
         */
    };

    int num_islands {};

    for (int row = 0; row < num_rows; ++row)
    {
        for (int col = 0; col < num_cols; ++col)
        {
            if (grid_cpy[row][col] == '1')
            {
                ++num_islands;

                dfs(row, col);
            }
        }
    }

    return num_islands;

} // static int numIslandsDS1( ...

TEST(NumIslandsTest, SampleTest1)
{
    const std::vector<std::vector<char>> grid {
        {'1', '1', '1', '1', '0'},
        {'1', '1', '0', '1', '0'},
        {'1', '1', '0', '0', '0'},
        {'0', '0', '0', '0', '0'}};

    EXPECT_EQ(1, numIslandsFA(grid));
    EXPECT_EQ(1, numIslandsDS1(grid));
}

TEST(NumIslandsTest, SampleTest2)
{
    const std::vector<std::vector<char>> grid {
        {'1', '1', '0', '0', '0'},
        {'1', '1', '0', '0', '0'},
        {'0', '0', '1', '0', '0'},
        {'0', '0', '0', '1', '1'}};

    EXPECT_EQ(1, numIslandsFA(grid));
    EXPECT_EQ(1, numIslandsDS1(grid));
}

TEST(NumIslandsTest, SampleTest3)
{
    const std::vector<std::vector<char>> grid {
        {'1','1','1','1','1','0','1','1','1','1','1','1','1','1','1','0','1','0','1','1'},
        {'0','1','1','1','1','1','1','1','1','1','1','1','1','0','1','1','1','1','1','0'},
        {'1','0','1','1','1','0','0','1','1','0','1','1','1','1','1','1','1','1','1','1'},
        {'1','1','1','1','0','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
        {'1','0','0','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
        {'1','0','1','1','1','1','1','1','0','1','1','1','0','1','1','1','0','1','1','1'},
        {'0','1','1','1','1','1','1','1','1','1','1','1','0','1','1','0','1','1','1','1'},
        {'1','1','1','1','1','1','1','1','1','1','1','1','0','1','1','1','1','0','1','1'},
        {'1','1','1','1','1','1','1','1','1','1','0','1','1','1','1','1','1','1','1','1'},
        {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
        {'0','1','1','1','1','1','1','1','0','1','1','1','1','1','1','1','1','1','1','1'},
        {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
        {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
        {'1','1','1','1','1','0','1','1','1','1','1','1','1','0','1','1','1','1','1','1'},
        {'1','0','1','1','1','1','1','0','1','1','1','0','1','1','1','1','0','1','1','1'},
        {'1','1','1','1','1','1','1','1','1','1','1','1','0','1','1','1','1','1','1','0'},
        {'1','1','1','1','1','1','1','1','1','1','1','1','1','0','1','1','1','1','0','0'},
        {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
        {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
        {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'}};
}
