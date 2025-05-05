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
    //! @pre LC handles deletion of dynamically allocated memory
    //! @param[in, out] curr_nary_node   Pointer to current NaryNode
    //! @param[in, out] parent_nary_node Pointer to parent of current NaryNode
    //! @param[in]      curr_idx         Index of current NaryNode in parent
    //! @return Pointer to TreeNode
    [[nodiscard]] TreeNode* encode(NaryNode* curr_nary_node,
                                   NaryNode* parent_nary_node,
                                   int       curr_idx)
    {
        if (curr_nary_node == nullptr)
        {
            return nullptr;
        }

        auto* tree_node = new TreeNode {curr_nary_node->val};
        if (parent_nary_node != nullptr
            && std::ssize(parent_nary_node->children) > curr_idx + 1)
        {
            //! Left child of each TreeNode is next sibling of curr NaryNode
            tree_node->left = encode(parent_nary_node->children[curr_idx + 1],
                                     parent_nary_node,
                                     curr_idx + 1);
        }

        if (!curr_nary_node->children.empty())
        {
            //! Right child of each TreeNode is first child of current NaryNode
            tree_node->right = encode(curr_nary_node->children[0],
                                      curr_nary_node,
                                      curr_idx);
        }

        return tree_node;
    }

    //! Encodes an n-ary tree to a binary tree
    //! @pre LC handles deletion of dynamically allocated memory
    TreeNode* encode(NaryNode* root)
    {
        return encode(root, nullptr, 0);
    }

    //! @brief Recursive helper function to decode binary tree to an n-ary tree
    //! @pre LC handles deletion of dynamically allocated memory
    //! @param[in, out] curr_tree_node   Pointer to current TreeNode
    //! @param[in, out] parent_nary_node Pointer to parent of current NaryNode
    //! @return Pointer to NaryNode
    [[nodiscard]] NaryNode*
        decode(TreeNode* curr_tree_node, NaryNode* parent_nary_node)
    {
        if (curr_tree_node == nullptr)
        {
            return nullptr;
        }

        auto* nary_node = new NaryNode {curr_tree_node->val};
        if (curr_tree_node->left != nullptr && parent_nary_node != nullptr)
        {
            //! Add TreeNode left child as next child of NaryNode's parent
            parent_nary_node->children.emplace_back(
                decode(curr_tree_node->left, parent_nary_node));
        }

        if (curr_tree_node->right != nullptr)
        {
            //! Add TreeNode right child as first child of NaryNode
            nary_node->children.insert(
                nary_node->children.begin(),
                decode(curr_tree_node->right, nary_node));
        }

        return nary_node;
    }

    //! Decodes a binary tree to an n-ary tree
    //! @pre LC handles deletion of dynamically allocated memory
    NaryNode* decode(TreeNode* root)
    {
        return decode(root, nullptr);
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
