#include "NaryNode.hpp"

#include <queue>
#include <string>
#include <vector>

//! @class CodecFA
//! @brief First attempt solution to serialize and deserialize an N-ary tree
//! @details leetcode.com/explore/learn/card/n-ary-tree/132/conclusion/924
//!
//!          Time complexity O(N) where N = number of nodes in the N-ary tree.
//!          We traverse each node in the tree once.
//!          Space complexity O().
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
    
        std::string           tree("[");
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

                tree += std::to_string(node->val);

                for (const auto* child : node->children)
                {

                }
            }

            tree += '\0';
        }
    }

    //! Decodes your encoded data to a tree
    NaryNode* deserialize(std::string data)
    {
        
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

    CodecFA    codec_fa;
    const auto root_fa = codec_fa.deserialize(codec_fa.serialize(&one));
    EXPECT_NE(nullptr, root_fa);
    EXPECT_EQ(one.val, root_fa->val);
    EXPECT_EQ(one.children, root_fa->children);
}

TEST(CodecSerializeDeserializeTest, SampleTest2)
{
    NaryNode two {2};
    NaryNode four {4};
    NaryNode five {5};
    NaryNode six {6};

    NaryNode three {3, std::vector<NaryNode*> {&five, &six}};
    NaryNode one {1, std::vector<NaryNode*> {&three, &two, &four}};

    CodecFA    codec_fa;
    const auto root_fa = codec_fa.deserialize(codec_fa.serialize(&one));
    EXPECT_NE(nullptr, root_fa);
    EXPECT_EQ(one.val, root_fa->val);
    EXPECT_EQ(one.children, root_fa->children);
}

TEST(CodecSerializeDeserializeTest, SampleTest3)
{
    CodecFA codec_fa;
    EXPECT_EQ(nullptr, codec_fa.deserialize(codec_fa.serialize(nullptr)));
}
