#include "ListNode.hpp"

#include <gtest/gtest.h>

static ListNode* removeNthFromEndFA(ListNode* head, int n)
{
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
        return nullptr;
    }

    curr = head;
    while (n_ahead != nullptr)
    {
        n_ahead = n_ahead->next;
        prev    = curr;
        curr    = curr->next;
    }

    if (prev != nullptr)
    {
        prev->next = curr->next;
    }
    delete curr;

    return head;

} // static ListNode* removeNthFromEndFA( ...
