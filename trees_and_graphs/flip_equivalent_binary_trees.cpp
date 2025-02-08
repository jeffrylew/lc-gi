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

//! @brief Check if nodes share the same value and should be processed further
//! @param[in] node1 Pointer to the first TreeNode
//! @param[in] node2 Pointer to the second TreeNode
//! @return True if the nodes share the same value, else false
[[nodiscard]] static bool are_node_values_equal_DS2(const TreeNode* node1,
                                                    const TreeNode* node2)
{
    if (node1 == nullptr && node2 == nullptr)
    {
        return true;
    }

    if (node1 != nullptr && node2 != nullptr && node1->val == node2->val)
    {
        return true;
    }

    return false;
}

//! @brief Iterative DFS discussion solution
//! @param[in, out] root1 Pointer to root of first binary tree
//! @param[in, out] root2 Pointer to root of second binary tree
//! @return True if two trees are flip equivalent, else false
static bool flipEquivDS2(TreeNode* root1, TreeNode* root2)
{
    //! @details https://leetcode.com/problems/flip-equivalent-binary-trees
    //!
    //!          Time complexity O(N) where N = number of nodes in smaller tree.
    //!          Each node in the smaller tree enters the stack at most twice -
    //!          one with swap and one without. Thus, the loop will run O(N)
    //!          times where the operations within the loop are O(1).
    //!          Space complexity O(N). The size of the stack can reach at most
    //!          twice the number of nodes in the smaller tree, as each node can
    //!          be pushed onto the stack in up to two configurations.

    //! Initialize stack storing pairs of nodes
    std::stack<std::pair<TreeNode*, TreeNode*>> node_pair_stack({root1, root2});

    while (!node_pair_stack.empty())
    {
        const auto [node1, node2] = node_pair_stack.top();
        node_pair_stack.pop();

        if (node1 == nullptr && node2 == nullptr)
        {
            continue;
        }

        if (!are_node_values_equal_DS2(node1, node2))
        {
            return false;
        }

        //! Check both configurations: no swap and swap
        if (are_node_values_equal_DS2(node1->left, node2->left)
            && are_node_values_equal_DS2(node1->right, node2->right))
        {
            node_pair_stack.push({node1->left, node2->left});
            node_pair_stack.push({node1->right, node2->right});
        }
        else if (are_node_values_equal_DS2(node1->left, node2->right)
                 && are_node_values_equal_DS2(node1->right, node2->left))
        {
            node_pair_stack.push({node1->left, node2->right});
            node_pair_stack.push({node1->right, node2->left});
        }
        else
        {
            return false;
        }
    }

    return true;

} // static bool flipEquivDS2( ...

//! @brief Get canonical form of tree
//! @param[in, out] root Pointer to root of binary tree
static void find_canonical_form_DS3(TreeNode* root)
{
    //! @details A binary tree is in its canonical form if, for each node, one
    //!          of the following conditions holds
    //!          - The node has no children
    //!          - The node has only a left child
    //!          - The left child's value is greater than right child's value
    //!          Two binary trees are flip-equivalent if and only if they have
    //!          the same canonical form.

    if (root == nullptr)
    {
        return;
    }

    //! Post-order traversal: First bring subtrees to their canonical form
    find_canonical_form_DS3(root->left);
    find_canonical_form_DS3(root->right);

    if (root->right == nullptr)
    {
        return;
    }

    //! Swap subtrees so that the left is not empty
    if (root->left == nullptr)
    {
        root->left  = root->right;
        root->right = nullptr;
        return;
    }

    //! Both left and right are non-nullptr
    auto* left  = root->left;
    auto* right = root->right;

    //! Swap subtrees
    if (left->val > right->val)
    {
        root->left  = right;
        root->right = left;
    }

} // static void find_canonical_form_DS3( ...

//! @brief Check if both trees are equivalent
//! @param[in] root1 Pointer to root of first binary tree
//! @param[in] root2 Pointer to root of second binary tree
//! @return True if both binary trees are equivalent
[[nodiscard]] static bool are_equivalent_DS3(TreeNode* root1, TreeNode* root2)
{
    if (root1 == nullptr && root2 == nullptr)
    {
        return true;
    }

    if (root1 == nullptr || root2 == nullptr)
    {
        return false;
    }

    if (root1->val != root2->val)
    {
        return false;
    }

    return are_equivalent_DS3(root1->left, root2->left)
           && are_equivalent_DS3(root1->right, root2->right);

} // [[nodiscard]] static bool are_equivalent_DS3( ...

//! @brief Canonical forms discussion solution
//! @param[in, out] root1 Pointer to root of first binary tree
//! @param[in, out] root2 Pointer to root of second binary tree
//! @return True if two trees are flip equivalent, else false
static bool flipEquivDS3(TreeNode* root1, TreeNode* root2)
{
    //! @details https://leetcode.com/problems/flip-equivalent-binary-trees
    //!
    //!          The choice of tree for the flip operation does not affect the
    //!          outcome. We can apply flip operations to transform each tree
    //!          into a standardized format that makes it easier to determine
    //!          whether they are equivalent.
    //!
    //!          Time complexity O(N) where N = number of nodes in the bigger
    //!          tree. find_canonical_form_DS3 processes each node in the tree
    //!          exactly once. The are_equivalent_DS3 function performs a DFS on
    //!          both trees, also visiting each node once in O(N).
    //!          Space complexity O(N) for the recursion stack. In the worst
    //!          case, the recursion goes as deep as the height of the tree,
    //!          which can be O(N) for a skewed tree (each internal node has
    //!          only one child). For a balanced tree, the space complexity is
    //!          O(log N) because the height of the tree would be logarithmic
    //!          relative to the number of nodes.

    find_canonical_form_DS3(root1); //! Convert root1 tree to its canonical form
    find_canonical_form_DS3(root2); //! Convert root2 tree to its canonical form

    return are_equivalent_DS3(root1, root2);

} // static bool flipEquivDS3( ...

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
    EXPECT_TRUE(flipEquivDS2(&root1_one, &root2_one));
    EXPECT_TRUE(flipEquivDS3(&root1_one, &root2_one));
}

TEST(FlipEquivTest, SampleTest2)
{
    EXPECT_TRUE(flipEquivFA(nullptr, nullptr));
    EXPECT_TRUE(flipEquivDS1(nullptr, nullptr));
    EXPECT_TRUE(flipEquivDS2(nullptr, nullptr));
    EXPECT_TRUE(flipEquivDS3(nullptr, nullptr));
}

TEST(FlipEquivTest, SampleTest3)
{
    const TreeNode one {1};

    EXPECT_FALSE(flipEquivFA(nullptr, &one));
    EXPECT_FALSE(flipEquivDS1(nullptr, &one));
    EXPECT_FALSE(flipEquivDS2(nullptr, &one));
    EXPECT_FALSE(flipEquivDS3(nullptr, &one));
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
    EXPECT_FALSE(flipEquivDS2(&root1_one, &root2_one));
    EXPECT_FALSE(flipEquivDS3(&root1_one, &root2_one));
}
