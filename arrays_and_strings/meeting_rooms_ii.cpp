#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <queue>
#include <vector>

//! @brief First attempt to get min number of conference rooms required
//! @param[in] intervals Reference to vector of meeting time intervals
//! @return Minimum number of conference rooms required
static int minMeetingRoomsFA(const std::vector<std::vector<int>>& intervals)
{
    //! @details https://leetcode.com/explore/interview/card/google/59
    //!          /array-and-strings/3059/

    int min_req_rooms {};

    auto sorted_intervals = intervals;
    std::sort(sorted_intervals.begin(),
              sorted_intervals.end(),
              [](const std::vector<int>& lhs, const std::vector<int>& rhs) {
                  return lhs[0] < rhs[0];
              });

    std::priority_queue<int,
                        std::vector<int>,
                        std::greater<int>> end_time_min_heap;

    for (const auto& interval : sorted_intervals)
    {
        const int start_time {interval[0]};
        const int end_time {interval[1]};

        if (!end_time_min_heap.empty())
        {
            const int earliest_end_time {end_time_min_heap.top()};

            if (earliest_end_time < start_time)
            {
                end_time_min_heap.pop();
            }
        }

        end_time_min_heap.push(end_time);

        min_req_rooms =
            std::max(min_req_rooms,
                     static_cast<int>(std::ssize(end_time_min_heap)));
    }

    return min_req_rooms;
}

TEST(MinMeetingRoomsTest, SampleTest1)
{
    const std::vector<std::vector<int>> intervals {{0, 30}, {5, 10}, {15, 20}};

    EXPECT_EQ(2, minMeetingRoomsFA(intervals));
}

TEST(MinMeetingRoomsTest, SampleTest2)
{
    const std::vector<std::vector<int>> intervals {{7, 10}, {2, 4}};

    EXPECT_EQ(1, minMeetingRoomsFA(intervals));
}
