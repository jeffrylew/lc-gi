#include "TreeNode.hpp"

#include <gtest/gtest.h>

#include <queue>

static void preorder_traversal_FA(TreeNode* root, std::queue<int>& node_values)
{
    if (root == nullptr)
    {
        return;
    }

    node_values.push(root->val);
    preorder_traversal_FA(root->left);
    preorder_traversal_FA(root->right);
}

static void postorder_traversal_FA(TreeNode* root, std::queue<int>& node_values)
{
    if (root == nullptr)
    {
        return;
    }

    if (node_values.empty())
    {
        return;
    }

    if (root->val == node_values.front())
    {
        node_values.pop();
        postorder_traversal_FA(root->right);
        postorder_traversal_FA(root->left);  
    }
}

//! @brief First attempt to check if binary tree is a mirror of itself
//! @param[in] root Pointer to root TreeNode
//! @return True if binary tree is symmetric around its center, else false
static bool isSymmetricFA(TreeNode* root)
{
    //! @details https://leetcode.com/explore/learn/card
    //!          /data-structure-tree/17/solve-problems-recursively/536

    if (root->left == nullptr && root->right == nullptr)
    {
        return true;
    }

    if (root->left == nullptr || root->right == nullptr)
    {
        return false;
    }

    std::queue<int> node_values {};
    preorder_traversal_FA(root->left, node_values);
    postorder_traversal_FA(root->right, node_values);
    return node_values.empty();

} // static bool isSymmetricFA( ...

TEST(IsSymmetricTest, SampleTest1)
{
    constexpr TreeNode three_lhs {3};
    constexpr TreeNode three_rhs {3};
    constexpr TreeNode four_lhs {4};
    constexpr TreeNode four_rhs {4};

    const TreeNode two_lhs {2, &three_lhs, &four_lhs};
    const TreeNode two_rhs {2, &three_rhs, &four_rhs};
    const TreeNode one {1, &two_lhs, &two_rhs};

    EXPECT_TRUE(isSymmetricFA(&one));
}

TEST(IsSymmetricTest, SampleTest2)
{
    constexpr TreeNode three_lhs {3};
    constexpr TreeNode three_rhs {3};

    const TreeNode two_lhs {2, nullptr, &three_lhs};
    const TreeNode two_rhs {2, nullptr, &three_rhs};
    const TreeNode one {1, &two_lhs, &two_rhs};

    EXPECT_FALSE(isSymmetricFA(&one));
}
