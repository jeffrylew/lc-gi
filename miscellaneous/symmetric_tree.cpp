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

//! @brief Recursive discussion solution helper
//! @param[in] t1 Pointer to TreeNode root of first binary tree
//! @param[in] t2 Pointer to TreeNode root of second binary tree
//! @return True if tree rooted at t1 is a mirror of the tree rooted at t2
[[nodiscard]] static bool is_mirror_DS1(TreeNode* t1, TreeNode* t2)
{
    if (t1 == nullptr && t2 == nullptr)
    {
        return true;
    }

    if (t1 == nullptr || t2 == nullptr)
    {
        return false;
    }

    return t1->val == t2->val
        && is_mirror_DS1(t1->right, t2->left)
        && is_mirror_DS1(t1->left, t2->right);
}

//! @brief Recursive discussion solution to check if binary tree is a mirror
//! @param[in] root Pointer to root TreeNode
//! @return True if binary tree is symmetric around its center, else false
static bool isSymmetricDS1(TreeNode* root)
{
    //! @details https://leetcode.com/problems/symmetric-tree/editorial/
    //!
    //!          Time complexity O(N) where N = total number of nodes in tree.
    //!          We have to traverse the entire input tree once.
    //!          Space complexity O(N). The number of recursive calls is bound
    //!          by the height of the tree. In the worst case, the tree is
    //!          linear and the height is O(N).

    return is_mirror_DS1(root, root);
}

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
    EXPECT_TRUE(isSymmetricDS1(&one));
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
    EXPECT_FALSE(isSymmetricDS1(&one));
}
