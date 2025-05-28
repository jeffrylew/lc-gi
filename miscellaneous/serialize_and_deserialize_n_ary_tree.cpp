#include "NaryNode.hpp"

#include <cctype>
#include <charconv>
#include <iterator>
#include <queue>
#include <ranges>
#include <string>
#include <string_view>
#include <system_error>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std::string_view_literals;

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
//!          For every node, we add 3 values to the serialized string. When
//!          deserializing, the first loop is O(3 * N) and second loop is O(N).
//!          Space complexity O(N). The space occupied by the serialize_node
//!          helper is through the recursion stack and the final string, which
//!          usually isn't considered but its size is not fixed in this case.
//!          Deserialization has a space complexity of O(N) for the hash map and
//!          queue.
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
        if (data.empty())
        {
            return nullptr;
        }

        return deserialize_string(data);
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

    //! @brief Deserialize comma-delimited string_view of serialized tree
    //! @param[in] serialized_tree Comma-delimited std::string_view of tree
    //! @return Pointer to root NaryNode
    NaryNode* deserialize_string(std::string_view serialized_tree)
    {
        //! Map of <unique node ID, <unique parent ID, current NaryNode*>>
        std::unordered_map<int, std::pair<int, NaryNode*>> ids_nodes {};

        //! Queue of <parent ID, child ID>
        std::queue<std::pair<int, int>> parent_child_ids {};

        //! https://stackoverflow.com/questions/14265581
        //!     /parse-split-a-string-in-c-using-string-delimiter-standard-c
        auto ids_and_node_vals =
            serialized_tree
                | std::ranges::views::split(","sv)
                | std::ranges::views::transform([](auto&& str) {
                    return std::string_view(str.data(),
                                            std::ranges::distance(str));
                });

        //! Index of current element in ids_and_node_vals view
        int element_idx {};

        //! Unique ID of current node
        int node_id {};

        //! Value of current node
        int node_val {};

        //! String value converted to an int
        int string_val {};

        for (auto&& id_or_val : ids_and_node_vals)
        {
            auto [ptr, ec] =
                std::from_chars(id_or_val.data(),
                                id_or_val.data() + id_or_val.size(),
                                string_val);

            if (ec == std::errc())
            {
                const int ids_nodes_subidx {element_idx % 3};
                if (ids_nodes_subidx == 0)
                {
                    node_id = string_val;
                }
                else if (ids_nodes_subidx == 1)
                {
                    node_val = string_val;
                }
                else
                {
                    auto* curr_node = new NaryNode {node_val};

                    //! string_val = Parent ID here
                    ids_nodes.try_emplace(node_id, string_val, curr_node);
                    parent_child_ids.emplace(string_val, node_id);
                }

                ++element_idx;
            }
            else
            /*
            else if (ec == std::errc::invalid_argument
                     || ec == std::errc::result_out_of_range)
             */
            {
                //! Should not encounter this
                return nullptr;
            }
        }

        //! parent_child_ids should not be empty
        const auto [root_parent_id, root_id] = parent_child_ids.front();
        parent_child_ids.pop();

        //! Second pass over parent_child_ids queue to connect parent to child
        while (!parent_child_ids.empty())
        {
            const auto [parent_id, child_id] = parent_child_ids.front();
            parent_child_ids.pop();

            auto* child_node  = ids_nodes[child_id].second;
            auto* parent_node = ids_nodes[parent_id].second;

            parent_node->children.push_back(child_node);
        }

        return ids_nodes[root_id].second;
    }
};

