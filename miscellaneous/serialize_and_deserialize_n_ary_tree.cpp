#include "NaryNode.hpp"

#include <cctype>
#include <queue>
#include <string>
#include <vector>

//! @class CodecFA
//! @brief First attempt solution to serialize and deserialize an N-ary tree
//! @details leetcode.com/explore/learn/card/n-ary-tree/132/conclusion/924
//!
//!          Time complexity O(N) where N = number of nodes in the N-ary tree.
//!          We traverse each node in the tree once.
//!          Space complexity O(N) for the node_queue. The output string of
//!          serialize also uses O(N).
//!
//!          First attempt solution does not pass SampleTest1.
class CodecFA
{
public:
    //! Encodes a tree to a single string
    std::string serialize(NaryNode* root)
    {
        /*
        if (root == nullptr)
        {
            return {};
        }

        std::string tree {"[" + std::to_string(root->val)};

        if (root->children.empty())
        {
            return tree;
        }

        tree += serialize(root->children[0]);
        for (int child_idx = 1;
             child_idx < std::ssize(root->children);
             ++child_idx)
        {
            tree += serialize(root->children[child_idx]);
        }

        tree += "]";

        return tree;
        */

        if (root == nullptr)
        {
            return {};
        }

        std::string           tree;
        std::queue<NaryNode*> node_queue {};
        node_queue.push(root);

        while (!node_queue.empty())
        {
            const auto num_nodes_in_level =
                static_cast<int>(std::ssize(node_queue));

            for (int node_idx = 0; node_idx < num_nodes_in_level; ++node_idx)
            {
                const auto node = node_queue.front();
                node_queue.pop();

                if (node == nullptr)
                {
                    tree += "\0,";
                }
                else
                {
                    tree += std::to_string(node->val) + ",";

                    for (auto* child : node->children)
                    {
                        node_queue.push(child);
                    }

                    node_queue.push(nullptr);
                }
            }

            tree += "\0,";
        }

        //! Remove last character while it is non-numerical
        while (!tree.empty() && !std::isdigit(tree.back()))
        {
            tree.pop_back();
        }
    
        return tree;
    }

    //! Decodes your encoded data to a tree
    //! @pre LC handles memory deallocation
    NaryNode* deserialize(std::string data)
    {
        if (data.empty())
        {
            return nullptr;
        }

        auto* root = new NaryNode(static_cast<int>(data[0] - '0'));

        std::queue<NaryNode*> node_queue {};
        node_queue.push(root);

        NaryNode* parent {nullptr};

        //! Start from idx = 2. If tree has more than one element then next
        //! number or null char will be at index 2
        for (int idx = 2; idx < std::ssize(data); ++idx)
        {
            if (data[idx] == ',')
            {
                continue;
            }

            if (data[idx] == '\0' && !node_queue.empty())
            {
                parent = node_queue.front();
                node_queue.pop();
                continue;
            }

            auto* node = new NaryNode(static_cast<int>(data[idx] - '0'));
            node_queue.push(node);
            if (parent != nullptr)
            {
                //! parent nullptr check prevents UB sanitizer warning but I
                //! don't think it should be needed.
                parent->children.push_back(node);
            }
        }

        return root;
    }
};

//! @class CodecDS1
//! @brief Parent child relationships discussion solution
//! @details https://leetcode.com/problems/serialize-and-deserialize-n-ary-tree
//!
//!          Time complexity O(N) where N = number of nodes in the N-ary tree.
class CodecDS1
{
public:
    //! Encodes a tree to a single string
    std::string serialize(NaryNode* root)
    {
        std::string serialized_tree;

        //! Unique node IDs begin at 1. Root node has a parent ID of 0.
        int node_id {1};
        serialize_node(root, serialized_tree, node_id, 0);

        if (!serialized_tree.empty())
        {
            //! If N-ary tree has at least one element, serialized_tree will end
            //! with an extra comma so remove it
            serialized_tree.pop_back();
        }

        return serialized_tree;
    }

    //! Decodes your encoded data to a tree
    //! @pre LC handles memory deallocation
    NaryNode* deserialize(std::string data)
    {
        //! @todo
    }

private:
    //! @brief Add node ID, node value, parent ID to str with comma delimiters
    //! @param[in]      node      Pointer to current NaryNode
    //! @param[out]     str       Output serialized string
    //! @param[in, out] node_id   Unique ID of current node
    //! @param[in]      parent_id Unique ID of parent node
    void serialize_node(NaryNode*    node,
                        std::string& str,
                        int&         node_id,
                        int          parent_id)
    {
        if (node == nullptr)
        {
            return;
        }

        //! Add node ID
        str += std::to_string(node_id) + ',';

        //! Add node value
        str += std::to_string(node->val) + ',';

        //! Add parent ID
        str += std::to_string(parent_id) + ',';

        //! Set parent_id to current node_id
        parent_id = node_id;

        for (auto* child : node->children)
        {
            ++node_id;
            serialize_node(child, str, node_id, parent_id);
        }
    }
};

TEST(CodecSerializeDeserializeTest, SampleTest1)
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
    const auto root_fa = codec_fa.deserialize(codec_fa.serialize(&one));
    EXPECT_NE(nullptr, root_fa);
    EXPECT_EQ(one.val, root_fa->val);
    EXPECT_EQ(one.children, root_fa->children);
     */

     CodecDS1   codec_ds1;
     const auto root_ds1 = codec_ds1.deserialize(codec_ds1.serialize(&one));
     EXPECT_NE(nullptr, root_ds1);
     EXPECT_EQ(one.val, root_ds1->val);
     EXPECT_EQ(one.children, root_ds1->children);
}

TEST(CodecSerializeDeserializeTest, SampleTest2)
{
    NaryNode two {2};
    NaryNode four {4};
    NaryNode five {5};
    NaryNode six {6};

    NaryNode three {3, std::vector<NaryNode*> {&five, &six}};
    NaryNode one {1, std::vector<NaryNode*> {&three, &two, &four}};

    /*
    CodecFA    codec_fa;
    const auto root_fa = codec_fa.deserialize(codec_fa.serialize(&one));
    EXPECT_NE(nullptr, root_fa);
    EXPECT_EQ(one.val, root_fa->val);
    EXPECT_EQ(one.children, root_fa->children);
     */

     CodecDS1   codec_ds1;
     const auto root_ds1 = codec_ds1.deserialize(codec_ds1.serialize(&one));
     EXPECT_NE(nullptr, root_ds1);
     EXPECT_EQ(one.val, root_ds1->val);
     EXPECT_EQ(one.children, root_ds1->children);
}

TEST(CodecSerializeDeserializeTest, SampleTest3)
{
    CodecFA codec_fa;
    EXPECT_EQ(nullptr, codec_fa.deserialize(codec_fa.serialize(nullptr)));

    CodecDS1 codec_ds1;
    EXPECT_EQ(nullptr, codec_ds1.deserialize(codec_ds1.serialize(nullptr)));
}
