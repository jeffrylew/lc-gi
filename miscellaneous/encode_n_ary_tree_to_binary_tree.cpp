#include "NaryNode.hpp"
#include "TreeNode.hpp"

#include <gtest/gtest.h>

#include <queue>
#include <utility>
#include <vector>

//! @class CodecFA
//! @brief First attempt solution to encode an n-ary tree into a binary tree and
//!        decode the binary tree back to the original n-ary tree
//! @details leetcode.com/explore/learn/card/n-ary-tree/131/recursion/920
//!          Used leetcode.com/explore/learn/card/n-ary-tree/131/recursion/922
//!          Solution has UB due to incrementing nullptr somewhere.
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

//! @class CodecDS1
//! @brief BFS discussion solution
//! @details leetcode.com/problems/encode-n-ary-tree-to-binary-tree/editorial
//!
//!          Time complexity O(N) where N = number of nodes in the N-ary tree.
//!          We traverse each node in the tree once and only once.
//!          Space complexity O(L) where L = max number of nodes that reside in
//!          the same level. Since L is proportional to N in the worst case, we
//!          can generalize to O(N). We use a queue for level by level traversal
//!          and at any moment, the queue contains nodes that are at most spread
//!          into two levels. Assuming the max number of nodes at one level is L
//!          then the size of the queue would be less than 2L at any time.
class CodecDS1
{
public:
    //! Encodes an n-ary tree to a binary tree
    TreeNode* encode(NaryNode* root)
    {
        if (root == nullptr)
        {
            return nullptr;
        }

        auto* new_root = new TreeNode {root->val};

        std::queue<std::pair<TreeNode*, NaryNode*>> node_queue;
        node_queue.emplace(new_root, root);

        while (!node_queue.empty())
        {
            auto [curr_tree_node, curr_nary_node] = node_queue.front();
            node_queue.pop();

            //! Encode the child NaryNodes into a singly linked list of TreeNode
            TreeNode* prev_tree_node {nullptr};
            TreeNode* head_tree_node {nullptr};
            for (const auto& child_nary_node : curr_nary_node->children)
            {
                auto* new_tree_node = new TreeNode {child_nary_node->val};

                if (prev_tree_node == nullptr)
                {
                    head_tree_node = new_tree_node;
                }
                else
                {
                    prev_tree_node->right = new_tree_node;
                }

                prev_tree_node = new_tree_node;

                node_queue.emplace(new_tree_node, child_nary_node);
            }

            //! Attach the singly linked list of children to the left node
            curr_tree_node->left = head_tree_node;
        }

        return new_root;
    }

    //! Decodes a binary tree to an n-ary tree
    NaryNode* decode(TreeNode* root)
    {
        if (root == nullptr)
        {
            return nullptr;
        }

        auto* new_root = new NaryNode {root->val};

        std::queue<std::pair<NaryNode*, TreeNode*>> node_queue;
        node_queue.emplace(new_root, root);

        while (!node_queue.empty())
        {
            auto [curr_nary_node, curr_tree_node] = node_queue.front();
            node_queue.pop();

            //! Decode the children list (curr_tree_node->left is first child)
            TreeNode* sibling = curr_tree_node->left;
            while (sibling != nullptr)
            {
                auto* child_nary_node = new NaryNode {sibling->val};
                curr_nary_node->children.emplace_back(child_nary_node);

                node_queue.emplace(child_nary_node, sibling);
                sibling = sibling->right;
            }
        }

        return new_root;
    }
};

//! @class CodecDS2
//! @brief DFS discussion solution
//! @details leetcode.com/problems/encode-n-ary-tree-to-binary-tree/editorial
//!
//!          Time complexity O(N) where N = number of nodes in the N-ary tree.
//!          We traverse each node in the tree once and only once.
//!          Space complexity O(D) where D = depth of the N-ary tree. In the
//!          worst case, we can generalize to O(N). The size of the call stack
//!          at any moment is exactly the level where the current node resides.
class CodecDS2
{
public:
    //! Encodes an n-ary tree to a binary tree
    TreeNode* encode(NaryNode* root)
    {
        if (root == nullptr)
        {
            return nullptr;
        }

        auto* new_root = new TreeNode {root->val};

        //! Encode the first child NaryNode as the left TreeNode of binary tree
        if (!root->children.empty())
        {
            auto* first_child_nary_node = root->children.front();
            new_root->left              = encode(first_child_nary_node);
        }

        //! Encode the rest of the sibling nodes (new_root->left is first child)
        auto* sibling_tree_node = new_root->left;
        for (int child_idx = 1;
             child_idx < std::ssize(root->children);
             ++child_idx)
        {
            sibling_tree_node->right = encode(root->children[child_idx]);
            sibling_tree_node        = sibling_tree_node->right;
        }

        return new_root;
    }

    //! Decodes a binary tree to an n-ary tree
    NaryNode* decode(TreeNode* root)
    {
        if (root == nullptr)
        {
            return nullptr;
        }

        auto* new_root = new NaryNode {root->val};

        //! Decode all children nodes
        auto* sibling_tree_node = root->left;
        while (sibling_tree_node != nullptr)
        {
            new_root->children.push_back(decode(sibling_tree_node));
            sibling_tree_node = sibling_tree_node->right;
        }

        return new_root;
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

    /*
    CodecFA    codec_fa;
    const auto root_fa = codec_fa.decode(codec_fa.encode(&one));
    EXPECT_NE(nullptr, root_fa);
    EXPECT_EQ(one.val, root_fa->val);
    EXPECT_EQ(one.children, root_fa->children);
     */

    CodecDS1   codec_ds1;
    const auto root_ds1 = codec_ds1.decode(codec_ds1.encode(&one));
    EXPECT_NE(nullptr, root_ds1);
    EXPECT_EQ(one.val, root_ds1->val);
    EXPECT_EQ(one.children, root_ds1->children);

    CodecDS2   codec_ds2;
    const auto root_ds2 = codec_ds2.decode(codec_ds2.encode(&one));
    EXPECT_NE(nullptr, root_ds2);
    EXPECT_EQ(one.val, root_ds2->val);
    EXPECT_EQ(one.children, root_ds2->children);
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

    /*
    CodecFA    codec_fa;
    const auto root_fa = codec_fa.decode(codec_fa.encode(&one));
    EXPECT_NE(nullptr, root_fa);
    EXPECT_EQ(one.val, root_fa->val);
    EXPECT_EQ(on.children, root_fa->children);
     */

    CodecDS1   codec_ds1;
    const auto root_ds1 = codec_ds1.decode(codec_ds1.encode(&one));
    EXPECT_NE(nullptr, root_ds1);
    EXPECT_EQ(one.val, root_ds1->val);
    EXPECT_EQ(one.children, root_ds1->children);

    CodecDS2   codec_ds2;
    const auto root_ds2 = codec_ds2.decode(codec_ds2.encode(&one));
    EXPECT_NE(nullptr, root_ds2);
    EXPECT_EQ(one.val, root_ds2->val);
    EXPECT_EQ(one.children, root_ds2->children);
}

TEST(CodecTest, SampleTest3)
{
    CodecFA codec_fa;
    EXPECT_EQ(nullptr, codec_fa.decode(codec_fa.encode(nullptr)));

    CodecDS1 codec_ds1;
    EXPECT_EQ(nullptr, codec_ds1.decode(codec_ds1.encode(nullptr)));

    CodecDS2 codec_ds2;
    EXPECT_EQ(nullptr, codec_ds2.decode(codec_ds2.encode(nullptr)));
}
