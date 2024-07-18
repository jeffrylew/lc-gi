#include "TreeNode.hpp"

#include <gtest/gtest.h>

#include <tuple>
#include <unordered_map>
#include <unordered_set>

//! @brief First attempt to create tree descriptions and return its root
//! @param[in] descriptions Vec descriptions_i = {parent_i, child_i, isLeft_i}
//! @return Root of binary tree described by descriptions
static TreeNode* createBinaryTreeFA(
    const std::vector<std::vector<int>>& descriptions)
{
    //! @details https://leetcode.com/problems
    //!          /create-binary-tree-from-descriptions

    std::unordered_map<int, TreeNode*> node_map {};
    std::unordered_set<int>            children {};

    for (const auto& desc : descriptions)
    {
        auto parent_ret = node_map.emplace(desc[0], new TreeNode(desc[0]));
        auto child_ret  = node_map.emplace(desc[1], new TreeNode(desc[1]));

        auto* parent_node = parent_ret.first->second;
        auto* child_node  = child_ret.first->second;

        std::ignore = children.emplace(desc[1]);

        if (desc[2] == 0)
        {
            parent_node->right = child_node;
        }
        else
        {
            parent_node->left = child_node;
        }
    }

    for (const auto& [val, node] : node_map)
    {
        if (!children.contains(val))
        {
            return node;
        }
    }

    return nullptr;

} // static TreeNode* createBinaryTreeFA( ...