//! @class CodecDS2
//! @brief DFS with children sizes discussion solution
//! @details https://leetcode.com/problems/serialize-and-deserialize-n-ary-tree
//!
//!          Time complexity O(N) where N = number of nodes in the N-ary tree.
//!          For every node, we add 3 values to the serialized string. When
//!          deserializing, the first loop is O(3 * N) and second loop is O(N).
//!          Space complexity O(N). The space occupied by the serialize_node
//!          helper is through the recursion stack and the final string, which
//!          usually isn't considered but its size is not fixed in this case.
//!          Deserialization has a space complexity of O(N) for hash map/queue.
class CodecDS2
{
public:
    //! Encodes a tree to a single string
    std::string serialize(NaryNode* root)
    {
        std::string serialized_tree;

        serialize_node(root, serialized_tree);

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
        if (data.empty())
        {
            return nullptr;
        }

        //! https://stackoverflow.com/questions/14265581
        //!     /parse-split-a-string-in-c-using-string-delimiter-standard-c
        auto val_num_children =
            serialized_tree
                | std::ranges::views::split(","sv)
                | std::ranges::views::transform([](auto&& str) {
                    return std::string_view(str.data(),
                                            std::ranges::distance(str));
                });

        return deserialize_string(data, 0);
    }

private:
    //! @brief Add node value and number of children to str with comma delimiter
    //! @param[in]  node Pointer to current NaryNode
    //! @param[out] str  Output serialized string
    void serialize_node(NaryNode* node, std::string& str)
    {
        if (node == nullptr)
        {
            return;
        }

        //! Add node value
        str += std::to_string(node->val) + ',';

        //! Add number of children
        str += std::to_string(std::ssize(node->children)) + ',';

        for (auto* child : node->children)
        {
            serialize_node(child, str);
        }
    }

    //! @brief Deserialize comma-delimited string_view of serialized tree
    //! @param[in] serialized_tree_view View of elements from serialized tree
    //! @param[in] index                Index keeps track of processed chars 
    //! @return Pointer to root NaryNode
    NaryNode* deserialize_string(auto& serialized_tree_view, int index)
    {
        if (index == std::ssize(serialized_tree))
        {
            return nullptr;
        }

        //! Node value or num children string converted to an int
        int node_val_or_num_children {};

        auto curr_node_view = serialized_tree_view.begin() + index;
        if (std::from_chars(curr_node_view,
                            curr_node_view + curr_node_view.size(),
                            node_val_or_num_children).ec
            != std::errc {})
        {
            return nullptr;
        }

        //! Invariant here is that index points to a node and index + 1 is
        //! the number of children it has
        auto* node = new NaryNode(node_val_or_num_children);

        ++index;
        auto num_children_view = serialized_tree_view.begin() + index;
        if (std::from_chars(num_children_view,
                            num_children_view + num_children_view.size(),
                            node_val_or_num_children).ec
            != std::errc {})
        {
            return nullptr;
        }

        for (int child = 0; child < node_val_or_num_children; ++child)
        {
            ++index;
            node->children.push_back(
                deserialize_string(serialized_tree_view, index));
        }

        return node;
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

     /*
      * Serialization of tree
      * 1,1,0,
      * 2,2,1,
      * 3,3,1,
      * 4,6,3,
      * 5,7,3,
      * 6,11,5,
      * 7,14,6,
      * 8,4,1,
      * 9,8,8,
      * 10,12,9,
      * 11,5,1,
      * 12,9,11,
      * 13,13,12,
      * 14,10,11
      */
    CodecDS1   codec_ds1;
    const auto root_ds1 = codec_ds1.deserialize(codec_ds1.serialize(&one));
    EXPECT_NE(nullptr, root_ds1);
    EXPECT_EQ(one.val, root_ds1->val);
    EXPECT_EQ(one.children, root_ds1->children);

    CodecDS2   codec_ds2;
    const auto root_ds2 = codec_ds2.deserialize(codec_ds2.serialize(&one));
    EXPECT_NE(nullptr, root_ds2);
    EXPECT_EQ(one.val, root_ds2->val);
    EXPECT_EQ(one.children, root_ds2->children);
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

    CodecDS2   codec_ds2;
    const auto root_ds2 = codec_ds2.deserialize(codec_ds2.serialize(&one));
    EXPECT_NE(nullptr, root_ds2);
    EXPECT_EQ(one.val, root_ds2->val);
    EXPECT_EQ(one.children, root_ds2->children);
}

TEST(CodecSerializeDeserializeTest, SampleTest3)
{
    CodecFA codec_fa;
    EXPECT_EQ(nullptr, codec_fa.deserialize(codec_fa.serialize(nullptr)));

    CodecDS1 codec_ds1;
    EXPECT_EQ(nullptr, codec_ds1.deserialize(codec_ds1.serialize(nullptr)));

    CodecDS2 codec_ds2;
    EXPECT_EQ(nullptr, codec_ds2.deserialize(codec_ds2.serialize(nullptr)));
}
