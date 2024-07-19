#include "TreeNode.hpp"

#include <gtest/gtest.h>

#include <queue>
#include <stack>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

//! @brief First attempt to create tree descriptions and return its root
//! @param[in] descriptions Vec descriptions_i = {parent_i, child_i, isLeft_i}
//! @return Root of binary tree described by descriptions
static TreeNode* createBinaryTreeFA(
    const std::vector<std::vector<int>>& descriptions)
{
    //! @details https://leetcode.com/problems
    //!          /create-binary-tree-from-descriptions
    //!
    //!          Time complexity O(N) where N = number of nodes in binary tree.
    //!          Need to iterate through descriptions to create tree and then
    //!          iterate through all nodes in tree.
    //!          Space complexity O(N) for node_map and children.

    std::unordered_map<int, TreeNode*> node_map {};
    std::unordered_set<int>            children {};

    for (const auto& desc : descriptions)
    {
        auto parent_ret = node_map.emplace(desc[0], new TreeNode(desc[0]));
        auto child_ret  = node_map.emplace(desc[1], new TreeNode(desc[1]));

        auto* parent_node = parent_ret.first->second;
        auto* child_node  = child_ret.first->second;

        std::ignore = children.emplace(desc[1]);

        if (desc[2] == 0)
        {
            parent_node->right = child_node;
        }
        else
        {
            parent_node->left = child_node;
        }
    }

    for (const auto& [val, node] : node_map)
    {
        if (!children.contains(val))
        {
            return node;
        }
    }

    return nullptr;

} // static TreeNode* createBinaryTreeFA( ...

//! @brief Convert to graph with breadth first search discussion solution
//! @param[in] descriptions Vec descriptions_i = {parent_i, child_i, isLeft_i}
//! @return Root of binary tree described by descriptions
static TreeNode* createBinaryTreeDS1(
    const std::vector<std::vector<int>>& descriptions)
{
    //! @details https://leetcode.com/problems
    //!          /create-binary-tree-from-descriptions
    //!
    //!          Time complexity O(N) where N = number of entries in description
    //!          Building parent_to_children, children, and parents takes O(N).
    //!          Finding root node involves iterating through parents, which is
    //!          O(N) in the worst case. Constructing the binary tree using BFS
    //!          takes O(N) since each node is processed once.
    //!          Space complexity O(N). parent_to_children map can store up to
    //!          N entries. The children and parents sets can store up to N
    //!          elements. BFS queue can store up to N nodes in the worst case.

    //! Sets to track unique children and parents
    //! @note Technically parents stores all nodes in the tree
    std::unordered_set<int> children {};
    std::unordered_set<int> parents {};

    //! Map to store parent to children relationships
    //! <parent, vector of <child, is_left>>
    std::unordered_map<int,
                       std::vector<std::pair<int, int>>> parent_to_children {};

    //! Build graph from parent to child and add nodes to sets
    for (const auto& description : descriptions)
    {
        const int parent {description[0]};
        const int child {description[1]};
        const int is_left {description[2]};

        parents.insert(parent);
        parents.insert(child);
        children.insert(child);
        parent_to_children[parent].emplace_back(child, is_left);
    }

    //! Find root node - search for node in parents that is not in children
    //! @note Could also use std::set_difference with std::set
    auto it = parents.begin();
    while (it != parents.end())
    {
        if (children.find(*it) != children.end())
        {
            it = parents.erase(it);
            continue;
        }

        ++it;
    }

    TreeNode* root = new TreeNode(*parents.begin());

    //! Use BFS to construct binary tree from root
    std::queue<TreeNode*> node_queue;
    node_queue.push(root);

    while (!node_queue.empty())
    {
        auto* parent = node_queue.front();
        node_queue.pop();

        //! Iterate over children of current parent
        for (const auto& child_info : parent_to_children[parent->val])
        {
            const int child_value {child_info.first};
            const int is_left {child_info.second};

            TreeNode* child = new TreeNode(child_value);
            node_queue.push(child);

            //! Attach child node to parent based on is_left
            if (is_left == 1)
            {
                parent->left = child;
            }
            else
            {
                parent->right = child;
            }
        }
    }

    return root;

} // static TreeNode* createBinaryTreeDS1( ...

TEST(CreateBinaryTreeTest, SampleTest1)
{
    const std::vector<std::vector<int>> descriptions {
        {20, 15, 1}, {20, 17, 0}, {50, 20, 1}, {50, 80, 0}, {80, 19, 1}};

    const TreeNode nineteen {19};
    const TreeNode eighty {80, &nineteen, nullptr};

    const TreeNode seventeen {17};
    const TreeNode fifteen {15};
    const TreeNode twenty {20, &fifteen, &seventeen};

    const TreeNode fifty {50, &twenty, &eighty};

    auto* root_fa = createBinaryTreeFA(descriptions);

    std::stack<TreeNode*> output_nodes {root_fa};
    std::stack<TreeNode*> expected_nodes {&fifty};

    while (!output_nodes.empty() && !expected_nodes.empty())
    {
        auto* output_node = output_nodes.top();
        output_nodes.pop();

        auto* expected_node = expected_nodes.top();
        expected_nodes.pop();

        if (output_node->left == nullptr
            && output_node->right == nullptr
            && expected_node->left == nullptr
            && expected_node->right == nullptr)
        {
            EXPECT_EQ(output_node->val, expected_node->val);
            delete output_node;
            output_node = nullptr;
            continue;
        }

        if (output_node->left != nullptr && expected_node->left != nullptr)
        {
            expected_nodes.push(expected_node->left);
            output_nodes.push(output_node->left);
        }

        if (output_node->right != nullptr && expected_node->right != nullptr)
        {
            expected_nodes.push(expected_node->right);
            output_nodes.push(output_node->right);
        }
    }
}
