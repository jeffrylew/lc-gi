#include <gtest/gtest.h>

#include <functional>
#include <map>
#include <set>
#include <stack>
#include <unordered_set>
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

//! Union-Find data structure for tracking groups of connected stones
class Union_find_DS2
{
public:
    //! Initially, each stone is its own connected component/group
    explicit Union_find_DS2(int num_stones)
        : num_groups_of_connected_stones {num_stones}
        , stone_parents(num_stones, -1)
    {
    }

    [[nodiscard]] constexpr int get_connected_components() const noexcept
    {
        return num_groups_of_connected_stones;
    }

    //! Find the root of a stone with path compression
    [[nodiscard]] constexpr int find_root(int stone_id) noexcept
    {
        if (stone_parents[stone_id] == -1)
        {
            return stone_id;
        }

        return stone_parents[stone_id] = find_root(stone_parents[stone_id]);
    }

    //! Union two stones, reducing the number of groups of connected stones
    constexpr void union_stones(int stone_id1, int stone_id2) noexcept
    {
        const int root_stone_id1 {find_root(stone_id1)};
        const int root_stone_id2 {find_root(stone_id2)};

        //! If stones are already in the same group/component, do nothing
        if (root_stone_id1 == root_stone_id2)
        {
            return;
        }

        //! Merge the groups/components and reduce the count of groups of
        //! connected stones
        stone_parents[root_stone_id1] = root_stone_id2;
        --num_groups_of_connected_stones;
    }

private:
    //! Number of groups of connected stones
    int num_groups_of_connected_stones {};

    //! Vector tracks the parent of each stone
    std::vector<int> stone_parents {};

}; // class Union_find_DS2

//! @brief Disjoint Set Union discussion solution
//! @param[in] stones Reference to vector of locations of N stones
//! @return Largest possible number of stones that can be removed
static int removeStonesDS2(const std::vector<std::vector<int>>& stones)
{
    //! @details https://leetcode.com/problems
    //!          /most-stones-removed-with-same-row-or-column/editorial/
    //!
    //!          Time complexity O(N ^ 2 * alpha(N)) where N = stones.size() and
    //!          alpha(N) is the inverse Ackermann function. Initializing vector
    //!          stone_parents with -1 takes O(N). The nested loops iterate
    //!          through each pair of stones. The number of pairs is N(N - 1)/2,
    //!          which is O(N ^ 2). For each pair, the union_stones operation is
    //!          performed if the stones share the same row or column. The
    //!          union_stones operation and subsequent find_root operation takes
    //!          O(alpha(N)).
    //!          Space complexity O(N) for the stone_parents vector.

    const auto     num_stones = static_cast<int>(std::ssize(stones));
    Union_find_DS2 union_find {num_stones};

    //! Connect stones that share the same row or column
    for (int stone_id1 = 0; stone_id1 < num_stones; ++stone_id1)
    {
        for (int stone_id2 = stone_id1 + 1; stone_id2 < num_stones; ++stone_id2)
        {
            if (stones[stone_id1][0] == stones[stone_id2][0]
                || stones[stone_id1][1] == stones[stone_id2][1])
            {
                union_find.union_stones(stone_id1, stone_id2);
            }
        }
    }

    return num_stones - union_find.get_connected_components();

} // static int removeStonesDS2( ...

//! Union-Find data structure for tracking connected components
class Union_find_DS3
{
public:
    //! Initialize all nodes as their own parent
    explicit Union_find_DS3(int num_nodes)
        : parent(num_nodes, -1)
    {
    }

    //! Find the root of a node with path compression
    [[nodiscard]] int find_root(int node)
    {
        //! If the node has not been tracked yet, increase the component count
        if (!unique_nodes.contains(node))
        {
            ++component_count;
            unique_nodes.insert(node);
        }

        if (parent[node] == -1)
        {
            return node;
        }

        return parent[node] = find_root(parent[node]);
    }

