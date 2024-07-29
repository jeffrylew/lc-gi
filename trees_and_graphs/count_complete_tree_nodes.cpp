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
    //!          Since H = log N where N = number of nodes in the tree, the
    //!          complexity is O(N).
    //!          Space complexity O(2 ^ H). The worst case is processing the row
    //!          before the last where the last row is complete. The second-to-
    //!          last row will store 2 ^ H elements in the node_level stack.
    //!          Similar to the time complexity, O(2 ^ H) = O(N).

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

//! @brief Linear time discussion solution
//! @param[in] root Pointer to root of complete binary tree
//! @return Number of nodes in complete binary tree
static int countNodesDS1(TreeNode* root)
{
    //! @details https://leetcode.com/problems/count-complete-tree-nodes
    //!
    //!          Time complexity O(N) where N = number of nodes in tree
    //!          Space complexity O(d = log N) for the recursion stack,
    //!          where d = tree depth.

    return root != nullptr
        ? 1 + countNodesDS1(root->right) + countNodesDS1(root->left)
        : 0;
}

//! @brief Binary search discussion solution
//! @param[in] root Pointer to root of complete binary tree
//! @return Number of nodes in complete binary tree
static int countNodesDS2(TreeNode* root)
{
    //! @details https://leetcode.com/problems/count-complete-tree-nodes
    //!
    //!          Time complexity O(depth ^ 2) = O(log^2 N) where N = number of
    //!          nodes in the tree
    //!          Space complexity O(1)

    if (root == nullptr)
    {
        //! Tree is empty
        return 0;
    }

    const auto compute_height = [](TreeNode* node) -> int {
        int height {};

        while (node->left != nullptr)
        {
            node = node->left;
            ++height;
        }

        return height;
    };

    const int depth {compute_height(root)};
    if (depth == 0)
    {
        //! Tree contains 1 node
        return 1;
    }

    //! @brief Check if last level node with index idx exists
    //! @param[in] idx  Index of node in last level to check if it exists
    //! @param[in] node Pointer to TreeNode in first level to search from
    //! @return True if the last level node at index idx exists, else false
    const auto exists = [&depth](int idx, TreeNode* node) -> bool {
        //! Last level nodes enumerated from 0 to 2 ^ depth - 1 (left -> right)
        int left {};
        int right {std::pow(2, depth) - 1}; // Index of last node in last level
        int pivot {};

        for (int i = 0; i < depth; ++i)
        {
            pivot = left + (right - left) / 2;

            if (idx <= pivot)
            {
                node  = node->left;
                right = pivot;
            }
            else
            {
                node = node->right;
                left = pivot + 1;
            }
        }

        return node != nullptr;

    }; // const auto exists = ...

    //! Perform binary search to check how many nodes exist
    //! Last level nodes enumerated from 0 to 2 ^ depth - 1 (left -> right)
    int left {1};
    int right {std::pow(2, depth) - 1};
    int pivot {};

    while (left <= right)
    {
        pivot = left + (right - left) / 2;

        if (exists(pivot, root))
        {
            left = pivot + 1;
        }
        else
        {
            right = pivot - 1;
        }
    }

    //! The tree contains 2 ^ depth - 1 nodes on the first (depth - 1) levels
    //! and left nodes on the last level
    return std::pow(2, depth) - 1 + left;

} // static int countNodesDS2( ...

TEST(CountNodesTest, SampleTest1)
{
    const TreeNode six {6};
    const TreeNode three {3, &six, nullptr};

    const TreeNode five {5};
    const TreeNode four {4};
    const TreeNode two {2, &four, &five};

    const TreeNode one {1, &two, &three};

    EXPECT_EQ(6, countNodesFA(&one));
    EXPECT_EQ(6, countNodesDS1(&one));
    EXPECT_EQ(6, countNodesDS2(&one));
}

TEST(CountNodesTest, SampleTest2)
{
    EXPECT_EQ(0, countNodesFA(nullptr));
    EXPECT_EQ(0, countNodesDS1(nullptr));
    EXPECT_EQ(0, countNodesDS2(nullptr));
}

TEST(CountNodesTest, SampleTest3)
{
    const TreeNode one {1};

    EXPECT_EQ(1, countNodesFA(&one));
    EXPECT_EQ(1, countNodesDS1(&one));
    EXPECT_EQ(1, countNodesDS2(&one));
}
