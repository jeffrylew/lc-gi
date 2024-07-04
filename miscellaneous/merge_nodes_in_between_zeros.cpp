#include <gtest/gtest.h>

//! @struct ListNode
//! @brief Definition for singly-linked list
struct ListNode
{
    int       val {};
    ListNode* next {nullptr};

    ListNode() = default;

    ListNode(int x)
        : val {x}
    {
    }

    ListNode(int x, ListNode* next_node)
        : val {x}
        , next {next_node}
    {
    }
};

//! @brief First attempt to merge nodes between zero nodes in singly-linked list
//! @param[in] head Pointer to head of singly-linked list
//! @return Head of modified linked list
static ListNode* mergeNodesFA(ListNode* head)
{
    //! @details https://leetcode.com/problems/merge-nodes-in-between-zeros
    //!
    //!          Time complexity O(N) where N = number of input nodes
    //!          Space complexity O(N) in the worst case

    ListNode* head_out = new ListNode();
    ListNode* curr     = head_out;

    int curr_sum {};

    while (head != nullptr)
    {
        if (head->val == 0)
        {
            head = head->next;

            if (curr_sum != 0)
            {
                curr->next = new ListNode(curr_sum);
                curr = curr->next;
            }

            curr_sum = 0;
            continue;
        }

        curr_sum += head->val;
        head = head->next;
    }

    return head_out->next;

} // static ListNode* mergeNodesFA( ...

//! @brief Two-pointer discussion solution
//! @param[in] head Pointer to head of singly-linked list
//! @return Head of modified linked list
static ListNode* mergeNodesDS1(ListNode* head)
{
    //! @details https://leetcode.com/problems/merge-nodes-in-between-zeros
    //!
    //!          Time complexity O(N) where N = size of linked list. All nodes
    //!          of linked list are visited exactly once.
    //!          Space complexity O(1). No additional space is used.

    //! Initialize a sentinel/dummy node with the first non-zero value
    ListNode* modify   = head->next;
    ListNode* next_sum = modify;

    while (next_sum != nullptr)
    {
        int sum {};

        //! Find the sum of all nodes until a zero is encountered
        while (next_sum != nullptr && next_sum->val != 0)
        {
            sum += next_sum->val;
            next_sum = next_sum->next;
        }

        //! Assign sum to current node's value
        modify->val = sum;

        //! Move next_sum to first non-zero value of next block
        next_sum = next_sum->next;

        //! Also move modify to this node
        modify->next = next_sum;
        modify = modify->next;
    }

    return head->next;

} // static ListNode* mergeNodesDS1( ...

//! @brief Recursion discussion solution
//! @param[in] head Pointer to head of singly-linked list
//! @return Head of modified linked list
static ListNode* mergeNodesDS2(ListNode* head)
{
    //! @details https://leetcode.com/problems/merge-nodes-in-between-zeros
    //!
    //!          Time complexity O(N) where N = size of linked list. All nodes
    //!          are visited exactly once.
    //!          Space complexity O(N). Recursion could go up to N levels deep.

    // Start with first non-zero value
    head = head->next;
    if (head == nullptr)
    {
        return head;
    }

    //! Initialize a dummy head node
    ListNode* temp = head;
    int       sum {};

    while (temp != nullptr && temp->val != 0)
    {
        sum += temp->val;
        temp = temp->next;
    }

    //! Store the sum in head's value
    head->val = sum;

    //! Store head's next node as recursive result for temp node
    head->next = mergeNodesDS2(temp);

    return head;

} // static ListNode* mergeNodesDS2( ...

TEST(MergeNodesTest, SampleTest1)
{
    //! Output nodes starting with four
    const ListNode eleven {11};
    const ListNode four {4, &eleven};
    auto*          expected_output = &four;

    //! Input nodes starting with zero_1
    const ListNode zero_3 {0};
    const ListNode two {2, &zero_3};
    const ListNode five {5, &two};
    const ListNode four {4, &five};
    const ListNode zero_2 {0, &four};
    const ListNode one {1, &zero_2};
    const ListNode three {3, &one};
    const ListNode zero_1 {0, &three};

    auto* head_in  = &zero_1;
    auto* head_out = mergeNodesFA(head_in);

    while (head_out != nullptr && expected_output != nullptr)
    {
        EXPECT_EQ(head_out->val, expected_output->val);

        expected_output = expected_output->next;

        //! Shouldn't allocate and delete memory manually but it is LC's setup
        auto* prev = head_out;
        head_out   = head_out->next;
        delete prev;
    }
}