    voi union_nodes(int node1, int node2)
    {
        const int root1 {find_root(node1)};
        const int root2 {find_root(node2)};

        //! If nodes are already in the same group/component, do nothing
        if (root1 == root2)
        {
            return;
        }

        //! Merge the components and reduce the component count
        parent[root1] = root2;
        --component_count;
    }

private:
    //! Number of connected components
    int component_count {};

    //! Vector tracks the parent of each node
    std::vector<int> parent;

    //! Set to track unique nodes
    std::unordered_set<int> unique_nodes;

}; // class Union_find_DS3

//! @brief Optimized Disjoint Set Union discussion solution
//! @param[in] stones Reference to vector of locations of N stones
//! @return Largest possible number of stones that can be removed
static int removeStonesDS3(const std::vector<std::vector<int>>& stones)
{
    //! @details https://leetcode.com/problems
    //!          /most-stones-removed-with-same-row-or-column/editorial/
    //!
    //!          Time complexity O(N) where N = stones.size(). The size of the
    //!          parent vector is constant (20002) so initialization takes O(1).
    //!          The union_nodes operation is called N times, once for each
    //!          stone. All union_nodes and find_root operations take
    //!          O(alpha(20002)) = O(1) where alpha is the inverse Ackermann
    //!          function.
    //!          Space complexity O(N + 20002). The parent vector takes constant
    //!          space. The unique_nodes set can have at most 2 * N elements,
    //!          corresponding to all unique x and y coordinates.

    const auto num_stones = static_cast<int>(std::ssize(stones));

    //! Initialize Union_find_DS3 with large enough range to handle coordinates
    Union_find_DS3 union_find {20002};

    //! Union stones that share the same row or column
    for (const auto& stone : stones)
    {
        //! Offset y-coordinates to avoid conflict with x-coordinates
        union_find.union_nodes(stone[0], stone[1] + 10001);
    }

    return num_stones - union_find.component_count;

} // static int removeStonesDS3( ...

TEST(RemoveStonesTest, SampleTest1)
{
    const std::vector<std::vector<int>> stones {
        {0, 0}, {0, 1}, {1, 0}, {1, 2}, {2, 1}, {2, 2}};

    EXPECT_EQ(5, removeStonesFA(stones));
    EXPECT_EQ(5, removeStonesDS1(stones));
    EXPECT_EQ(5, removeStonesDS2(stones));
    EXPECT_EQ(5, removeStonesDS3(stones));
}

TEST(RemoveStonesTest, SampleTest2)
{
    const std::vector<std::vector<int>> stones {
        {0, 0}, {0, 2}, {1, 1}, {2, 0}, {2, 2}};

    EXPECT_EQ(3, removeStonesFA(stones));
    EXPECT_EQ(3, removeStonesDS1(stones));
    EXPECT_EQ(3, removeStonesDS2(stones));
    EXPECT_EQ(3, removeStonesDS3(stones));
}

TEST(RemoveStonesTest, SampleTest3)
{
    const std::vector<std::vector<int>> stones {{0, 0}};

    EXPECT_EQ(0, removeStonesFA(stones));
    EXPECT_EQ(0, removeStonesDS1(stones));
    EXPECT_EQ(0, removeStonesDS2(stones));
    EXPECT_EQ(0, removeStonesDS3(stones));
}

TEST(RemoveStonesTest, SampleTest4)
{
    const std::vector<std::vector<int>> stones {
        {1, 2}, {1, 3}, {3, 3}, {3, 1}, {2, 1}, {1, 0}};

    EXPECT_NE(5, removeStonesFA(stones));
    EXPECT_EQ(4, removeStonesFA(stones));
    EXPECT_EQ(5, removeStonesDS1(stones));
    EXPECT_EQ(5, removeStonesDS2(stones));
    EXPECT_EQ(5, removeStonesDS3(stones));
}
