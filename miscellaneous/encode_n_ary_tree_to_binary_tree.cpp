#include "NaryNode.hpp"
#include "TreeNode.hpp"

#include <gtest/gtest.h>

#include <vector>

//! @class CodecFA
//! @brief First attempt solution to encode an n-ary tree into a binary tree and
//!        decode the binary tree back to the original n-ary tree
//! @details leetcode.com/explore/learn/card/n-ary-tree/131/recursion/920
//!          Used leetcode.com/explore/learn/card/n-ary-tree/131/recursion/922
class CodecFA
{
public:
    //! @brief Recursive helper function to encode n-ary tree to a binary tree
    //! @param[in, out] parent_nary_node Pointer to parent of current NaryNode
    //! @param[in, out] curr_nary_node   Pointer to current NaryNode
    //! @param[in]      curr_idx         Index of current NaryNode in parent
    //! @return Pointer to TreeNode
    TreeNode* encode(NaryNode* parent_nary_node,
                     NaryNode* curr_nary_node,
                     int       curr_idx)
    {
        if (curr_nary_node == nullptr)
        {
            return nullptr;
        }

        auto* tree_node = new TreeNode {curr_nary_node->val};
        if (parent_nary_node != nullptr)
        {
            if (std::ssize(parent_nary_node->children) > 1)
            {
                //! Left child of each TreeNode is next sibling of curr NaryNode
                tree_node->left =
                    encode(parent_nary_node->children[1], parent_nary_node);
            }
        }

        if (!curr_nary_node->children.empty())
        {
            //! Right child of each TreeNode is first child of current NaryNode
            tree_node->right =
                encode(curr_nary_node->children[0], curr_nary_node);
        }
    }

    //! Encodes an n-ary tree to a binary tree
    //! @pre LC handles deletion of dynamically allocated memory
    TreeNode* encode(NaryNode* root)
    {
        return encode(nullptr, root, 0);
        if (root == nullptr)
        {
            return nullptr;
        }

        auto* tree_node = new TreeNode {root->val};
        if (root->children.empty())
        {
            return tree_node;
        }

        //! Right child of each TreeNode is first child of NaryNode
        tree_node->right = encode(root->children[0]);

        //! Left child of each TreeNode is the next sibling of NaryNode
        for (int sibling = 1; sibling < std::ssize(root->children); ++sibling)
        {
            tree_node->left = encode(root->children[sibling]);
        }

        return tree_node;
    }

    //! @brief Recursive helper function to decode binary tree to an n-ary tree
    //! @param[in, out] root   Pointer to TreeNode
    //! @param[in, out] parent Pointer to NaryNode that is current node's parent
    //! @return Pointer to the root NaryNode
    NaryNode* decode(TreeNode* root, NaryNode* parent)
    {
        if (root == nullptr)
        {
            return nullptr;
        }

        auto* node = new NaryNode {root->val};
        if (root->left != nullptr)
        {
            //! Add TreeNode left child as next child of NaryNode's parent
            parent->children.emplace_back(decode(root->left, parent));
        }

        if (root->right != nullptr)
        {
            //! Add TreeNode right child as first child of NaryNode
            node->children.insert(node->children.begin(),
                                  decode(root->right, ));
        }
    }

    //! Decodes a binary tree to an n-ary tree
    //! @pre LC handles deletion of dynamically allocated memory
    NaryNode* decode(TreeNode* root)
    {
        if (root == nullptr)
        {
            return nullptr;
        }

        auto* node = new NaryNode {root->val};
        if (root->left != nullptr)
        {
            //! Add TreeNode left child as next child of NaryNode's parent
            node->children.emplace_back(decode(root->left));
        }

        if (root->right != nullptr)
        {
            //! Add TreeNode right child as first child of NaryNode
            node->children.insert(node->children.begin(), decode(root->right));
        }

        return node;
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
    NaryNode two {2};
    NaryNode six {6};
    NaryNode ten {10};
    NaryNode twelve {12};
    NaryNode thirteen {13};
    NaryNode fourteen {14};

    NaryNode eight {8, std::vector<NaryNode*> {&twelve}};
    NaryNode nine {9, std::vector<NaryNode*> {&thirteen}};
    NaryNode eleven {11, std::vector<NaryNode*> {&fourteen}};

    NaryNode four {4, std::vector<NaryNode*> {&eight}};
    NaryNode seven {7, std::vector<NaryNode*> {&eleven}};

    NaryNode three {3, std::vector<NaryNode*> {&six, &seven}};
    NaryNode five {5, std::vector<NaryNode*> {&nine, &ten}};

    NaryNode one {1, std::vector<NaryNode*> {&two, &three, &four, &five}};

    CodecFA    codec_fa;
    const auto root_fa = codec_fa.decode(codec_fa.encode(&one));
    EXPECT_NE(nullptr, root_fa);
    EXPECT_EQ(one.val, root_fa->val);
    EXPECT_EQ(on.children, root_fa->children);
}

TEST(CodecTest, SampleTest3)
{
    CodecFA codec_fa;
    EXPECT_EQ(nullptr, codec_fa.decode(codec_fa.encode(nullptr)));
}
