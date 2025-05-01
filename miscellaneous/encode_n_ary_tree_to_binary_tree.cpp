#include "NaryNode.hpp"
#include "TreeNode.hpp"

#include <gtest/gtest.h>

#include <vector>

//! @class CodecFA
//! @brief First attempt solution
class CodecFA
{
public:
    //! Encodes an n-ary tree to a binary tree
    TreeNode* encode(NaryNode* root)
    {
        //! @todo
    }

    //! Decodes a binary tree to an n-ary tree
    NaryNode* decode(TreeNode* root)
    {
        //! @todo
    }
};

TEST(CodecTest, SampleTest1)
{
    NaryNode two {2};
    NaryNode four {4};
    NaryNode five {5};
    NaryNode six {6};

    NaryNode three {3, std::vector<NaryNode*> {&five, &six}};
    NaryNode one {1, std::vector<NaryNode*> {&three, &two, &four}};

    CodecFA    codec_fa;
    const auto root_fa = codec_fa.decode(codec_fa.encode(&one));
    EXPECT_NE(nullptr, root_fa);
    EXPECT_EQ(one.val, root_fa->val);
    EXPECT_EQ(one.children, root_fa->children);
}

TEST(CodecTest, SampleTest2)
{
    //! @todo
}

TEST(CodecTest, SampleTest3)
{
    CodecFA codec_fa;
    EXPECT_EQ(nullptr, codec_fa.decode(codec_fa.encode(nullptr)));
}
