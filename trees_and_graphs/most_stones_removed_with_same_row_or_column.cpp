#include <gtest/gtest.h>

#include <map>
#include <set>
#include <stack>
#include <utility>
#include <vector>

//! @brief First attempt to get largest number of stones that can be removed
//! @param[in] stones Reference to vector of locations of N stones
//! @return Largest possible number of stones that can be removed
static int removeStonesFA(const std::vector<std::vector<int>>& stones)
{
    //! @details https://leetcode.com/explore/interview/card/google/61
    //!          /trees-and-graphs/3076/
    //!
    //!          First attempt solution does not pass SampleTest4

    using      row_col    = std::pair<int, int>;
    const auto num_stones = static_cast<int>(std::ssize(stones));

    std::map<row_col, std::vector<row_col>> connected_stone_locs {};

    for (int idx1 = 0; idx1 < num_stones; ++idx1)
    {
        const int row1 {stones[idx1].front()};
        const int col1 {stones[idx1].back()};

        for (int idx2 = idx1 + 1; idx2 < num_stones; ++idx2)
        {
            const int row2 {stones[idx2].front()};
            const int col2 {stones[idx2].back()};

            if (row1 == row2 || col1 == col2)
            {
                connected_stone_locs[{row1, col1}].emplace_back(row2, col2);
                connected_stone_locs[{row2, col2}].emplace_back(row1, col1);
            }
        }
    }

    int                                 max_connected_stones {};
    std::set<row_col>                   removed_stone_locs {};
    std::stack<std::pair<row_col, int>> loc_connections {};

    const auto get_num_connected_stones = [&](int start_row, int start_col) {
        if (!removed_stone_locs.emplace(start_row, start_col).second)
        {
            return;
        }
        loc_connections.emplace(std::make_pair(start_row, start_col), 0);

        while (!loc_connections.empty())
        {
            const auto& [curr_loc, num_connections] = loc_connections.top();
            loc_connections.pop();

            max_connected_stones =
                std::max(max_connected_stones, num_connections);

            for (const auto& neighbor_loc : connected_stone_locs[curr_loc])
            {
                if (removed_stone_locs.contains(neighbor_loc))
                {
                    continue;
                }

                removed_stone_locs.insert(neighbor_loc);
                loc_connections.emplace(neighbor_loc, num_connections + 1);
            }
        }
    };

    for (const auto& stone_loc : stones)
    {
        get_num_connected_stones(stone_loc[0], stone_loc[1]);
    }

    return max_connected_stones;

} // static int removeStonesFA( ...

TEST(RemoveStonesTest, SampleTest1)
{
    const std::vector<std::vector<int>> stones {
        {0, 0}, {0, 1}, {1, 0}, {1, 2}, {2, 1}, {2, 2}};

    EXPECT_EQ(5, removeStonesFA(stones));
}

TEST(RemoveStonesTest, SampleTest2)
{
    const std::vector<std::vector<int>> stones {
        {0, 0}, {0, 2}, {1, 1}, {2, 0}, {2, 2}};

    EXPECT_EQ(3, removeStonesFA(stones));
}

TEST(RemoveStonesTest, SampleTest3)
{
    const std::vector<std::vector<int>> stones {{0, 0}};

    EXPECT_EQ(0, removeStonesFA(stones));
}

TEST(RemoveStonesTest, SampleTest4)
{
    const std::vector<std::vector<int>> stones {
        {1, 2}, {1, 3}, {3, 3}, {3, 1}, {2, 1}, {1, 0}};

    EXPECT_NE(5, removeStonesFA(stones));
    EXPECT_EQ(4, removeStonesFA(stones));
}
