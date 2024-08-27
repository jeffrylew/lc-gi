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

TEST(RemoveNthFromEndTest, SampleTest2)
{
    ListNode one {1};

    EXPECT_EQ(nullptr, removeNthFromEndFA(&one, 1));
}

TEST(RemoveNthFromEndTest, SampleTest3)
{
    ListNode two {2};
    ListNode one {1, &two};

    ListNode expected_one {1};

    EXPECT_EQ(expected_one->val, removeNthFromEndFA(&one, 1)->val);
}
