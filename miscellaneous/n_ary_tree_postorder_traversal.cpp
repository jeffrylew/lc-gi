#include "NaryNode.hpp"

#include <gtest/gtest.h>

#include <vector>

//! @brief First attempt helper function for postorder traversal of n-ary tree
//! @param[in]  node   Pointer to current NaryNode
//! @param[out] values Output vector of node values
static void postorder_traversalFA(NaryNode* node, std::vector<int>& values)
{
    if (node == nullptr)
    {
        return;
    }

    for (auto& child_ptr : node->children)
    {
        postorder_traversalFA(child_ptr, values);
    }

    values.push_back(node->val);

} // static void postorder_traversalFA( ...

//! @brief First attempt to get the postorder traversal of an n-ary tree
//! @param[in] root Pointer to an NaryNode that is the root of an n-ary tree
//! @return Vector of node values from a postorder traversal
static std::vector<int> postorderFA(NaryNode* root)
{
    //! @details leetcode.com/explore/learn/card/n-ary-tree/130/traversal/926
    //!
    //!          Time complexity O(N) where N = number of nodes in n-ary tree.
    //!          Space complexity O(N) in the worst case where the recursion
    //!          stack stores the entire tree, which is a singly-linked list.

    std::vector<int> node_values;

    postorder_traversalFA(root, node_values);

    return node_values;

} // static std::vector<int> postorderFA( ...

TEST(NaryPostorderTest, SampleTest1)
{
    constexpr NaryNode two {2};
    constexpr NaryNode four {4};
    constexpr NaryNode five {5};
    constexpr NaryNode six {6};

    const NaryNode three {3, std::vector<NaryNode*> {&five, &six}};
    const NaryNode one {1, std::vector<NaryNode*> {&three, &two, &four}};

    const std::vector<int> expected_output {5, 6, 3, 2, 4, 1};

    EXPECT_EQ(expected_output, postorderFA(&one));
}

TEST(NaryPostorderTest, SampleTest2)
{
    constexpr NaryNode two {2};
    constexpr NaryNode six {6};
    constexpr NaryNode ten {10};
    constexpr NaryNode twelve {12};
    constexpr NaryNode thirteen {13};
    constexpr NaryNode fourteen {14};

    const NaryNode eight {8, std::vector<NaryNode*> {&twelve}};
    const NaryNode nine {9, std::vector<NaryNode*> {&thirteen}};
    const NaryNode eleven {11, std::vector<NaryNode*> {&fourteen}};

    const NaryNode four {4, std::vector<NaryNode*> {&eight}};
    const NaryNode seven {7, std::vector<NaryNode*> {&eleven}};

    const NaryNode three {3, std::vector<NaryNode*> {&six, &seven}};
    const NaryNode five {5, std::vector<NaryNode*> {&nine, &ten}};

    const NaryNode one {1, std::vector<NaryNode*> {&two, &three, &four, &five}};

    const std::vector<int> expected_output {
        2, 6, 14, 11, 7, 3, 12, 8, 4, 13, 9, 10, 5, 1};

    EXPECT_EQ(expected_output, postorderFA(&one));
}
