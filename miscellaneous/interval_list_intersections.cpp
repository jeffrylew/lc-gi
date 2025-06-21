#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

//! @brief First attempt to get intersection of two interval lists
//! @param[in] firstList  Reference to first vector of closed intervals
//! @param[in] secondList Reference to second vector of closed intervals
//! @return Intersection of two interval lists
static std::vector<std::vector<int>>
    intervalIntersectionFA(const std::vector<std::vector<int>>& firstList,
                           const std::vector<std::vector<int>>& secondList)
{
    //! @details https://leetcode.com/problems/interval-list-intersections
    //!
    //!          Time complexity O(max(M, N)) where M = firstList.size() and
    //!          N = secondList.size(). In the worst case, one must iterate
    //!          through all elements in the larger list.
    //!          Space complexity O(1), excluding output intersections vector.

    std::vector<std::vector<int>> intersections {};

    if (firstList.empty() || secondList.empty())
    {
        return intersections;
    }

    const auto first_size  = static_cast<int>(std::ssize(firstList));
    const auto second_size = static_cast<int>(std::ssize(secondList));

    int first_idx {};
    int second_idx {};

    while (first_idx < first_size && second_idx < second_size)
    {
        const auto& first_interval  = firstList[first_idx];
        const auto& second_interval = secondList[second_idx];

        //! Let first_interval = [A, B] and second_interval = [C, D]
        //! Cases using conditionals A < B; A == B; C < D; C == D
        //!   - A < B and C < D
        //!     - B < C: [1, 2]; [3, 4] -> []
        //!     - B == C: [1, 2]; [2, 4] -> [2, 2]
        //!     - B > C
        //!       - A < C: [1, 3]; [2, 4] -> [2, 3]
        //!       - A == C: [2, 3]; [2, 4] -> [2, 3]
        //!       - A > C: [2, 3]; [1, 4] -> [2, 3]
        //!     - B == D
        //!       - A < C: [1, 4]; [2, 4] -> [2, 4]
        //!       - A == C: [2, 4]; [2, 4] -> [2, 4]
        //!       - A > C: [3, 4]; [2, 4] -> [3, 4]
        //!     - B > D
        //!       - A < C: [1, 4]; [2, 3] -> [2, 3]
        //!       - A == C: [2, 4]; [2, 3] -> [2, 3]
        //!       - A > C: [3, 4]; [2, 3] -> [3, 3]
        //!   - A < B and C == D
        //!       - OMG so many cases...
        //!   - A == B and C < D
        //!   - A == B and C == D

        const int max_start {std::max(first_interval[0], second_interval[0])};
        const int min_end {std::min(first_interval[1], second_interval[1])};
    
        if (max_start > min_end)
        {
            //! No intersection, increment index for interval of smaller values
            (first_interval[1] < second_interval[0])
                ? ++first_idx
                : ++second_idx;
            continue;
        }

        intersections.push_back({max_start, min_end});
    
        if (first_interval[1] == second_interval[1])
        {
            ++first_idx;
            ++second_idx;
        }
        else if (first_interval[1] < second_interval[1])
        {
            ++first_idx;
        }
        else
        {
            ++second_idx;
        }
    }

    (first_idx == first_size) ? ++second_idx : ++first_idx;

    while (first_idx < first_size)
    {
        const auto& first_interval = firstList[first_idx];
        const int   second_start {secondList.back()[0]};
        const int   second_end {secondList.back()[1]};

        const int max_start {std::max(first_interval[0], second_start)};
        const int min_end {std::min(first_interval[1], second_end)};

        if (max_start > min_end)
        {
            break;
        }

        intersections.push_back({max_start, min_end});

        if (first_interval[1] < second_end)
        {
            ++first_idx;
        }
        else
        {
            break;
        }
    }

    while (second_idx < second_size)
    {
        const auto& second_interval = secondList[second_idx];
        const int   first_start {firstList.back()[0]};
        const int   first_end {firstList.back()[1]};

        const int max_start {std::max(first_start, second_interval[0])};
        const int min_end {std::min(first_end, second_interval[1])};

        if (max_start > min_end)
        {
            break;
        }

        intersections.push_back({max_start, min_end});

        if (second_interval[1] < first_end)
        {
            ++second_idx;
        }
        else
        {
            break;
        }
    }

    return intersections;

} // static std::vector<std::vector<int>> intervalIntersectionFA( ...

TEST(IntervalIntersectionTest, SampleTest1)
{
    const std::vector<std::vector<int>> first_list {
        {0, 2}, {5, 10}, {13, 23}, {24, 25}};
    const std::vector<std::vector<int>> second_list {
        {1, 5}, {8, 12}, {15, 24}, {25, 26}};
    const std::vector<std::vector<int>> expected_output {
        {1, 2}, {5, 5}, {8, 10}, {15, 23}, {24, 24}, {25, 25}};

    EXPECT_EQ(expected_output, intervalIntersectionFA(first_list, second_list));
}

TEST(IntervalIntersectionTest, SampleTest2)
{
    const std::vector<std::vector<int>> first_list {{1, 3}, {5, 9}};
    const std::vector<std::vector<int>> second_list {};

    EXPECT_TRUE(intervalIntersectionFA(first_list, second_list).empty());
}
