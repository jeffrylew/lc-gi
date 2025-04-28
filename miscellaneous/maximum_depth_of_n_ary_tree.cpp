#include "NaryNode.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <vector>

//! @brief First attempt top down recursive solution to find the max depth
//! @param[in] root Pointer to root NaryNode
//! @return Max num of nodes along longest path from root to farthest leaf node
static int maxDepthFARecursive(NaryNode* root)
{
    //! @details leetcode.com/explore/learn/card/n-ary-tree/131/recursion/919/
    //!
    //!          Time complexity O(N) where N = number of nodes in n-ary tree
    //!          Space complexity O(N) in the worst case when n-ary tree is
    //!          skewed like a singly linked list

    if (root == nullptr)
    {
        return 0;
    }

    int max_depth {};

    const std::function<void(NaryNode*, int)> find_max_depth =
        [&](NaryNode* node, int curr_depth) {
            if (node == nullptr)
            {
                return;
            }

            if (node->children.empty())
            {
                max_depth = std::max(max_depth, curr_depth);
                return;
            }

            for (const auto& child : node->children)
            {
                find_max_depth(child, curr_depth + 1);
            }
        };

    find_max_depth(root, 1);

    return max_depth;

} // static int maxDepthFARecursive( ...

//! @brief Second attempt bottom up recursive solution to find the max depth
//! @param[in] root Pointer to root NaryNode
//! @return Max num of nodes along longest path from root to farthest leaf node
static int maxDepthSARecursive(NaryNode* root)
{
    //! @details leetcode.com/explore/learn/card/n-ary-tree/131/recursion/919/
    //!
    //!          Time complexity O(N) where N = number of nodes in n-ary tree
    //!          Space complexity O(N) in the worst case when n-ary tree is
    //!          skewed like a singly linked list

    if (root == nullptr)
    {
        return 0;
    }

    int max_depth {};

    for (const auto& child : root->children)
    {
        max_depth = std::max(max_depth, maxDepthSARecursive(child));
    }

    return max_depth + 1;

} // static int maxDepthSARecursive( ...

//! @brief Recursive discussion solution to find the max depth of n-ary tree
//! @param[in] root Pointer to root NaryNode
//! @return Max num of nodes along longest path from root to farthest leaf node
static int maxDepthDS1(NaryNode* root)
{
    //! @details https://leetcode.com/problems/maximum-depth-of-n-ary-tree
    //!
    //!          Time complexity O(N) where N = number of nodes in n-ary tree.
    //!          Space complexity O(N) in the worst case when the tree is
    //!          completely unbalanced. Each node has only one child node so the
    //!          recursion call occurs N times (the height of the tree). In the
    //!          best case, the tree is completely balanced and the height of
    //!          the tree is log N.

    if (root == nullptr)
    {
        return 0;
    }

    if (root->children.empty())
    {
        return 1;
    }

    std::vector<int> heights {};
    heights.reserve(root->children.size());

    for (const auto& child : root->children)
    {
        heights.push_back(maxDepthDS1(child));
    }

    return *std::max_element(heights.begin(), heights.end()) + 1;

} // static int maxDepthDS1( ...

TEST(MaxDepthTest, SampleTest1)
{
    constexpr NaryNode two {2};
    constexpr NaryNode four {4};
    constexpr NaryNode five {5};
    constexpr NaryNode six {6};

    const NaryNode three {3, std::vector<NaryNode*> {&five, &six}};
    const NaryNode one {1, std::vector<NaryNode*> {&three, &two, &four}};

    EXPECT_EQ(3, maxDepthFARecursive(&one));
    EXPECT_EQ(3, maxDepthSARecursive(&one));
    EXPECT_EQ(3, maxDepthDS1(&one));
}

TEST(MaxDepthTest, SampleTest2)
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

    EXPECT_EQ(5, maxDepthFARecursive(&one));
    EXPECT_EQ(5, maxDepthSARecursive(&one));
    EXPECT_EQ(5, maxDepthDS1(&one));
}
