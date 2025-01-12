#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <map>
#include <set>
#include <stack>
#include <unordered_map>
#include <vector>

//! @brief First attempt to merge overlapping intervals
//! @param[in] intervals Reference to vector of intervals
//! @return Vector of non-overlapping intervals that cover all input intervals
static std::vector<std::vector<int>> mergeFA(
    const std::vector<std::vector<int>>& intervals)
{
    //! @details https://leetcode.com/explore/interview/card/google/63
    //!          /sorting-and-searching-4/450/
    //!
    //!          First attempt solution does not pass SampleTest3

    std::vector<std::vector<int>> merged_intervals {};

    //! Map of <element in interval, representative of interval>
    std::unordered_map<int, int> representative {};

    //! Sort intervals based on lower bound
    auto sorted_intervals = intervals;
    std::sort(sorted_intervals.begin(),
              sorted_intervals.end(),
              [](const auto& interval_lhs, const auto& interval_rhs) {
                  return interval_lhs[0] < interval_rhs[0];
              });

    //! Initialize representative map
    for (const auto& interval : sorted_intervals)
    {
        const int lower_bound {interval[0]};
        const int upper_bound {interval[1]};

        //! lower bound is the representative of the interval
        //! If it is already in representative then lower bound is already the
        //! upper bound of a previously processed interval.
        if (!representative.contains(lower_bound))
        {
            representative[lower_bound] = lower_bound;
        }
        representative[upper_bound] = lower_bound;
    }

    const std::function<int(int)> find_representative =
        [&](int bound) {
            if (bound == representative[bound])
            {
                return bound;
            }

            return find_representative(representative[bound]);
        };

    const auto combine_intervals =
        [&](int bound_1, int bound_2) {
            bound_1 = find_representative(bound_1);
            bound_2 = find_representative(bound_2);

            //! Elements of intervals have the same representative
            //! They are in the same set so no action needed
            if (bound_1 == bound_2)
            {
                return;
            }

            representative[bound_2] = bound_1;
        };

    for (int idx = 1; idx < std::ssize(sorted_intervals); ++idx)
    {
        const auto& prev_interval = sorted_intervals[idx - 1];
        const auto& curr_interval = sorted_intervals[idx];

        const int prev_lower_bound {prev_interval[0]};
        const int prev_upper_bound {prev_interval[1]};
        const int curr_lower_bound {curr_interval[0]};
        const int curr_upper_bound {curr_interval[1]};

        if (curr_lower_bound <= prev_upper_bound)
        {
            combine_intervals(prev_upper_bound, curr_lower_bound);
        }
    }

    for (const auto& interval : sorted_intervals)
    {
        if (merged_intervals.empty())
        {
            merged_intervals.push_back(interval);
            continue;
        }

        const int lower_bound {interval[0]};
        const int upper_bound {interval[1]};
        const int representative_lower_bound {find_representative(lower_bound)};

        auto& last_interval = merged_intervals.back();
        if (representative_lower_bound == last_interval[0])
        {
            last_interval[1] = std::max(last_interval[1], upper_bound);
        }
        else
        {
            merged_intervals.push_back(interval);
        }
    }

    return merged_intervals;

} // static std::vector<std::vector<int>> mergeFA( ...

