#include "TreeNode.hpp"

#include <gtest/gtest.h>

//! @brief Merge two sorted linked lists into one sorted list
//! @param[in] list1 Head of first sorted linked list
//! @param[in] list2 Head of second sorted linked list
//! @return Head of the merged linked list
static ListNode* mergeTwoListsFA(ListNode* list1, ListNode* list2)
{
    if (list1 == nullptr && list2 == nullptr)
    {
        return nullptr;
    }

    if (list1 == nullptr)
    {
        return list2;
    }

    if (list2 == nullptr)
    {
        return list1;
    }

    auto* head = list1->val <= list2->val ? list1 : list2;

    ListNode* prev1 {nullptr};
    ListNode* prev2 {nullptr};

    while (list1 != nullptr && list2 != nullptr)
    {
        const int val1 {list1->val};
        const int val2 {list2->val};

        auto* next1 = list1->next;
        auto* next2 = list2->next;

        if (val1 <= val2)
        {
            prev1       = list1;
            list1->next = list2;
            list1       = next1;
        }
        else
        {
            prev2       = list2;
            list2->next = list1;
            list2       = next2;
        }
    }

    if (list2 != nullptr)
    {
        prev1->next = list2;
    }

    if (list1 != nullptr)
    {
        prev2->next = list1;
    }

    return head;

} // static ListNode* mergeTwoListsFA( ...
