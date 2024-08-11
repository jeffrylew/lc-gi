#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <map>
#include <stack>
#include <tuple>
#include <utility>
#include <vector>

//! @brief First attempt to get length of longest increasing path in matrix
//! @param[in] matrix Reference to 2D vector of m x n ints
//! @return Length of longest increasing path in matrix
static int longestIncreasingPathFA(const std::vector<std::vector<int>>& matrix)
{
    //! @details leetcode.com/problems/longest-increasing-path-in-a-matrix
    //!
    //!          First attempt solution passes 63 / 139 test cases.
    //!          Does not pass SampleTest4

    const auto nrows = static_cast<int>(std::ssize(matrix));
    const auto ncols = static_cast<int>(std::ssize(matrix[0]));

    std::map<int, std::vector<std::pair<int, int>>> val_pos_map {};
    for (int row = 0; row < nrows; ++row)
    {
        for (int col = 0; col < ncols; ++col)
        {
            val_pos_map[matrix[row][col]].emplace_back(row, col);
        }
    }

    const auto is_valid = [&](int row, int col) {
        return row >= 0 && row < nrows && col >= 0 && col < ncols;
    };

    const std::vector<std::pair<int, int>> directions {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    std::stack<std::tuple<int, int, int>> neighbors {};

    int longest_path_len {};

    for (const auto& [val, pos_vec] : val_pos_map)
    {
        for (const auto& [start_row, start_col] : pos_vec)
        {
            std::vector<std::vector<bool>> seen(
                nrows, std::vector<bool>(ncols, false));

            neighbors.emplace(start_row, start_col, 1);
            seen[start_row][start_col] = true;

            while (!neighbors.empty())
            {
                const auto [curr_row, curr_col, path_len] = neighbors.top();
                neighbors.pop();

                longest_path_len = std::max(longest_path_len, path_len);

                for (const auto& [drow, dcol] : directions)
                {
                    const auto next_row = curr_row + drow;
                    const auto next_col = curr_col + dcol;

                    if (!is_valid(next_row, next_col))
                    {
                        continue;
                    }

                    if (seen[next_row][next_col])
                    {
                        continue;
                    }
                    seen[next_row][next_col] = true;

                    if (matrix[next_row][next_col]
                        <= matrix[curr_row][curr_col])
                    {
                        continue;
                    }

                    neighbors.emplace(next_row, next_col, path_len + 1);
                }
            }
        }
    }

    return longest_path_len;

} // static int longestIncreasingPathFA( ...

//! @brief Naive DFS discussion solution
//! @param[in] matrix Reference to 2D vector of m x n ints
//! @return Length of longest increasing path in matrix
static int longestIncreasingPathDS1(const std::vector<std::vector<int>>& matrix)
{
    //! @details leetcode.com/problems/longest-increasing-path-in-a-matrix
    //!
    //!          Time complexity O(2 ^ (M + N)) where M and N are dimensions of
    //!          matrix. The search is repeated for each valid increasing path.
    //!          In the worst case, there is a path that spans every cell in the
    //!          matrix. Each cell will have two increasing neighbor cells
    //!          except for the last and corner cells and multiplies the number
    //!          of possible paths by 2.
    //!          Space complexity O(M * N). For each DFS, we need O(H) space for
    //!          the system stack where H is the max depth of the recursion. In
    //!          the worst case, O(H) = O(M * N).

    if (matrix.empty())
    {
        return 0;
    }

    const auto nrows = static_cast<int>(std::ssize(matrix));
    const auto ncols = static_cast<int>(std::ssize(matrix[0]));

    const std::vector<std::pair<int, int>> dirs {
        {0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    std::function<int(int, int)> dfs = [&](int row_in, int col_in) {
        int longest_path {};

        for (const auto& [drow, dcol] : dirs)
        {
            const int next_row {row_in + drow};
            const int next_col {col_in + dcol};

            if (0 <= next_row
                && next_row < nrows
                && 0 <= next_col
                && next_col < ncols
                && matrix[next_row][next_col] > matrix[row_in][col_in])
            {
                longest_path = std::max(longest_path, dfs(next_row, next_col));
            }
        }

        return ++longest_path;
    };

    int longest_increasing_path {};
    for (int row = 0; row < nrows; ++row)
    {
        for (int col = 0; col < ncols; ++col)
        {
            longest_increasing_path = std::max(longest_increasing_path,
                                               dfs(row, col));
        }
    }

    return longest_increasing_path;

} // static int longestIncreasingPathDS1( ...

TEST(LongestIncreasingPathTest, SampleTest1)
{
    const std::vector<std::vector<int>> matrix {
        {9, 9, 4}, {6, 6, 8}, {2, 1, 1}};

    EXPECT_EQ(4, longestIncreasingPathFA(matrix));
    EXPECT_EQ(4, longestIncreasingPathDS1(matrix));
}

TEST(LongestIncreasingPathTest, SampleTest2)
{
    const std::vector<std::vector<int>> matrix {
        {3, 4, 5}, {3, 2, 6}, {2, 2, 1}};

    EXPECT_EQ(4, longestIncreasingPathFA(matrix));
    EXPECT_EQ(4, longestIncreasingPathDS1(matrix));
}

TEST(LongestIncreasingPathTest, SampleTest3)
{
    const std::vector<std::vector<int>> matrix {{1}};

    EXPECT_EQ(1, longestIncreasingPathFA(matrix));
    EXPECT_EQ(1, longestIncreasingPathDS1(matrix));
}

TEST(LongestIncreasingPathTest, SampleTest4)
{
    const std::vector<std::vector<int>> matrix {
        {7, 8, 9}, {9, 7, 6}, {7, 2, 3}};

    EXPECT_EQ(4, longestIncreasingPathFA(matrix));
    EXPECT_NE(6, longestIncreasingPathFA(matrix));
    EXPECT_EQ(6, longestIncreasingPathDS1(matrix));
}
