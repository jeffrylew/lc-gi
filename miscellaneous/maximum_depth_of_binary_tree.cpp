#include "TreeNode.hpp"

#include <gtest/gtest.h>

#include <algorithm>

//! @brief Top down recursive helper for first attempt solution
//! @param[in]     node           Pointer to current TreeNode to process
//! @param[in]     curr_depth     Current depth of node in the binary tree
//! @param[in,out] max_tree_depth Reference to int storing maximum tree depth
static void max_depth_FA_top_down(TreeNode* node,
                                  int       curr_depth,
                                  int&      max_tree_depth)
{
    if (node == nullptr)
    {
        return;
    }

    if (node->left == nullptr && node->right == nullptr)
    {
        max_tree_depth = std::max(max_tree_depth, curr_depth);
        return;
    }

    max_depth_FA_top_down(node->left, curr_depth + 1, max_tree_depth);
    max_depth_FA_top_down(node->right, curr_depth + 1, max_tree_depth);
}

//! @brief Top down recursive first attempt solution to get max depth of tree
//! @param[in] root Pointer to root of binary tree
//! @return Max depth of binary tree
static int maxDepthFATopDown(TreeNode* root)
{
    //! @details https://leetcode.com/explore/learn/card/data-structure-tree/17
    //!          /solve-problems-recursively/535
    //!
    //!          Time complexity O(N) where N = number of nodes in tree.
    //!          Space complexity O(N) in the worst case for a skewed binary
    //!          tree that resembles a singly linked list.

    int max_depth {};
    if (root == nullptr)
    {
        return max_depth;
    }

    max_depth_FA_top_down(root, 1, max_depth);
    return max_depth;

} // static int maxDepthFATopDown( ...

//! @brief Bottom up recursive first attempt solution to get max depth of tree
//! @param[in] root Pointer to root of binary tree
//! @return Max depth of binary tree
static int maxDepthFABottomUp(TreeNode* root)
{
    //! @details https://leetcode.com/explore/learn/card/data-structure-tree/17
    //!          /solve-problems-recursively/534/
    //!
    //!          Time complexity O(N) where N = number of nodes in tree.
    //!          Space complexity O(N) in the worst case for a skewed binary
    //!          tree that resembles a singly linked list.

    if (root == nullptr)
    {
        return 0;
    }

    const int left_depth {maxDepthFABottomUp(root->left)};
    const int right_depth {maxDepthFABottomUp(root->right)};

    return std::max(left_depth, right_depth) + 1;

} // static int maxDepthFABottomUp( ...

TEST(MaxDepthTest, SampleTest1)
{
    constexpr TreeNode seven {7};
    constexpr TreeNode nine {9};
    constexpr TreeNode fifteen {15};

    const TreeNode twenty {20, &fifteen, &seven};
    const TreeNode three {3, &nine, &twenty};

    EXPECT_EQ(3, maxDepthFATopDown(&three));
    EXPECT_EQ(3, maxDepthFABottomUp(&three));
}

TEST(MaxDepthTest, SampleTest2)
{
    constexpr TreeNode two {2};
    const     TreeNode one {1, nullptr, &two};

    EXPECT_EQ(2, maxDepthFATopDown(&one));
    EXPECT_EQ(2, maxDepthFABottomUp(&one));
}
