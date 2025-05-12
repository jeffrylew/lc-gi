#include "TreeNode.hpp"

#include <gtest/gtest.h>

//! @brief First attempt to check if binary tree is a mirror of itself
//! @param[in] root Pointer to root TreeNode
//! @return True if binary tree is symmetric around its center, else false
static bool isSymmetricFA(TreeNode* root)
{
    //! @details https://leetcode.com/explore/learn/card
    //!          /data-structure-tree/17/solve-problems-recursively/536

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
