#include "TreeNode.hpp"

#include <gtest/gtest.h>

#include <functional>
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
    //!          Time complexity O(N) where N = num entries in descriptions.
    //!          Building parent_to_children, children, and all_nodes takes O(N)
    //!          Finding root node involves iterating through all_nodes, which
    //!          is O(N) in the worst case. Constructing the binary tree using
    //!          BFS takes O(N) since each node is processed once.
    //!          Space complexity O(N). parent_to_children map can store up to
    //!          N entries. The children and all_nodes sets can store up to N
    //!          elements. BFS queue can store up to N nodes in the worst case.

    //! Sets to track unique children and all nodes in the tree
    std::unordered_set<int> all_nodes {};
    std::unordered_set<int> children {};

    //! Map to store parent to children relationships
    //! <parent, vector of <child, is_left>>
    std::unordered_map<int,
                       std::vector<std::pair<int, bool>>> parent_to_children {};

    //! Build graph from parent to child and add nodes to sets
    for (const auto& description : descriptions)
    {
        const int  parent {description[0]};
        const int  child {description[1]};
        const auto is_left = static_cast<bool>(description[2]);

        all_nodes.insert(parent);
        all_nodes.insert(child);
        children.insert(child);
        parent_to_children[parent].emplace_back(child, is_left);
    }

    //! Find root node - search for node in all_nodes that is not in children
    //! @note Could also use std::set_difference with std::set
    int root_val {};
    for (const int node : all_nodes)
    {
        if (!children.contains(node))
        {
            root_val = node;
            break;
        }
    }

    TreeNode* root = new TreeNode(root_val);

    //! Use BFS to construct binary tree from root
    std::queue<TreeNode*> node_queue;
    node_queue.push(root);

    while (!node_queue.empty())
    {
        auto* parent = node_queue.front();
        node_queue.pop();

        //! Iterate over children of current parent
        for (const auto& [child_val, is_left] : parent_to_children[parent->val])
        {
            TreeNode* child_node = new TreeNode(child_val);
            node_queue.push(child_node);

            //! Attach child node to parent based on is_left
            if (is_left)
            {
                parent->left = child_node;
            }
            else
            {
                parent->right = child_node;
            }
        }
    }

    return root;

} // static TreeNode* createBinaryTreeDS1( ...

//! @brief Convert to graph with depth first search discussion solution
//! @param[in] descriptions Vec descriptions_i = {parent_i, child_i, isLeft_i}
//! @return Root of binary tree described by descriptions
static TreeNode* createBinaryTreeDS2(
    const std::vector<std::vector<int>>& descriptions)
{
    //! @details https://leetcode.com/problems
    //!          /create-binary-tree-from-descriptions
    //!
    //!          Time complexity O(N) where N = num entries in descriptions.
    //!          Building parent_to_children, all_nodes, and children takes O(N)
    //!          Finding root node involves iterating through all_nodes, which
    //!          is O(N) in the worst case. Constructing the binary tree using
    //!          DFS takes O(N) since each node is processed once.
    //!          Space complexity O(N). parent_to_children map can store up to
    //!          N entries. The all_nodes and children sets can store up to N
    //!          elements. Recursive DFS stack stores N nodes in worst case.

    //! Sets to track unique children and all nodes in the tree
    std::unordered_set<int> all_nodes {};
    std::unordered_set<int> children {};

    //! Map to store parent to children relationships
    //! <parent, vector of <child, is_left>>
    std::unordered_map<int,
                       std::vector<std::pair<int, bool>>> parent_to_children {};

    //! Build graph from parent to child and add nodes to sets
    for (const auto& description : descriptions)
    {
        const int  parent {description[0]};
        const int  child {description[1]};
        const auto is_left = static_cast<bool>(description[2]);

        all_nodes.insert(parent);
        all_nodes.insert(child);
        children.insert(child);
        parent_to_children[parent].emplace_back(child, is_left);
    }

    //! Find root node - search for node in all_nodes that is not in children
    int root_val {};
    for (const int node : all_nodes)
    {
        if (!children.contains(node))
        {
            root_val = node;
            break;
        }
    }

    //! Build the tree using DFS
    std::function<TreeNode*(int)> dfs = [&](int val) {
        //! Create new TreeNode for current value
        TreeNode* node = new TreeNode(val);

        //! If current node has children, recursively build them
        if (parent_to_children.contains(val))
        {
            for (const auto& [child_val, is_left] : parent_to_children[val])
            {
                //! Attach child node based on is_left
                if (is_left)
                {
                    node->left = dfs(child_val);
                }
                else
                {
                    node->right = dfs(child_val);
                }
            }
        }

        return node;
    };

    return dfs(root_val);

} // static TreeNode* createBinaryTreeDS2( ...

//! @brief Construct tree directly from map
//! @param[in] descriptions Vec descriptions_i = {parent_i, child_i, isLeft_i}
//! @return Root of binary tree described by descriptions
static TreeNode* createBinaryTreeDS3(
    const std::vector<std::vector<int>>& descriptions)
{
    //! @details https://leetcode.com/problems
    //!          /create-binary-tree-from-descriptions
    //!
    //!          Time complexity O(N) where N = number of nodes in binary tree.
    //!          The algorithm iterates through each description once, and for
    //!          each description, performs constant time operations:
    //!          - Checking and adding nodes to node_map
    //!          - Updating node connections (left or right child assignments)
    //!          - Adding child values to children set
    //!          The final loop iterates through node_map, which contains all
    //!          created nodes, to find the root node.
    //!          Space complexity O(N). node_map stores references to all
    //!          created nodes. In the worst case, this map contains all nodes.
    //!          The children set also takes O(N) to store child values.

    //! Map values to TreeNode pointers
    std::unordered_map<int, TreeNode*> node_map {};

    //! Store children values
    std::unordered_set<int> children {};

    //! Iterate through descriptions to create nodes and set up tree
    for (const auto& description : descriptions)
    {
        const int  parent {description[0]};
        const int  child {description[1]};
        const auto is_left = static_cast<bool>(description[2]);

        //! Create parent and child nodes if not already created
        auto parent_ret = node_map.try_emplace(parent, new TreeNode(parent));
        auto child_ret  = node_map.try_emplace(child, new TreeNode(child));

        //! Attach child node to parent's left or right branch
        if (is_left)
        {
            parent_ret.first->second->left = child_ret.first->second;
        }
        else
        {
            parent_ret.first->second->right = child_ret.first->second;
        }

        children.insert(child);
    }

    //! Find and return the root node
    for (const auto& [value, node] : node_map)
    {
        if (!children.contains(value))
        {
            return node;
        }
    }

    //! Should not occur according to problem statement
    return nullptr;

} // static TreeNode* createBinaryTreeDS3( ...

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
