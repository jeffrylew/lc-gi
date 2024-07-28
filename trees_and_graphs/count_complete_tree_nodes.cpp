#include "../miscellaneous/TreeNode.hpp"

#include <gtest/gtest.h>

#include <cmath>
#include <stack>
#include <utility>

//! @brief First attempt solution to get number of nodes in complete binary tree
//! @param[in] root Pointer to root of complete binary tree
//! @return Number of nodes in complete binary tree
static int countNodesFA(TreeNode* root)
{
    //! @details https://leetcode.com/problems/count-complete-tree-nodes
    //!
    //!          Time complexity O(2 ^ H) where H = height of tree. Process
    //!          each node before last level, which is the height.
    //!          Space complexity O(2 ^ H). The worst case is processing the row
    //!          before the last where the last row is complete. The second-to-
    //!          last row will store 2 ^ H elements in the node_level stack.

    if (root == nullptr)
    {
        return 0;
    }

    //! Number of nodes on last level of tree
    int num_last_level_nodes {};

    //! Get height of tree
    int   height {};
    auto* node_ptr = root;
    while (node_ptr->left != nullptr)
    {
        ++height;
        node_ptr = node_ptr->left;
    }

    //! <current node, level>
    std::stack<std::pair<TreeNode*, int>> node_level {};
    node_level.emplace(root, 0);

    while (!node_level.empty())
    {
        const auto [node, curr_level] = node_level.top();
        node_level.pop();

        if (node->left == nullptr && node->right == nullptr)
        {
            if (curr_level != height)
            {
                //! Found a node in level before last with no children
                break;
            }

            ++num_last_level_nodes;

            //! node is on last level, skip logic below
            continue;
        }

        if (node->right != nullptr)
        {
            node_level.emplace(node->right, curr_level + 1);
        }
        else
        {
            //! Node in level before the last does not have a right child
            //! Increment num_last_level_nodes for unprocessed left child
            ++num_last_level_nodes;
            break;
        }

        if (node->left != nullptr)
        {
            node_level.emplace(node->left, curr_level + 1);
        }
    }

    return std::pow(2, height) - 1 + num_last_level_nodes;

} // static int countNodesFA( ...

TEST(CountNodesTest, SampleTest1)
{
    const TreeNode six {6};
    const TreeNode three {3, &six, nullptr};

    const TreeNode five {5};
    const TreeNode four {4};
    const TreeNode two {2, &four, &five};

    const TreeNode one {1, &two, &three};

    EXPECT_EQ(6, countNodesFA(&one));
}

TEST(CountNodesTest, SampleTest2)
{
    EXPECT_EQ(0, countNodesFA(nullptr));
}

TEST(CountNodesTest, SampleTest3)
{
    const TreeNode one {1};

    EXPECT_EQ(1, countNodesFA(&one));
}
