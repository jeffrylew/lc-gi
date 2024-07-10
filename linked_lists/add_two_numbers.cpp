#include "../miscellaneous/ListNode.hpp"

#include <gtest/gtest.h>

//! @brief First attempt to add two numbers where digits are stored in reverse
//! @param[in] l1 First linked list where digits are stored in reverse order
//! @param[in] l2 Second linked list where digits are stored in reverse order
//! @return Linked list containing sum of two numbers
static ListNode* addTwoNumbersFA(ListNode* l1, ListNode* l2)
{
    //! @details https://leetcode.com/problems/add-two-numbers/description/
    //!
    //!          Time complexity O(M + N) where M = length of linked list l1 and
    //!          N = length of linked list l2
    //!          Space complexity O(1)

    int   carryover {};
    auto* l1_prev = l1;
    auto* l1_curr = l1;

    while (l1_curr != nullptr && l2 != nullptr)
    {
        int curr_sum {l1_curr->val + l2->val};
        if (carryover > 0)
        {
            curr_sum += carryover;
            carryover = 0;
        }

        l1_curr->val = curr_sum % 10;
        carryover    = curr_sum / 10;

        l1_prev = l1_curr;
        l1_curr = l1_curr->next;
        l2      = l2->next;
    }

    if (l1_curr == nullptr && l2 == nullptr)
    {
        if (carryover > 0)
        {
            //! @note Memory allocation with new is expected
            l1_prev->next = new ListNode(carryover);
        }
    }
    else if (l1_curr == nullptr)
    {
        l1_prev->next = l2;
    }

    while (l2 != nullptr)
    {
        int curr_digit {l2->val};
        if (carryover > 0)
        {
            curr_digit += carryover;
            carryover = 0;
        }

        l2->val   = curr_digit % 10;
        carryover = curr_digit / 10;

        if (l2->next == nullptr && carryover > 0)
        {
            //! @note Memory allocation with new is expected
            l2->next = new ListNode(carryover);
            break;
        }

        l2 = l2->next;
    }

    while (l1_curr != nullptr)
    {
        int curr_digit {l1_curr->val};
        if (carryover > 0)
        {
            curr_digit += carryover;
            carryover = 0;
        }

        l1_curr->val = curr_digit % 10;
        carryover    = curr_digit / 10;

        if (l1_curr->next == nullptr && carryover > 0)
        {
            //! @note Memory allocation with new is expected
            l1_curr->next = new ListNode(carryover);
            break;
        }

        l1_curr = l1_curr->next;
    }

    return l1;

} // static ListNode* addTwoNumbersFA( ...

//! @brief Elementary math discussion solution
//! @param[in] l1 First linked list where digits are stored in reverse order
//! @param[in] l2 Second linked list where digits are stored in reverse order
//! @return Linked list containing sum of two numbers
static ListNode* addTwoNumbersDS(ListNode* l1, ListNode* l2)
{
    //! @details https://leetcode.com/problems/add-two-numbers/description/
    //!
    //!          Time complexity O(max(M, N)) where M = length of l1 and N =
    //!          length of l2. The algorithm iterates at most max(M, N) times.
    //!          Space complexity O(1). The length of the new list is at most
    //!          max(M, N) + 1. However, we don't count the answer as part of
    //!          the space complexity.

    ListNode* dummy_head = new ListNode(0);
    auto*     curr       = dummy_head;

    int carry {};

    while (l1 != nullptr || l2 != nullptr || carry != 0)
    {
        const int x {l1 != nullptr ? l1->val : 0};
        const int y {l2 != nullptr ? l2->val : 0};
        const int sum {carry + x + y};

        carry      = sum / 10;
        curr->next = new ListNode(sum % 10);
        curr       = curr->next;

        l1 = (l1 != nullptr) ? l1->next : nullptr;
        l2 = (l2 != nullptr) ? l2->next : nullptr;
    }

    auto* result = dummy_head->next;

    //! Free memory allocated for dummy_head
    delete dummy_head;

    return result;

} // static ListNode* addTwoNumbersDS( ...

TEST(AddTwoNumbersTest, SampleTest1)
{
    //! Memory allocation with new is undesired but required
    const ListNode* three  = new ListNode(3);
    ListNode*       four_1 = new ListNode(4, three);
    ListNode*       two    = new ListNode(2, four_1);

    const ListNode* four_2 = new ListNode(4);
    ListNode*       six    = new ListNode(6, four_2);
    ListNode*       five   = new ListNode(5, six);

    const ListNode eight {8};
    const ListNode zero {0, &eight};
    const ListNode seven {7, &zero};
    ListNode*      expected_output_head {&seven};

    auto* head = addTwoNumbersFA(two, five);
    auto* prev = head;

    while (head != nullptr && expected_output_head != nullptr)
    {
        EXPECT_EQ(head->val, expected_output_head->val);

        //! Update prev for memory deallocation
        prev = head;

        head                 = head->next;
        expected_output_head = expected_output_head->next;

        //! Deallocate prev
        delete prev;
    }

    expected_output_head = &seven;
    head                 = addTwoNumbersDS(two, five);
    prev                 = head;

    while (head != nullptr && expected_output_head != nullptr)
    {
        EXPECT_EQ(head->val, expected_output_head->val);

        //! Update prev for memory deallocation
        prev = head;

        head                 = head->next;
        expected_output_head = expected_output_head->next;

        //! Deallocate prev
        delete prev;
    }

    //! Deallocate memory from first list
    while (two != nullptr)
    {
        prev = two;
        two  = two->next;
        delete prev;
    }

    //! Deallocate memory from second list
    while (five != nullptr)
    {
        prev = five;
        five = five->next;
        delete prev;
    }
}
