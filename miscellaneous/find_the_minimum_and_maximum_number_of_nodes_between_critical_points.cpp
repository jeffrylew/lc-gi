#include "ListNode.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <limits>
#include <vector>

//! @brief First attempt to get min and max distances between two critical pts
//! @param[in] head Pointer to head of linked list
//! @return Vector containing {minDistance, maxDistance} or {-1, -1} if < 2 pts
static std::vector<int> nodesBetweenCriticalPointsFA(ListNode* head)
{
    //! @details https://leetcode.com/problems/
    //!     find-the-minimum-and-maximum-number-of-nodes-between-critical-points
    //!
    //!          Time complexity O(N) where N = number of nodes in linked list
    //!          Space complexity O(1) for a few ints and pointers

    if (head->next->next == nullptr)
    {
        return {-1, -1};
    }

    int first_crit_pos {};
    int last_crit_pos {};
    int num_nodes {1};

    int min_dist {std::numeric_limits<int>::max()};
    int max_dist {std::numeric_limits<int>::min()};

    ListNode* prev = head;
    ListNode* curr = head->next;

    while (curr != nullptr)
    {
        ++num_nodes;

        if (curr->next == nullptr)
        {
            //! curr cannot be a critical point
            break;
        }

        if ((curr->val < prev->val && curr->val < curr->next->val)
            || (curr->val > prev->val && curr->val > curr->next->val))
        {
            //! curr is a critical point
            if (first_crit_pos == 0)
            {
                first_crit_pos = num_nodes;
            }
            else
            {
                min_dist = std::min(min_dist, num_nodes - last_crit_pos);
                max_dist = std::max(max_dist, num_nodes - first_crit_pos);
            }

            last_crit_pos = num_nodes;
        }

        prev = curr;
        curr = curr->next;
    }

    if (first_crit_pos == last_crit_pos)
    {
        return {-1, -1};
    }

    return {min_dist, max_dist};

} // static std::vector<int> nodesBetweenCriticalPointsFA( ...

TEST(NodesBetweenCriticalPointsTest, SampleTest1)
{
    const ListNode         one {1};
    const ListNode         three {3, &one};
    const std::vector<int> expected_output {-1, -1};

    EXPECT_EQ(expected_output, nodesBetweenCriticalPointsFA(&three));
}

TEST(NodesBetweenCriticalPointsTest, SampleTest2)
{
    const ListNode         two_2 {2};
    const ListNode         one_2 {1, &two_2};
    const ListNode         five_2 {5, &one_2};
    const ListNode         two_1 {2, &five_2};
    const ListNode         one_1 {1, &two_1};
    const ListNode         three {3, &one_1};
    const ListNode         five_1 {5, &three};
    const std::vector<int> expected_output {1, 3};

    EXPECT_EQ(expected_output, nodesBetweenCriticalPointsFA(&five_1));
}
