#include "ListNode.hpp"

#include <gtest/gtest.h>

//! @brief First attempt to remove nth node from end of list and return head
//! @param[in] head Pointer to head of linked list
//! @param[in] n    Number of node from end to remove
//! @return Pointer to head of list after removal
static ListNode* removeNthFromEndFA(ListNode* head, int n)
{
    //! @details https://leetcode.com/problems/remove-nth-node-from-end-of-list
    //!
    //!          Time complexity O(N) where N = number of nodes in list
    //!          Space complexity O(1)

    ListNode* prev {nullptr};
    ListNode* curr {nullptr};
    ListNode* n_ahead = head;

    while (n > 0 && n_ahead != nullptr)
    {
        n_ahead = n_ahead->next;
        --n;
    }

    if (n_ahead == nullptr)
    {
        if (head->next != nullptr)
        {
            return head->next;
        }
        return nullptr;
    }

    curr = head;
    while (n_ahead != nullptr)
    {
        n_ahead = n_ahead->next;
        prev    = curr;
        curr    = curr->next;
    }

    if (curr == nullptr)
    {
        prev->next = nullptr;
        return prev;
    }
    if (prev != nullptr)
    {
        prev->next = curr->next;
    }
    curr = nullptr;

    return head;

} // static ListNode* removeNthFromEndFA( ...

//! @brief Two pass algorithm discussion solution
//! @param[in] head Pointer to head of linked list
//! @param[in] n    Number of node from end to remove
//! @return Pointer to head of list after removal
static ListNode* removeNthFromEndDS1(ListNode* head, int n)
{
    //! @details https://leetcode.com/problems/remove-nth-node-from-end-of-list
    //!
    //!          Time complexity O(L). Algorithm makes two traversals of the
    //!          list. First to calculate its length L and second to find the
    //!          (L - n)th node. There are 2L - n operations.
    //!          Space complexity O(1)

    ListNode dummy {0};
    dummy.next = head;

    int   length {};
    auto* first = head;

    while (first != nullptr)
    {
        ++length;
        first = first->next;
    }

    lenth -= n;
    first = &dummy;

    while (length > 0)
    {
        --length;
        first = first->next;
    }

    first->next = first->next->next;
    return dummy->next;

} // static ListNode* removeNthFromEndDS1( ...

//! @brief One pass algorithm discussion solution
//! @param[in] head Pointer to head of linked list
//! @param[in] n    Number of node from end to remove
//! @return Pointer to head of list after removal
static ListNode* removeNthFromEndDS2(ListNode* head, int n)
{
    //! @details https://leetcode.com/problems/remove-nth-node-from-end-of-list
    //!
    //!          Time complexity O(L). The algorithm makes one traversal of the
    //!          list of L nodes.
    //!          Space complexity O(1)

    ListNode dummy {0, head};

    auto* first  = &dummy;
    auto* second = &dummy;

    //! Advance first pointer so gap between first and second is n nodes
    //! Assume first is not nullptr for these iterations
    for (int pos = 1; pos <= n + 1; ++pos)
    {
        first = first->next;
    }

    //! Move first to the end, maintaining the gap
    while (first != nullptr)
    {
        first  = first->next;
        second = second->next;
    }

    second->next = second->next->next;
    return dummy.next;

} // static ListNode* removeNthFromEndDS2( ...

TEST(RemoveNthFromEndTest, SampleTest2)
{
    ListNode one {1};

    EXPECT_EQ(nullptr, removeNthFromEndFA(&one, 1));
    EXPECT_EQ(nullptr, removeNthFromEndDS1(&one, 1));
    EXPECT_EQ(nullptr, removeNthFromEndDS2(&one, 1));
}

TEST(RemoveNthFromEndTest, SampleTest3)
{
    ListNode two {2};
    ListNode one {1, &two};

    ListNode expected_one {1};

    EXPECT_EQ(expected_one->val, removeNthFromEndFA(&one, 1)->val);
    EXPECT_EQ(expected_one->val, removeNthFromEndDS1(&one, 1)->val);
    EXPECT_EQ(expected_one->val, removeNthFromEndDS2(&one, 1)->val);
}
