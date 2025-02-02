#include "../miscellaneous/TreeNode.hpp"

#include <gtest/gtest.h>

#include <stack>
#include <unordered_map>
#include <utility>

//! @brief First attempt to check if two trees are flip equivalent
//! @param[in, out] root1 Pointer to root of first binary tree
//! @param[in, out] root2 Pointer to root of second binary tree
//! @return True if two trees are flip equivalent, else false
static bool flipEquivFA(TreeNode* root1, TreeNode* root2)
{
    //! @details https://leetcode.com/explore/interview/card/google/61
    //!          /trees-and-graphs/3077/
    //!
    //!          Time complexity O(M + N) where M = number of nodes in first
    //!          binary tree and N = number of nodes in second binary tree. In
    //!          the worst case, both trees are flip equivalent and need to
    //!          traverse all nodes in both trees.
    //!          Space complexity O(M) for the parent_children map.

    if (root1 == nullptr && root2 == nullptr)
    {
        return true;
    }

    if ((root1 == nullptr && root2 != nullptr)
        || (root1 != nullptr && root2 == nullptr)
        || (root1->val != root2->val))
    {
        return false;
    }

    //! Map of parent node to its children. null child represented by -1
    std::unordered_map<int, std::pair<int, int>> parent_children {};

    std::stack<TreeNode*> node_stack({root1});

    while (!node_stack.empty())
    {
        auto node = node_stack.top();
        node_stack.pop();

        parent_children.emplace(node->val, std::make_pair(-1, -1));
        auto& [left_child, right_child] = parent_children[node->val];

        if (node->left != nullptr)
        {
            left_child = node->left->val;
            node_stack.push(node->left);
        }

        if (node->right != nullptr)
        {
            right_child = node->right->val;
            node_stack.push(node->right);
        }
    }

    node_stack.push(root2);

    while (!node_stack.empty())
    {
        auto node = node_stack.top();
        node_stack.pop();

        if (!parent_children.contains(node->val))
        {
            return false;
        }

        int left_child2 {-1};
        if (node->left != nullptr)
        {
            left_child2 = node->left->val;
            node_stack.push(node->left);
        }

        int right_child2 {-1};
        if (node->right != nullptr)
        {
            right_child2 = node->right->val;
            node_stack.push(node->right);
        }

        const auto& [left_child1, right_child1] = parent_children[node->val];

        if ((left_child1 != left_child2 || right_child1 != right_child2)
            && (left_child1 != right_child2 || right_child1 != left_child2))
        {
            return false;
        }
    }

    return true;

} // static bool flipEquivFA( ...

//! @brief Recursive (top-down traversal) discussion solution
//! @param[in, out] root1 Pointer to root of first binary tree
//! @param[in, out] root2 Pointer to root of second binary tree
//! @return True if two trees are flip equivalent, else false
static bool flipEquivDS1(TreeNode* root1, TreeNode* root2)
{
    //! @details https://leetcode.com/problems/flip-equivalent-binary-trees
    //!
    //!          Time complexity O(N) where N = number of nodes in the smaller
    //!          tree. Recursion stops at the leaf nodes or when a mismatch
    //!          occurs. In the worst case, every node in the smaller tree will
    //!          be visited.
    //!          Space complexity O(N) due to the recursion stack. In the worst
    //!          case, the recursion is deep as the height of the tree, which is
    //!          O(N) in the case of a skewed tree (each node has one child).
    //!          For a balanced tree, the space complexity is O(log N) because
    //!          the height is logarithmic relative to the number of nodes.

    //! Both trees are empty
    if (root1 == nullptr && root2 == nullptr)
    {
        return true;
    }

    //! One of the trees is empty
    if (root1 == nullptr || root2 == nullptr)
    {
        return false;
    }

    //! Corresponding values differ
    if (root1->val != root2->val)
    {
        return false;
    }

    //! Check if corresponding subtrees are flip equivalent
    const bool flip_equivalent_with_no_swaps {
        flipEquivDS1(root1->left, root2->left)
        && flipEquivDS1(root1->right, root2->right)};

    //! Check if opposite subtrees are flip equivalent
    const bool flip_equivalent_with_swaps {
        flipEquivDS1(root1->left, root2->right)
        && flipEquivDS1(root1->right, root2->left)};

    return flip_equivalent_with_no_swaps || flip_equivalent_with_swaps;

} // static bool flipEquivDS1( ...

TEST(FlipEquivTest, SampleTest1)
{
    const TreeNode root1_four {4};
    const TreeNode root1_six {6};
    const TreeNode root1_seven {7};
    const TreeNode root1_eight {8};

    TreeNode root1_five {5, &root1_seven, &root1_eight};
    TreeNode root1_two {2, &root1_four, &root1_five};
    TreeNode root1_three {3, &root1_six, nullptr};
    TreeNode root1_one {1, &root1_two, &root1_three};

    const TreeNode root2_four {4};
    const TreeNode root2_six {6};
    const TreeNode root2_seven {7};
    const TreeNode root2_eight {8};

    TreeNode root2_five {5, &root2_eight, &root2_seven};
    TreeNode root2_three {3, nullptr, &root2_six};
    TreeNode root2_two {2, &root2_four, &root2_five};
    TreeNode root2_one {1, &root2_three, &root2_two};

    EXPECT_TRUE(flipEquivFA(&root1_one, &root2_one));
    EXPECT_TRUE(flipEquivDS1(&root1_one, &root2_one));
}

TEST(FlipEquivTest, SampleTest2)
{
    EXPECT_TRUE(flipEquivFA(nullptr, nullptr));
    EXPECT_TRUE(flipEquivDS1(nullptr, nullptr));
}

TEST(FlipEquivTest, SampleTest3)
{
    const TreeNode one {1};

    EXPECT_FALSE(flipEquivFA(nullptr, &one));
    EXPECT_FALSE(flipEquivDS1(nullptr, &one));
}

TEST(FlipEquivTest, SampleTest4)
{
    const TreeNode root1_two {2};
    const TreeNode root1_three {3};
    TreeNode       root1_one {1, &root1_two, &root1_three};

    const TreeNode root2_three {3};
    TreeNode       root2_two {2, &root2_three, nullptr};
    TreeNode       root2_one {1, &root2_two, nullptr};

    EXPECT_FALSE(flipEquivFA(&root1_one, &root2_one));
    EXPECT_FALSE(flipEquivDS1(&root1_one, &root2_one));
}
