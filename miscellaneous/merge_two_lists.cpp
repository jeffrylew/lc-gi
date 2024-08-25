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

//! @brief Recursive discussion solution
//! @param[in] list1 Head of first sorted linked list
//! @param[in] list2 Head of second sorted linked list
//! @return Head of the merged linked list
static ListNode* mergeTwoListsDS1(ListNode* list1, ListNode* list2)
{
    //! @details https://leetcode.com/problems/merge-two-sorted-lists
    //!
    //!          Recursively define merge as following (ignore empty list cases)
    //!          list1[0] + merge(list1[1:], list2) for list1[0] < list2[0]
    //!          list2[0] + merge(list1, list2[1:]) otherwise
    //!
    //!          Time complexity O(N + M) where N = number of nodes in list1 and
    //!          M = number of nodes in list2. Each recursive call increments
    //!          the pointer to list1 or list2 by one so there will be exactly
    //!          one call to mergeTwoLists per element in each list.
    //!          Space complexity O(N + M). The first call to mergeTwoLists does
    //!          not return until the ends of both list1 and list2 have been
    //!          reached so N + M stack frames use O(N + M) space.

    if (list1 == nullptr)
    {
        return list2;
    }

    if (list2 == nullptr)
    {
        return list1;
    }

    //! Determine which of list1 or list2 has a smaller head and recursively set
    //! the next value of that head to the next merge result. Both lists are
    //! null-terminated so the recursion will eventually terminate

    if (list1->val < list2->val)
    {
        list1->next = mergeTwoListsDS1(list1->next, list2);
        return list1;
    }

    list2->next = mergeTwoLists(list1, list2->next);
    return list2;

} // static ListNode* mergeTwoListsDS1( ...

//! @brief Iterative discussion solution
//! @param[in] list1 Head of first sorted linked list
//! @param[in] list2 Head of second sorted linked list
//! @return Head of the merged linked list
static ListNode* mergeTwoListsDS2(ListNode* list1, ListNode* list2)
{
    //! @details https://leetcode.com/problems/merge-two-sorted-lists
    //!
    //!          Time complexity O(N + M). Exactly one of l1 and l2 is
    //!          incremented on each loop iteration so the while loop runs for
    //!          a number of iterations equal to sum of lengths of two lists.
    //!          All other work is constant.
    //!          Space complexity O(1). The iterative approach only allocates a
    //!          few pointers.

    //! Maintain an unchanging reference to node ahead of return node
    ListNode prehead {-1};
    auto*    prev = &prehead;

    while (l1 != nullptr && l2 != nullptr)
    {
        if (l1->val <= l2->val)
        {
            prev->next = l1;
            l1         = l1->next;
        }
        else
        {
            prev->next = l2;
            l2         = l2->next;
        }

        prev = prev->next;
    }

    //! l1 or l2 can still have nodes so connect non-null list to end of merge
    prev->next = l1 == nullptr ? l2 : l1;
    return prehead.next;

} // static ListNode* mergeTwoListsDS2( ...

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
    EXPECT_EQ(nullptr, mergeTwoListsDS1(nullptr, nullptr));
    EXPECT_EQ(nullptr, mergeTwoListsDS2(nullptr, nullptr));
}

TEST(MergeTwoListsTest, SampleTest3)
{
    const ListNode zero {0};

    EXPECT_EQ(zero->val, mergeTwoListsFA(nullptr, &zero)->val);
    EXPECT_EQ(zero->val, mergeTwoListsDS1(nullptr, &zero)->val);
    EXPECT_EQ(zero->val, mergeTwoListsDS2(nullptr, &zero)->val);
}
