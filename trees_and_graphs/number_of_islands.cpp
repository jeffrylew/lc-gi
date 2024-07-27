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

//! @brief BFS discussion solution
//! @param[in] grid Reference to an M x N 2D grid of 1s (land) and 0s (water)
//! @return Number of islands
static int numIslandsDS2(const std::vector<std::vector<char>>& grid)
{
    //! @details https://leetcode.com/problems/number-of-islands/description
    //!
    //!          Time complexity O(M * N) where M = number of rows and
    //!          N = number of cols
    //!          Space complexity O(min(M, N)) because in worst case where grid
    //!          is filled with land, the size of queue can grow up to min(M, N)

    auto grid_cpy = grid;

    const auto num_rows = static_cast<int>(grid_cpy.size());
    if (num_rows == 0)
    {
        return 0;
    }
    const auto num_cols = static_cast<int>(grid_cpy[0].size());

    int num_islands {};

    for (int row = 0; row < num_rows; ++row)
    {
        for (int col = 0; col < num_cols; ++col)
        {
            if (grid[row][col] == '0')
            {
                continue;
            }

            ++num_islands;

            //! Mark as visited
            grid_cpy[row][col] = '0';

            std::queue<std::pair<int, int>> neighbors {};
            neighbors.emplace(row, col);

            while (!neighbors.empty())
            {
                const auto [next_row, next_col] = neighbors.front();
                neighbors.pop();

                if (next_row - 1 >= 0
                    && grid_cpy[next_row - 1][next_col] == '1')
                {
                    neighbors.emplace(next_row - 1, next_col);
                    grid_cpy[next_row - 1][next_col] = '0';
                }

                if (next_row + 1 < num_rows
                    && grid_cpy[next_row + 1][next_col] == '1')
                {
                    neighbors.emplace(next_row + 1, next_col);
                    grid_cpy[next_row + 1][next_col] = '0';
                }

                if (next_col - 1 >= 0
                    && grid_cpy[next_row][next_col - 1] == '1')
                {
                    neighbors.emplace(next_row, next_col - 1);
                    grid_cpy[next_row][next_col - 1] = '0';
                }

                if (next_col + 1 < num_cols
                    && grid_cpy[next_row][next_col + 1] == '1')
                {
                    neighbors.emplace(next_row, next_col + 1);
                    grid_cpy[next_row][next_col + 1] = '0';
                }
            }
        }
    }

    return num_islands;

} // static int numIslandsDS2( ...

class Union_find
{
public:
    explicit Union_find(const std::vector<std::vector<char>>& grid)
    {
        const auto num_rows = static_cast<int>(grid.size());
        const auto num_cols = static_cast<int>(grid[0].size());
        parent.reserve(num_rows * num_cols);

        for (int row = 0; row < num_rows; ++row)
        {
            for (int col = 0; col < num_cols; ++col)
            {
                if (grid[row][col] == '1')
                {
                    parent.push_back(row * num_cols + col);
                    ++count;
                }
                else
                {
                    parent.push_back(-1);
                }
                rank.push_back(0);
            }
        }

    } // explicit Union_find( ...

    //! Path compression
    int find(int idx)
    {
        if (parent[idx] != idx)
        {
            parent[idx] = find(parent[idx]);
        }
        return parent[idx];
    }

    int get_count() const
    {
        return count;
    }

    //! Union with rank
    void union_rank(int x, int y)
    {
        const int rootx {find(x)};
        const int rooty {find(y)};

        if (rootx != rooty)
        {
            if (rank[rootx] > rank[rooty])
            {
                parent[rooty] = rootx;
            }
            else if (rank[rootx] < rank[rooty])
            {
                parent[rootx] = rooty;
            }
            else
            {
                parent[rooty] = rootx;
                ++rank[rootx];
            }

            --count;
        }
    }

private:
    //! Number of connected components
    int count {};

    std::vector<int> parent {};
    std::vector<int> rank {};
    
}; // class Union_find

//! @brief Union find discussion solution
//! @param[in] grid Reference to an M x N 2D grid of 1s (land) and 0s (water)
//! @return Number of islands
static int numIslandsDS3(const std::vector<std::vector<char>>& grid)
{
    //! @details https://leetcode.com/problems/number-of-islands/description
    //!
    //!          Time complexity O(M * N) where M = number of rows and
    //!          N = number of cols. The union_rank operation takes constant
    //!          time when Union_find is implemented with both path compression
    //!          and union by rank.
    //!          Space complexity O(M * N) as required by Union_find structure.

    auto grid_cpy = grid;

    const auto num_rows = static_cast<int>(grid_cpy.size());
    if (num_rows == 0)
    {
        return 0;
    }
    const auto num_cols = static_cast<int>(grid_cpy[0].size());

    Union_find uf {grid_cpy};

    for (int row = 0; row < num_rows; ++row)
    {
        for (int col = 0; col < num_cols; ++col)
        {
            if (grid_cpy[row][col] == '0')
            {
                continue;
            }

            grid_cpy[row][col] = '0';

            if (row - 1 >= 0 && grid_cpy[row - 1][col] == '1')
            {
                uf.union_rank(row * num_cols + col, (row - 1) * num_cols + col);
            }

            if (row + 1 < num_rows && grid_cpy[row + 1][col] == '1')
            {
                uf.union_rank(row * num_cols + col, (row + 1) * num_cols + col);
            }

            if (col - 1 >= 0 && grid_cpy[row][col - 1] == '1')
            {
                uf.union_rank(row * num_cols + col, row * num_cols + (col - 1));
            }

            if (col + 1 < num_cols && grid_cpy[row][col + 1] == '1')
            {
                uf.union_rank(row * num_cols + col, row * num_cols + (col + 1));
            }
        }
    }

    return uf.get_count();

} // static int numIslandsDS3( ...

TEST(NumIslandsTest, SampleTest1)
{
    const std::vector<std::vector<char>> grid {
        {'1', '1', '1', '1', '0'},
        {'1', '1', '0', '1', '0'},
        {'1', '1', '0', '0', '0'},
        {'0', '0', '0', '0', '0'}};

    EXPECT_EQ(1, numIslandsFA(grid));
    EXPECT_EQ(1, numIslandsDS1(grid));
    EXPECT_EQ(1, numIslandsDS2(grid));
    EXPECT_EQ(1, numIslandsDS3(grid));
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
    EXPECT_EQ(1, numIslandsDS2(grid));
    EXPECT_EQ(1, numIslandsDS3(grid));
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