//! @brief Connected components discussion solution
//! @param[in] intervals Reference to vector of intervals
//! @return Vector of non-overlapping intervals that cover all input intervals
static std::vector<std::vector<int>> mergeDS1(
    const std::vector<std::vector<int>>& intervals)
{
    //! @details https://leetcode.com/problems/merge-intervals/editorial/
    //!
    //!          Time complexity O(N ^ 2) where N = intervals.size(). Building
    //!          the graph costs O(V + E) = O(V) + O(E) = O(N) + O(N ^ 2) where
    //!          all intervals are mutually overlapping in the worst case.
    //!          Traversing the graph has same cost because visited_intervals
    //!          set guarantees each interval is visited exactly once. Finally,
    //!          because each interval is in exactly one connected component,
    //!          the merge step costs O(V) = O(N). Note that this solution
    //!          passes 168 / 171 cases but has Time Limit Exceeded for 169.
    //!          Space complexity O(N ^ 2). In the worst case, all intervals are
    //!          mutually overlapping so there will be an edge for every pair of
    //!          intervals. The memory footprint is quadratic.

    std::map<std::vector<int>, std::vector<std::vector<int>>> graph {};

    std::map<int, std::vector<std::vector<int>>> intervals_in_component {};

    std::set<std::vector<int>> visited_intervals {};

    //! Build graph where undirected edge exists between overlapping intervals
    const auto build_graph = [&] {
        for (const auto& interval1 : intervals)
        {
            for (const auto& interval2 : intervals)
            {
                //! Intervals overlap
                if (interval1[0] <= interval2[1]
                    && interval2[0] <= interval1[1])
                {
                    graph[interval1].push_back(interval2);
                    graph[interval2].push_back(interval1);
                }
            }
        }
    };

    //! Mark all intervals in same connected component with component_number
    const auto mark_component_DFS =
        [&](const std::vector<int>& start, int component_number) {
            std::stack<std::vector<int>> interval_stack({start});

            while (!interval_stack.empty())
            {
                auto interval = interval_stack.top();
                interval_stack.pop();

                if (!visited_intervals.contains(interval))
                {
                    visited_intervals.insert(interval);

                    intervals_in_component[component_number].push_back(
                        interval);

                    for (const auto& neighbor_interval : graph[interval])
                    {
                        interval_stack.push(neighbor_interval);
                    }
                }
            }
        };

    //! Populate nodes_in_comp with the connected components in graph
    const auto connect_components = [&] {
        int component_number {};

        for (const auto& interval : intervals)
        {
            if (!visited_intervals.contains(interval))
            {
                mark_component_DFS(interval, component_number);
                ++component_number;
            }
        }
    };

    //! Merge all intervals in connected component into one interval
    const auto merge_intervals =
        [&](const std::vector<std::vector<int>>& connected_intervals) {
            int min_lower_bound {connected_intervals.front()[0]};
            int max_upper_bound {connected_intervals.front()[1]};

            for (const auto& interval : connected_intervals)
            {
                min_lower_bound = std::min(min_lower_bound, interval[0]);
                max_upper_bound = std::max(max_upper_bound, interval[1]);
            }

            return std::vector<int> {min_lower_bound, max_upper_bound};
        };

    build_graph();
    connect_components();

    //! For each component, merge all intervals into one interval
    std::vector<std::vector<int>> merged_intervals {};

    for (int component_number = 0;
         component_number < std::ssize(intervals_in_component);
         ++component_number)
    {
        merged_intervals.push_back(
            merge_intervals(intervals_in_component[component_number]));
    }

    return merged_intervals;

} // static std::vector<std::vector<int>> mergeDS1( ...

//! @brief Sorting discussion solution
//! @param[in] intervals Reference to vector of intervals
//! @return Vector of non-overlapping intervals that cover all input intervals
static std::vector<std::vector<int>> mergeDS2(
    const std::vector<std::vector<int>>& intervals)
{
    //! @details https://leetcode.com/problems/merge-intervals/editorial/
    //!
    //!          Time complexity O(N * log N) where N = intervals.size() for
    //!          sorting. We also perform a linear scan which is dominated.
    //!          Space complexity O(log N) or O(N) for sorting O(N) is needed to
    //!          allocate linear space for a copy of intervals, sorted_intervals

    auto sorted_intervals = intervals;
    std::sort(sorted_intervals.begin(),
              sorted_intervals.end(),
              [](const auto& interval_lhs, const auto& interval_rhs) {
                  return interval_lhs[0] < interval_rhs[0];
              });

    std::vector<std::vector<int>> merged_intervals {};

    for (const auto& interval : sorted_intervals)
    {
        //! If the list of merged intervals is empty or if the current interval
        //! does not overlap with the previous, simply append it
        if (merged_intervals.empty()
            || merged_intervals.back()[1] < interval[0])
        {
            merged_intervals.push_back(interval);
            continue;
        }

        //! There is overlap so merge the previous and current intervals
        merged_intervals.back()[1] =
            std::max(merged_intervals.back()[1], interval[1]);
    }

    return merged_intervals;

} // static std::vector<std::vector<int>> mergeDS2( ...

TEST(MergeTest, SampleTest1)
{
    const std::vector<std::vector<int>> intervals {
        {1, 3}, {2, 6}, {8, 10}, {15, 18}};

    const std::vector<std::vector<int>> expected_output {
        {1, 6}, {8, 10}, {15, 18}};

    EXPECT_EQ(expected_output, mergeFA(intervals));
    EXPECT_EQ(expected_output, mergeDS1(intervals));
    EXPECT_EQ(expected_output, mergeDS2(intervals));
}

TEST(MergeTest, SampleTest2)
{
    const std::vector<std::vector<int>> intervals {{1, 4}, {4, 5}};
    const std::vector<std::vector<int>> expected_output {{1, 5}};

    EXPECT_EQ(expected_output, mergeFA(intervals));
    EXPECT_EQ(expected_output, mergeDS1(intervals));
    EXPECT_EQ(expected_output, mergeDS2(intervals));
}

TEST(MergeTest, SampleTest3)
{
    const std::vector<std::vector<int>> intervals {
        {2, 3}, {4, 5}, {6, 7}, {8, 9}, {1, 10}};
    const std::vector<std::vector<int>> expected_output {{1, 10}};

    EXPECT_NE(expected_output, mergeFA(intervals));
    EXPECT_EQ(expected_output, mergeDS1(intervals));
    EXPECT_EQ(expected_output, mergeDS2(intervals));
}
