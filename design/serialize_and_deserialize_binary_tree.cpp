#include "../miscellaneous/TreeNode.hpp"

#include <gtest/gtest.h>

#include <charconv>
#include <queue>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>

//! @class CodecFA
//! @brief First attempt solution to serialize and deserialize a binary tree
//! @details https://leetcode.com/explore/interview/card/google/65/design-4/3092
//!
//!          First attempt solution does not pass SampleTest3.
class CodecFA
{
    static constexpr auto delim        = std::string_view {","};
    static constexpr auto null_node_sv = std::string_view {"10001"};
    static constexpr int  null_node_val {10001};

public:
    //! @brief Encodes a tree to a single string
    std::string serialize(TreeNode* root)
    {
        if (root == nullptr)
        {
            return {};
        }

        std::string serialized_tree;
    
        std::queue<TreeNode*> node_queue {};
        node_queue.push(root);

        while (!node_queue.empty())
        {
            const auto node = node_queue.front();
            node_queue.pop();

            if (node == nullptr)
            {
                serialized_tree += null_node_sv;
                serialized_tree += delim;
                continue;
            }

            serialized_tree += std::to_string(node->val);
            serialized_tree += delim;
            node_queue.push(node->left);
            node_queue.push(node->right);
        }

        //! Remove final trailing comma
        serialized_tree.pop_back();
        return serialized_tree;
    }

    //! @brief Decodes your encoded data to a tree
    //! @pre LC handles memory deallocation
    TreeNode* deserialize(std::string data)
    {
        if (data.empty())
        {
            return nullptr;
        }

        //! Map of <Index, TreeNode*>
        std::unordered_map<int, TreeNode*> node_map {};
        int                                node_idx {};

        std::string_view data_sv {data};

        for (const auto node_view : std::views::split(data_sv, delim))
        {
            int  node_val {};
            auto node_sv = std::string_view {node_view};

            if (std::from_chars(node_sv.data(),
                                node_sv.data() + node_sv.size(),
                                node_val).ec
                == std::errc {})
            {
                if (node_val == null_node_val)
                {
                    node_map[node_idx] = nullptr;
                }
                else
                {
                    node_map[node_idx] = new TreeNode {node_val};
                }

                if (node_idx % 2 == 1)
                {
                    //! If node_idx is odd then it is a left child
                    const int parent_idx {node_idx / 2};
                    if (node_map[parent_idx] != nullptr)
                    {
                        node_map[parent_idx]->left = node_map[node_idx];
                    }
                }
                else
                {
                    //! If node_idx is even then it is a right child (or root)
                    const int parent_idx {(node_idx - 2) / 2};
                    if (parent_idx > -1 && node_map[parent_idx] != nullptr)
                    {
                        node_map[parent_idx]->right = node_map[node_idx];
                    }
                }

                ++node_idx;
            }
        }

        return node_map[0];
    }
};

//! @class CodecDS1
//! @brief DFS discussion solution to serialize and deserialize a binary tree
//! @details https://leetcode.com/problems/serialize-and-deserialize-binary-tree
//!
//!          Time complexity O(N) where N = number of nodes in the tree. In both
//!          the serialization and deserialization functions, we visit each node
//!          exactly once.
//!          Space complexity O(N). In both serialization and deserialization,
//!          we keep the entire tree either at the beginning or at the end.
class CodecDS1
{
    static constexpr auto delim        = std::string_view {","};
    static constexpr auto null_node_sv = std::string_view {"10001"};
    static constexpr int  null_node_val {10001};

public:
    //! @brief Encodes a tree to a single string
    std::string serialize(TreeNode* root)
    {
        std::string serialized_tree;
        serialize_tree(root, serialized_tree);

        //! Remove final trailing comma
        serialized_tree.pop_back();

        return serialized_tree;
    }

