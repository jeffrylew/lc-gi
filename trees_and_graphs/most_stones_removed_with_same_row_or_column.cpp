#include <gtest/gtest.h>

#include <functional>
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

//! @brief Depth First Search discussion solution
//! @param[in] stones Reference to vector of locations of N stones
//! @return Largest possible number of stones that can be removed
static int removeStonesDS1(const std::vector<std::vector<int>>& stones)
{
    //! @details https://leetcode.com/problems
    //!          /most-stones-removed-with-same-row-or-column/editorial/
    //!
    //!          Time complexity O(N ^ 2) where N = stones.size(). The adjacency
    //!          list is built by iterating over all pairs of stones to check if
    //!          they share the same row or column. In the worst case, the DFS
    //!          will tranverse all nodes and edges. Since each stone can be
    //!          connected to every other stone, the algorithm can visit all
    //!          O(N ^ 2) edges across all DFS calls.
    //!          Space complexity O(N ^ 2). In the worst case, any two stones
    //!          could share the same row or column and adjacency_list could
    //!          store up to N ^ 2 edges. The visited_stone_ids vector takes an
    //!          additional linear space. The recursive DFS call stack can go as
    //!          deep as the number of connected stones in a single group. In
    //!          the worst case, this depth could be N, leading to O(N) space.

    const auto num_stones = static_cast<int>(std::ssize(stones));

    //! Adjacency list to store graph connections
    std::vector<std::vector<int>> adjacency_list(num_stones);

    //! Build the graph: Connect stones that share the same row or column
    for (int stone_id1 = 0; stone_id1 < num_stones; ++stone_id1)
    {
        for (int stone_id2 = stone_id1 + 1; stone_id2 < num_stones; ++stone_id2)
        {
            if (stones[stone_id1][0] == stones[stone_id2][0]
                || stones[stone_id1][1] == stones[stone_id2][1])
            {
                adjacency_list[stone_id1].push_back(stone_id2);
                adjacency_list[stone_id2].push_back(stone_id1);
            }
        }
    }

    int               num_groups_of_connected_stones {};
    std::vector<bool> visited_stone_ids(num_stones);

    //! DFS to visit all stones in a connected component
    const std::function<void(int)> visit_connected_stones = [&](int stone_id) {
        visited_stone_ids[stone_id] = true;

        for (const int neighbor_stone_id : adjacency_list[stone_id])
        {
            if (!visited_stone_ids[neighbor_stone_id])
            {
                visit_connected_stones(neighbor_stone_id);
            }
        }
    };

    //! Traverse all stones using DFS to count groups of connected stones
    for (int stone_id = 0; stone_id < num_stones; ++stone_id)
    {
        if (visited_stone_ids[stone_id])
        {
            continue;
        }

        visit_connected_stones(stone_id);
        ++num_groups_of_connected_stones;
    }

    //! Max stones that can be removed is total stones minus number of groups of
    //! connected stones
    return num_stones - num_groups_of_connected_stones;

} // static int removeStonesDS1( ...

TEST(RemoveStonesTest, SampleTest1)
{
    const std::vector<std::vector<int>> stones {
        {0, 0}, {0, 1}, {1, 0}, {1, 2}, {2, 1}, {2, 2}};

    EXPECT_EQ(5, removeStonesFA(stones));
    EXPECT_EQ(5, removeStonesDS1(stones));
}

TEST(RemoveStonesTest, SampleTest2)
{
    const std::vector<std::vector<int>> stones {
        {0, 0}, {0, 2}, {1, 1}, {2, 0}, {2, 2}};

    EXPECT_EQ(3, removeStonesFA(stones));
    EXPECT_EQ(3, removeStonesDS1(stones));
}

TEST(RemoveStonesTest, SampleTest3)
{
    const std::vector<std::vector<int>> stones {{0, 0}};

    EXPECT_EQ(0, removeStonesFA(stones));
    EXPECT_EQ(0, removeStonesDS1(stones));
}

TEST(RemoveStonesTest, SampleTest4)
{
    const std::vector<std::vector<int>> stones {
        {1, 2}, {1, 3}, {3, 3}, {3, 1}, {2, 1}, {1, 0}};

    EXPECT_NE(5, removeStonesFA(stones));
    EXPECT_EQ(4, removeStonesFA(stones));
    EXPECT_EQ(5, removeStonesDS1(stones));
}
