#include "../miscellaneous/TreeNode.hpp"

#include <gtest/gtest.h>

//! @brief First attempt to check if two trees are flip equivalent
//! @param[in, out] root1 Pointer to root of first binary tree
//! @param[in, out] root2 Pointer to root of second binary tree
//! @return True if two trees are flip equivalent, else false
static bool flipEquivFA(TreeNode* root1, TreeNode* root2)
{
    //! @todo

} // static bool flipEquivFA( ...

TEST(FlipEquivTest, SampleTest2)
{
    EXPECT_TRUE(flipEquivFA(nullptr, nullptr));
}

TEST(FlipEquivTest, SampleTest3)
{
    TreeNode one {1};

    EXPECT_FALSE(flipEquivFA(nullptr, &one));
}
