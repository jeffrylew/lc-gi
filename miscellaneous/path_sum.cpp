#include "TreeNode.hpp"

#include <gtest/gtest.h>

#include <functional>
#include <stack>

//! @brief First attempt to check if tree has a root-to-leaf path with targetSum
//! @param[in] root      Pointer to root TreeNode
//! @param[in] targetSum Sum that the root-to-leaf path should have
//! @return True if tree has a root-to-leaf path with targetSum, else false.
static bool hasPathSumFA(TreeNode* root, int targetSum)
{
    //! @details https://leetcode.com/explore/learn/card/data-structure-tree/17/
    //!          solve-problems-recursively/537/
    //!
    //!          Time complexity O(N) where N = number of nodes in tree. In the
    //!          worst case when no path has targetSum, we have to traverse all
    //!          nodes in the binary tree.
    //!          Space complexity O(1) for some primitives and pointers.

    if (root == nullptr)
    {
        return false;
    }

    bool found_target_sum {};

    std::function<void(TreeNode*, int)> has_target_sum =
        [&](TreeNode* curr_node, int curr_sum) {
            if (found_target_sum)
            {
                return;
            }

            curr_sum += curr_node->val;

            if (curr_node->left == nullptr && curr_node->right == nullptr)
            {
                if (!found_target_sum)
                {
                    found_target_sum = curr_sum == targetSum;
                }
                return;
            }

            if (curr_node->left != nullptr)
            {
                has_target_sum(curr_node->left, curr_sum);
            }

            if (curr_node->right != nullptr)
            {
                has_target_sum(curr_node->right, curr_sum);
            }
        };

    has_target_sum(root, 0);

    return found_target_sum;

} // static bool hasPathSumFA( ...

//! @brief Recursive discussion solution
//! @param[in] root      Pointer to root TreeNode
//! @param[in] targetSum Sum that the root-to-leaf path should have
//! @return True if tree has a root-to-leaf path with targetSum, else false.
static bool hasPathSumDS1(TreeNode* root, int targetSum)
{
    //! @details https://leetcode.com/problems/path-sum/editorial/
    //!
    //!          Time complexity O(N) where N = number of nodes. We visit each
    //!          node exactly once.
    //!          Space complexity O(log N). In the worst case, the tree is
    //!          completely unbalanced - each node has only one child node so
    //!          the recursion call occurs N times (the height of the tree) and
    //!          the call stack requires O(N). In the best case, the tree is
    //!          completely balanced and the height of the tree is O(log N).

    if (root == nullptr)
    {
        return false;
    }

    targetSum -= root->val;

    //! If reached a leaf
    if (root->left == nullptr && root->right == nullptr)
    {
        return targetSum == 0;
    }

    return hasPathSumDS1(root->left, targetSum)
        || hasPathSumDS1(root->right, targetSum);
}

//! @brief Iterative discussion solution
//! @param[in] root      Pointer to root TreeNode
//! @param[in] targetSum Sum that the root-to-leaf path should have
//! @return True if tree has a root-to-leaf path with targetSum, else false.
static bool hasPathSumDS2(TreeNode* root, int targetSum)
{
    //! @details https://leetcode.com/problems/path-sum/editorial/
    //!
    //!          Time complexity O(N) where N = number of nodes.
    //!          Space complexity O(N) in the worst case when the tree is
    //!          completely unbalanced (each node has one child) and all N nodes
    //!          are kept in the stack. In the best case, the tree is balanced
    //!          and the height of the tree is log N, giving O(log N).

    if (root == nullptr)
    {
        return false;
    }

    std::stack<TreeNode*> node_stack;
    std::stack<int>       sum_stack;
    node_stack.push(root);
    sum_sum.push(targetSum - root->val);

    while (!node_stack.empty())
    {
        auto* node = node_stack.top();
        node_stack.pop();

        const int curr_sum {sum_stack.top()};
        sum_stack.pop();

        if (node->right == nullptr && node->left == nullptr && curr_sum == 0)
        {
            return true;
        }

        if (node->right != nullptr)
        {
            node_stack.push(node->right);
            sum_stack.push(curr_sum - node->right->val);
        }

        if (node->left != nullptr)
        {
            node_stack.push(node->left);
            sum_stack.push(curr_sum - node->left->val);
        }
    }

    return false;

} // static bool hasPathSumDS2( ...

TEST(HasPathSumTest, SampleTest1)
{
    constexpr TreeNode one {1};
    constexpr TreeNode two {2};
    constexpr TreeNode seven {7};
    constexpr TreeNode thirteen {13};

    const TreeNode four_rhs {4, nullptr, &one};
    const TreeNode eleven {11, &seven, &two};

    const TreeNode four_lhs {4, &eleven, nullptr};
    const TreeNode eight {8, &thirteen, &four_rhs};
    const TreeNode five {5, &four_lhs, &eight};

    EXPECT_TRUE(hasPathSumFA(&five, 22));
    EXPECT_TRUE(hasPathSumDS1(&five, 22));
    EXPECT_TRUE(hasPathSumDS2(&five, 22));
}

TEST(HasPathSumTest, SampleTest2)
{
    constexpr TreeNode two {2};
    constexpr TreeNode three {3};
    const TreeNode     one {1, &two, &three};

    EXPECT_FALSE(hasPathSumFA(&one, 5));
    EXPECT_FALSE(hasPathSumDS1(&one, 5));
    EXPECT_FALSE(hasPathSumDS2(&one, 5));
}

TEST(HasPathSumTest, SampleTest3)
{
    EXPECT_FALSE(hasPathSumFA(nullptr, 0));
    EXPECT_FALSE(hasPathSumDS1(nullptr, 0));
    EXPECT_FALSE(hasPathSumDS2(nullptr, 0));
}

TEST(HasPathSumTest, SampleTest4)
{
    constexpr TreeNode two {2};
    const TreeNode     one {1, &two, nullptr};

    EXPECT_FALSE(hasPathSumFA(&one, 1));
    EXPECT_FALSE(hasPathSumDS1(&one, 1));
    EXPECT_FALSE(hasPathSumDS2(&one, 1));
}
