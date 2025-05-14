#include "TreeNode.hpp"

#include <gtest/gtest.h>

#include <queue>

static void preorder_traversal_LHS(TreeNode* root, std::queue<int>& node_values)
{
    if (root == nullptr)
    {
        return;
    }

    node_values.push(root->val);
    preorder_traversal_LHS(root->left, node_values);
    preorder_traversal_LHS(root->right, node_values);
}

[[nodiscard]] static
    bool preorder_traversal_RHS(TreeNode* root, std::queue<int>& node_values)
{
    if (root == nullptr)
    {
        return node_values.empty();
    }

    if (node_values.empty())
    {
        return false;
    }

    const auto oldest_val = node_values.front();
    if (oldest_val == root->val)
    {
        node_values.pop();

        if (root->right != nullptr)
        {
            preorder_traversal_RHS(root->right, node_values);
        }
        if (root->left != nullptr)
        {
            preorder_traversal_RHS(root->left, node_values);
        }
    }

    return node_values.empty();
}

//! @brief First attempt to check if binary tree is a mirror of itself
//! @param[in] root Pointer to root TreeNode
//! @return True if binary tree is symmetric around its center, else false
static bool isSymmetricFA(TreeNode* root)
{
    //! @details https://leetcode.com/explore/learn/card
    //!          /data-structure-tree/17/solve-problems-recursively/536
    //!
    //!          First attempt solution does not pass SampleTest2

    if (root->left == nullptr && root->right == nullptr)
    {
        return true;
    }

    if (root->left == nullptr || root->right == nullptr)
    {
        return false;
    }

    std::queue<int> node_values {};
    preorder_traversal_LHS(root->left, node_values);
    return preorder_traversal_RHS(root->right, node_values);

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

    EXPECT_TRUE(isSymmetricFA(&one));
    // EXPECT_FALSE(isSymmetricFA(&one));
}
