#include "NaryNode.hpp"

#include <gtest/gtest.h>

#include <queue>
#include <utility>
#include <vector>

//! @brief First attempt to get the level order traversal of an n-ary tree
//! @param[in] root Pointer to an NaryNode that is the root of an n-ary tree
//! @return Vector of node values per level
static std::vector<std::vector<int>> levelOrderFA(NaryNode* root)
{
    //! @details leetcode.com/explore/learn/card/n-ary-tree/130/traversal/915
    //!
    //!          Time complexity O(N) where N = number of nodes in n-ary tree.
    //!          Space complexity O(N) in the worst case where the root has N-1
    //!          children.

    std::vector<std::vector<int>> node_values;
    std::queue<NaryNode*>         nary_nodes;

    if (root == nullptr)
    {
        return node_values;
    }

    nary_nodes.push(root);
    while (!nary_nodes.empty())
    {
        const auto num_nodes = static_cast<int>(std::ssize(nary_nodes));

        std::vector<int> curr_level_node_values(num_nodes);

        for (int node_idx = 0; node_idx < num_nodes; ++node_idx)
        {
            const auto node_ptr = nary_nodes.front();
            nary_nodes.pop();

            curr_level_node_values[node_idx] = node_ptr->val;

            for (const auto& child_ptr : node_ptr->children)
            {
                nary_nodes.push(child_ptr);
            }
        }
        
        node_values.push_back(std::move(curr_level_node_values));
    }

    return node_values;

} // static std::vector<std::vector<int>> levelOrderFA( ...

//! @brief BFS using a queue discussion solution
//! @param[in] root Pointer to an NaryNode that is the root of an n-ary tree
//! @return Vector of node values per level
static std::vector<std::vector<int>> levelOrderDS1(NaryNode* root)
{
    //! @details https://leetcode.com/problems/n-ary-tree-level-order-traversal
    //!
    //!          Time complexity O(N) where N = number of nodes. Each node is
    //!          added to the queue, removed from the queue, and added to the
    //!          node_values output once.
    //!          Space complexity O(N). The nary_nodes queue will have 2 layers
    //!          of the tree on it at any given time. In the worst case, this is
    //!          all of the nodes. In the best case, it is just 1 node when the
    //!          tree is a singly linked list. For balanced trees, half or more
    //!          of the nodes are often in the lowest 2 layers in the average
    //!          case. Thus, we go with the worst case of O(N) since the average
    //!          case is probably similar.

    std::vector<std::vector<int>> node_values;
    if (root == nullptr)
    {
        return node_values;
    }

    std::queue<NaryNode*> nary_nodes;
    nary_nodes.push(root);

    while (!nary_nodes.empty())
    {
        const auto num_nodes = static_cast<int>(std::ssize(nary_nodes));

        std::vector<int> curr_level_node_values(num_nodes);

        for (int node_idx = 0; node_idx < num_nodes; ++node_idx)
        {
            const auto node_ptr = nary_nodes.front();
            nary_nodes.pop();

            curr_level_node_values[node_idx] = node_ptr->val;

            for (const auto& child_ptr : node_ptr->children)
            {
                nary_nodes.push(child_ptr);
            }
        }

        node_values.push_back(std::move(curr_level_node_values));
    }

    return node_values;
}

//! @brief Simplified BFS discussion solution
//! @param[in] root Pointer to an NaryNode that is the root of an n-ary tree
//! @return Vector of node values per level
static std::vector<std::vector<int>> levelOrderDS2(NaryNode* root)
{
    //! @details https://leetcode.com/problems/n-ary-tree-level-order-traversal
    //!
    //!          Time complexity O(N) where N = number of nodes.
    //!          Space complexity O(N) since we always have vectors containing
    //!          levels of nodes.

    if (root == nullptr)
    {
        return {};
    }

    std::vector<std::vector<int>> node_values;
    std::vector<NaryNode*>        prev_layer {root};

    while (!prev_layer.empty())
    {
        std::vector<NaryNode*> curr_layer;
        auto& prev_vals = node_values.emplace_back();

        for (const auto& node : prev_layer)
        {
            prev_vals.push_back(node->val);

            for (const auto& child : node->children)
            {
                curr_layer.push_back(child);
            }
        }

        prev_layer = std::move(curr_layer);
    }

    return node_values;

} // static std::vector<std::vector<int>> levelOrderDS2( ...

TEST(NaryLevelOrderTest, SampleTest1)
{
    constexpr NaryNode two {2};
    constexpr NaryNode four {4};
    constexpr NaryNode five {5};
    constexpr NaryNode six {6};

    const NaryNode three {3, std::vector<NaryNode*> {&five, &six}};
    const NaryNode one {1, std::vector<NaryNode*> {&three, &two, &four}};

    const std::vector<std::vector<int>> expected_output {
        {1}, {3, 2, 4}, {5, 6}};

    EXPECT_EQ(expected_output, levelOrderFA(&one));
    EXPECT_EQ(expected_output, levelOrderDS1(&one));
    EXPECT_EQ(expected_output, levelOrderDS2(&one));
}

TEST(NaryLevelOrderTest, SampleTest2)
{
    constexpr NaryNode two {2};
    constexpr NaryNode six {6};
    constexpr NaryNode ten {10};
    constexpr NaryNode twelve {12};
    constexpr NaryNode thirteen {13};
    constexpr NaryNode fourteen {14};

    const NaryNode eight {8, std::vector<NaryNode*> {&twelve}};
    const NaryNode nine {9, std::vector<NaryNode*> {&thirteen}};
    const NaryNode eleven {11, std::vector<NaryNode*> {&fourteen}};

    const NaryNode four {4, std::vector<NaryNode*> {&eight}};
    const NaryNode seven {7, std::vector<NaryNode*> {&eleven}};

    const NaryNode three {3, std::vector<NaryNode*> {&six, &seven}};
    const NaryNode five {5, std::vector<NaryNode*> {&nine, &ten}};

    const NaryNode one {1, std::vector<NaryNode*> {&two, &three, &four, &five}};

    const std::vector<int> expected_output {
        {1}, {2, 3, 4, 5}, {6, 7, 8, 9, 10}, {11, 12, 13}, {14}};

    EXPECT_EQ(expected_output, levelOrderFA(&one));
    EXPECT_EQ(expected_output, levelOrderDS1(&one));
    EXPECT_EQ(expected_output, levelOrderDS2(&one));
}
