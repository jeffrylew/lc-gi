#include "NaryNode.hpp"

#include <gtest/gtest.h>

//! @brief First attempt to recursively find the max depth of an n-ary tree
//! @param[in] root Pointer to root NaryNode
//! @return Max num of nodes along longest path from root to farthest leaf node
static int maxDepthFARecursive(NaryNode* root)
{
    //! @details leetcode.com/explore/learn/card/n-ary-tree/131/recursion/919/

    //! @todo

} // static int maxDepthFARecursive( ...

TEST(MaxDepthTest, SampleTest1)
{
    constexpr NaryNode two {2};
    constexpr NaryNode four {4};
    constexpr NaryNode five {5};
    constexpr NaryNode six {6};

    const NaryNode three {3, std::vector<NaryNode*> {&five, &six}};
    const NaryNode one {1, std::vector<NaryNode*> {&three, &two, &four}};

    EXPECT_EQ(3, maxDepthFARecursive(&one));
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
}
