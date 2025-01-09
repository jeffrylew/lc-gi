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

    std::vector<std::vector<int>> merged_intervals {};

    //! @todo Figure out how to use Union-Find algorithm to union all mergeable
    //!       intervals

    //! Map of <element in interval, representative of interval>
    std::unordered_map<int, int> representative {};

    //! Initialize representative map
    for (const auto& interval : intervals)
    {
        const int lower_bound {interval[0]};
        const int upper_bound {interval[1]};

        //! lower bound is the representative of the interval
        representative[lower_bound] = lower_bound;
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

    //! @todo Loop over interval and merge overlapping ones. Might sort first.
    for (const auto& interval : intervals)
    {
        const int lower_bound {interval[0]};
        const int upper_bound {interval[1]};
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