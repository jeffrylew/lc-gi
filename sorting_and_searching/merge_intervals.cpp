#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
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

TEST(MergeTest, SampleTest1)
{
    const std::vector<std::vector<int>> intervals {
        {1, 3}, {2, 6}, {8, 10}, {15, 18}};

    const std::vector<std::vector<int>> expected_output {
        {1, 6}, {8, 10}, {15, 18}};

    EXPECT_EQ(expected_output, mergeFA(intervals));
}

TEST(MergeTest, SampleTest2)
{
    const std::vector<std::vector<int>> intervals {{1, 4}, {4, 5}};
    const std::vector<std::vector<int>> expected_output {{1, 5}};

    EXPECT_EQ(expected_output, mergeFA(intervals));
}

TEST(MergeTest, SampleTest3)
{
    const std::vector<std::vector<int>> intervals {
        {2, 3}, {4, 5}, {6, 7}, {8, 9}, {1, 10}};
    const std::vector<std::vector<int>> expected_output {{1, 10}};

    EXPECT_NE(expected_output, mergeFA(intervals));
}