    //! @brief Decodes your encoded data to a tree
    //! @pre LC handles memory deallocation
    TreeNode* deserialize(std::string data)
    {
        std::queue<std::string_view> node_sv_queue {};
        std::string_view             data_sv {data};

        for (const auto node_view : std::views::split(data_sv, delim))
        {
            node_sv_queue.emplace(node_view);
        }

        return deserialize_tree(node_sv_queue);
    }

private:
    //! @brief Recursive serialization helper
    //! @param[in]  node     Pointer to TreeNode
    //! @param[out] tree_str Reference to string storing serialized tree
    void serialize_tree(TreeNode* node, std::string& tree_str)
    {
        if (node == nullptr)
        {
            tree_str.append(null_node_sv);
            tree_str.append(delim);
        }
        else
        {
            tree_str += std::to_string(node->val);
            tree_str.append(delim);

            serialize_tree(node->left, tree_str);
            serialize_tree(node->right, tree_str);
        }
    }

    //! @brief Recursive deserialization helper
    //! @param[in, out] node_sv_queue Reference to queue of node string_views
    [[nodiscard]] TreeNode*
        deserialize_tree(std::queue<std::string_view>& node_sv_queue)
    {
        const auto node_sv = node_sv_queue.front();
        node_sv_queue.pop();

        if (node_sv == null_node_sv)
        {
            return nullptr;
        }

        int node_val {};
        if (std::from_chars(node_sv.data(),
                            node_sv.data() + node_sv.size(),
                            node_val).ec
            == std::errc {})
        {
            auto* root  = new TreeNode {node_val};
            root->left  = deserialize_tree(node_sv_queue);
            root->right = deserialize_tree(node_sv_queue);
            return root;
        }

        return nullptr;
    }
};

TEST(CodecSerializeDeserializeBinaryTreeTest, SampleTest1)
{
    TreeNode two {2};
    TreeNode four {4};
    TreeNode five {5};

    TreeNode three {3, &four, &five};
    TreeNode one {1, &two, &three};

    CodecFA    codec_fa;
    const auto root_fa = codec_fa.deserialize(codec_fa.serialize(&one));
    EXPECT_NE(nullptr, root_fa);
    EXPECT_NE(nullptr, root_fa->left);
    EXPECT_NE(nullptr, root_fa->right);
    EXPECT_EQ(one.val, root_fa->val);
    EXPECT_EQ(one.left->val, root_fa->left->val);
    EXPECT_EQ(one.right->val, root_fa->right->val);

    CodecDS1   codec_ds1;
    const auto root_ds1 = codec_ds1.deserialize(codec_ds1.serialize(&one));
    EXPECT_NE(nullptr, root_ds1);
    EXPECT_NE(nullptr, root_ds1->left);
    EXPECT_NE(nullptr, root_ds1->right);
    EXPECT_EQ(one.val, root_ds1->val);
    EXPECT_EQ(one.left->val, root_ds1->left->val);
    EXPECT_EQ(one.right->val, root_ds1->right->val);
}

TEST(CodecSerializeDeserializeBinaryTreeTest, SampleTest2)
{
    CodecFA codec_fa;
    EXPECT_EQ(nullptr, codec_fa.deserialize(codec_fa.serialize(nullptr)));

    CodecFA codec_ds1;
    EXPECT_EQ(nullptr, codec_ds1.deserialize(codec_ds1.serialize(nullptr)));
}

TEST(CodecSerializeDeserializeBinaryTreeTest, SampleTest3)
{
    TreeNode two {2};
    TreeNode five {5};
    TreeNode six {6};
    TreeNode seven {7};

    TreeNode four {4, &six, &seven};
    TreeNode three {3, &four, &five};
    TreeNode one {1, &two, &three};

    /*
     * Output:   1, 2, 3, null, null, 4, 5
     * Expected: 1, 2, 3, null, null, 4, 5, 6, 7
    CodecFA codec_fa;
     */

    CodecDS1   codec_ds1;
    const auto root_ds1 = codec_ds1.deserialize(codec_ds1.serialize(&one));
    EXPECT_NE(nullptr, root_ds1);
    EXPECT_NE(nullptr, root_ds1->left);
    EXPECT_NE(nullptr, root_ds1->right);
    EXPECT_EQ(one.val, root_ds1->val);
    EXPECT_EQ(one.left->val, root_ds1->left->val);
    EXPECT_EQ(one.right->val, root_ds1->right->val);
}
