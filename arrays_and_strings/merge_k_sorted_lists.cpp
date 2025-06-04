#include "../linked_lists/ListNode.hpp"

#include <gtest/gtest.h>

#include <algorithm>
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

//! @brief Brute force discussion solution
//! @pre LC handles deallocation of memory.
//! @param[in, out] lists Reference to vector of k sorted linked-lists
//! @return Pointer to ListNode head of sorted linked list
static ListNode* mergeKListsDS1(std::vector<ListNode*>& lists)
{
    //! @details https://leetcode.com/problems/merge-k-sorted-lists/editorial/
    //!
    //!          Time complexity O(N * log N) where N = total number of nodes.
    //!          Collecting all values costs O(N). A stable sorting algorithm
    //!          costs O(N * log N) and iterating to create the linked list
    //!          costs O(N).
    //!          Space complexity O(log N) on average for std::sort (quicksort).
    //!          Creating a new linked list costs O(N).

    ListNode head {0};
    auto*    curr_node = &head;

    std::vector<int> node_vals;
    for (auto* list_node : lists)
    {
        while (list_node != nullptr)
        {
            node_vals.push_back(list_node->val);
            list_node = list_node->next;
        }
    }

    std::sort(node_vals.begin(), node_vals.end());
    for (const int node_val : node_vals)
    {
        curr_node->next = new ListNode(node_val);
        curr_node       = curr_node->next;
    }

    return head.next;
}

//! @brief Compare one by one using priority queue discussion solution
//! @pre LC handles deallocation of memory.
//! @param[in, out] lists Reference to vector of k sorted linked-lists
//! @return Pointer to ListNode head of sorted linked list
static ListNode* mergeKListsDS2(std::vector<ListNode*>& lists)
{
    //! @details https://leetcode.com/problems/merge-k-sorted-lists/editorial/
    //!
    //!          Time complexity O(N * log k) where N = total number of nodes
    //!          and k = number of linked lists. The comparison cost is O(log k)
    //!          for every pop and insertion to the priority queue. Finding the
    //!          node with the smallest value costs O(1). There are N nodes in
    //!          the final linked list.
    //!          Space complexity O(N) for creating a new linked list and O(k)
    //!          for the priority queue initially. This implementation uses an
    //!          in-place method, which costs O(1) after the initial O(k).

    auto cmp = [](ListNode* lhs, ListNode* rhs) { return lhs->val > rhs->val; };

    std::priority_queue<ListNode*, std::vector<ListNode*>, decltype(cmp)>
        node_min_heap(cmp);

    for (auto* list_head : lists)
    {
        if (list_head != nullptr)
        {
            node_min_heap.push(list_head);
        }
    }

    ListNode head {0};
    auto*    curr_node = &head;

    while (!node_min_heap.empty())
    {
        curr_node->next = node_min_heap.top();
        node_min_heap.pop();

        curr_node = curr_node->next;

        if (curr_node->next != nullptr)
        {
            node_min_heap.push(curr_node->next);
        }
    }

    return head.next;
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

    auto* root_ds1 = mergeKListsDS1(lists);
    EXPECT_NE(root_ds1, nullptr);
    EXPECT_EQ(1, root_ds1->val);
    EXPECT_NE(root_ds1->next, nullptr);
    EXPECT_EQ(1, root_ds1->next->val);

    /*
    auto* root_ds2 = mergeKListsDS2(lists);
    EXPECT_NE(root_ds2, nullptr);
    EXPECT_EQ(1, root_ds2->val);
    EXPECT_NE(root_ds2->next, nullptr);
    EXPECT_EQ(1, root_ds2->next->val);
     */
}

TEST(MergeKListsTest, SampleTest2)
{
    std::vector<ListNode*> lists {};

    auto* root_fa = mergeKListsFA(lists);
    EXPECT_EQ(root_fa, nullptr);

    auto* root_ds1 = mergeKListsDS1(lists);
    EXPECT_EQ(root_ds1, nullptr);

    /*
    auto* root_ds2 = mergeKListsDS2(lists);
    EXPECT_EQ(root_ds2, nullptr);
     */
}

TEST(MergeKListsTest, SampleTest3)
{
    std::vector<ListNode*> lists {nullptr};

    auto* root_fa = mergeKListsFA(lists);
    EXPECT_EQ(root_fa, nullptr);

    auto* root_ds1 = mergeKListsDS1(lists);
    EXPECT_EQ(root_ds1, nullptr);

    /*
    auto* root_ds2 = mergeKListsDS2(lists);
    EXPECT_EQ(root_ds2, nullptr);
     */
}
