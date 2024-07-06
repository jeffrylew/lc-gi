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

//! @brief One pass discussion solution
//! @param[in] head Pointer to head of linked list
//! @return Vector containing {minDistance, maxDistance} or {-1, -1} if < 2 pts
static std::vector<int> nodesBetweenCriticalPointsDS(ListNode* head)
{
    //! @details https://leetcode.com/problems/
    //!     find-the-minimum-and-maximum-number-of-nodes-between-critical-points
    //!
    //!          Time complexity O(N) where N = number of nodes in linked list.
    //!          The algorithm traverses the list only once.
    //!          Space complexity O(1), algorithm does not utilize any
    //!          additional data structures.

    std::vector<int> result {-1, -1};

    //! Initialize min distance
    int min_distance {std::numeric_limits<int>::max()};

    //! Pointers to track the previous node, current node, and indices
    ListNode* previous_node = head;
    ListNode* current_node  = head->next;

    int current_index {1};
    int previous_critical_index {};
    int first_critical_index {};

    while (current_node->next != nullptr)
    {
        //! Check if current node is a local maxima or minima
        if ((current_node->val < previous_node->val
             && current_node->val < current_node->next->val)
            || (current_node->val > previous_node->val
                && current_node->val > current_node->next->val))
        {
            if (previous_critical_index == 0)
            {
                //! This is the first critical point found
                previous_critical_index = current_index;
                first_critical_index    = current_index;
            }
            else
            {
                //! Calculate the minimum distance between critical points
                min_distance = std::min(current_index - previous_critical_index,
                                        min_distance);
                previous_critical_index = current_index;
            }
        }

        //! Move to the next node and update indices
        ++current_index;
        previous_node = current_node;
        current_node  = current_node->next;
    }

    //! If at least two critical points were found
    if (min_distance != std::numeric_limits<int>::max())
    {
        const int max_distance {previous_critical_index - first_critical_index};
        result = {min_distance, max_distance};
    }

    return result;

} // static std::vector<int> nodesBetweenCriticalPointsDS( ...

TEST(NodesBetweenCriticalPointsTest, SampleTest1)
{
    const ListNode         one {1};
    const ListNode         three {3, &one};
    const std::vector<int> expected_output {-1, -1};

    EXPECT_EQ(expected_output, nodesBetweenCriticalPointsFA(&three));
    EXPECT_EQ(expected_output, nodesBetweenCriticalPointsDS(&three));
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
    EXPECT_EQ(expected_output, nodesBetweenCriticalPointsDS(&five_1));
}
