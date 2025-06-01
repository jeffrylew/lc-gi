#include "../linked_lists/ListNode.hpp"

#include <gtest/gtest.h>

#include <queue>
#include <vector>

//! @brief First attempt to merge k sorted linked-lists into one sorted list
//! @param[in, out] lists Reference to vector of k sorted linked-lists
//! @return Pointer to ListNode head of sorted linked list
static ListNode* mergeKListsFA(std::vector<ListNode*>& lists)
{
    //! @details https://leetcode.com/explore/interview/card/google/59
    //!          /array-and-strings/342
    //!
    //!          Time complexity O(N * k * log k) where N = average nodes per
    //!          linked-list and k = number of sorted linked lists. Initially,
    //!          we push the head of each linked list onto the min heap which
    //!          costs O(k * log k). Then, for the remaining (N - 1) * k nodes,
    //!          we pop the current node and push its next node if not nullptr
    //!          in O(2 * log k) = O(log k). The size of the min heap is k since
    //!          we remove a node but add its next node (unless it is nullptr).
    //!          Thus, the overall time complexity is O(N * k * log k).
    //!          Space complexity O(k) for the min heap/priority queue.

    if (lists.empty())
    {
        return nullptr;
    }

    //! Sentinel node that will point to head of sorted linked list
    ListNode sentinel_head {-10001};

    //! Pointer to current ListNode in sorted linked list
    ListNode* curr_list_node {nullptr};

    auto node_cmp = [](ListNode* lhs, ListNode* rhs) {
        return lhs->val > rhs->val;
    };
    std::priority_queue<ListNode*, std::vector<ListNode*>, decltype(node_cmp)>
        node_min_heap(node_cmp);

    for (auto* list_head : lists)
    {
        if (list_head == nullptr)
        {
            continue;
        }

        node_min_heap.push(list_head);
    }

    while (!node_min_heap.empty())
    {
        auto* min_node = node_min_heap.top();
        node_min_heap.pop();

        if (min_node->next != nullptr)
        {
            node_min_heap.push(min_node->next);
        }

        if (curr_list_node == nullptr)
        {
            curr_list_node = min_node;
        }
        else
        {
            curr_list_node->next = min_node;
            curr_list_node       = curr_list_node->next;
        }

        if (sentinel_head.next == nullptr)
        {
            sentinel_head.next = curr_list_node;
        }
    }

    return sentinel_head.next;
}

TEST(MergeKListsTest, SampleTest1)
{
    ListNode l1_five {5};
    ListNode l1_four {4, &l1_five};
    ListNode l1_one {1, &l1_four};

    ListNode l2_four {4};
    ListNode l2_three {3, &l2_four};
    ListNode l2_one {1, &l2_three};

    ListNode l3_six {6};
    ListNode l3_two {2, &l3_six};

    std::vector<ListNode*> lists {&l1_one, &l2_one, &l3_two};

    auto* root_fa = mergeKListsFA(lists);
    EXPECT_NE(root_fa, nullptr);
    EXPECT_EQ(1, root_fa->val);
    EXPECT_NE(root_fa->next, nullptr);
    EXPECT_EQ(1, root_fa->next->val);
}

TEST(MergeKListsTest, SampleTest2)
{
    std::vector<ListNode*> lists {};

    auto* root_fa = mergeKListsFA(lists);
    EXPECT_EQ(root_fa, nullptr);
}

TEST(MergeKListsTest, SampleTest3)
{
    std::vector<ListNode*> lists {nullptr};

    auto* root_fa = mergeKListsFA(lists);
    EXPECT_EQ(root_fa, nullptr);
}
