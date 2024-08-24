#include "TreeNode.hpp"

#include <gtest/gtest.h>

//! @brief First attempt to merge two sorted linked lists into one sorted list
//! @param[in] list1 Head of first sorted linked list
//! @param[in] list2 Head of second sorted linked list
//! @return Head of the merged linked list
static ListNode* mergeTwoListsFA(ListNode* list1, ListNode* list2)
{
    //! @details https://leetcode.com/problems/merge-two-sorted-lists
    //!
    //!          Time complexity O(M + N) where M = number of nodes in list1 and
    //!          N = number of nodes in list2.
    //!          Space complexity O(1)

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
            prev1 = list1;

            if (next1 == nullptr ||  list2->val < next1->val)
            {
                list1->next = list2;
            }

            list1 = next1;
        }
        else
        {
            prev2 = list2;

            if (next2 == nullptr || list1->val <= next2->val)
            {
                list2->next = list1;
            }

            list2 = next2;
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

TEST(MergeTwoListsTest, SampleTest1)
{
    ListNode four_1 {4};
    ListNode two {2, &four_1};
    ListNode one_1 {1, &two};

    ListNode four_2 {4};
    ListNode three {3, &four_2};
    ListNode one_2 {1, &three};

    const ListNode out_four_2 {4};
    const ListNode out_four_1 {4, &out_four_2};
    const ListNode out_three {3, &out_four_1};
    const ListNode out_two {2, &out_three};
    const ListNode out_one_2 {1, &out_two};
    const ListNode out_one_1 {1, &out_one_2};

    auto* result          = mergeTwoListsFA(&one_1, &one_2);
    auto* expected_output = &out_one_1;

    while (result != nullptr && expected_output != nullptr)
    {
        EXPECT_EQ(result->val, expected_output->val);
        result          = result->next;
        expected_output = expected_output->next;
    }
}

TEST(MergeTwoListsTest, SampleTest2)
{
    EXPECT_EQ(nullptr, mergeTwoListsFA(nullptr, nullptr));
}

TEST(MergeTwoListsTest, SampleTest3)
{
    const ListNode zero {0};

    EXPECT_EQ(zero->val, mergeTwoListsFA(nullptr, &zero)->val);
}
