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
    //!
    //!          Time complexity O(N * log N) where N = intervals.size() for
    //!          sorting. end_time_min_heap will have N calls to push. In the
    //!          worst case when all meetings conflict, no elements are popped
    //!          so the time complexity would be O(N * log N) for the min heap.
    //!          Space complexity O(N) in the worst case when end_time_min_heap
    //!          must store all conflicting meetings. std::sort uses O(log N) in
    //!          the average and best cases for quick sort.

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

            if (earliest_end_time <= start_time)
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

//! @brief Priority queues discussion solution
//! @param[in] intervals Reference to vector of meeting time intervals
//! @return Minimum number of conference rooms required
static int minMeetingRoomsDS1(const std::vector<std::vector<int>>& intervals)
{
    //! @details https://leetcode.com/problems/meeting-rooms-ii/editorial/
    //!
    //!          Time complexity O(N * log N) where N = intervals.size().
    //!          Sorting intervals takes O(N * log N). There are also N add
    //!          operations to end_time_min_heap, each taking O(log N). In the
    //!          worst case, all N meetings collide with each other and there
    //!          will be N pop operations as well, each also taking O(log N).
    //!          Space complexity O(N) since end_time_min_heap can contain N
    //!          conflicting meeting intervals.

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
        if (!end_time_min_heap.empty()
            && end_time_min_heap.top() <= interval[0])
        {
            end_time_min_heap.pop();
        }

        end_time_min_heap.push(interval[1]);
    }

    return static_cast<int>(std::ssize(end_time_min_heap));
}

//! @brief Chronological ordering discussion solution
//! @param[in] intervals Reference to vector of meeting time intervals
//! @return Minimum number of conference rooms required
static int minMeetingRoomsDS2(const std::vector<std::vector<int>>& intervals)
{
    //! @details https://leetcode.com/problems/meeting-rooms-ii/editorial/
    //!
    //!          Time complexity O(N * log N) where N = intervals.size(). Need
    //!          to sort the start_times and end_times vectors, each containing
    //!          N elements.
    //!          Space complexity O(N) for start_times and end_times.

    std::vector<int> start_times;
    std::vector<int> end_times;
    start_times.reserve(intervals.size());
    end_times.reserve(intervals.size());

    for (const auto& interval : intervals)
    {
        start_times.push_back(interval[0]);
        end_times.push_back(interval[1]);
    }
    std::ranges::sort(start_times);
    std::ranges::sort(end_times);

    int used_rooms {};
    int end_times_idx {};

    for (const int start_time : start_times)
    {
        if (start_time < end_times[end_times_idx])
        {
            ++used_rooms;
            continue;
        }

        ++end_times_idx;
    }

    return used_rooms;
}

TEST(MinMeetingRoomsTest, SampleTest1)
{
    const std::vector<std::vector<int>> intervals {{0, 30}, {5, 10}, {15, 20}};

    EXPECT_EQ(2, minMeetingRoomsFA(intervals));
    EXPECT_EQ(2, minMeetingRoomsDS1(intervals));
    EXPECT_EQ(2, minMeetingRoomsDS2(intervals));
}

TEST(MinMeetingRoomsTest, SampleTest2)
{
    const std::vector<std::vector<int>> intervals {{7, 10}, {2, 4}};

    EXPECT_EQ(1, minMeetingRoomsFA(intervals));
    EXPECT_EQ(1, minMeetingRoomsDS1(intervals));
    EXPECT_EQ(1, minMeetingRoomsDS2(intervals));
}

TEST(MinMeetingRoomsTest, SampleTest3)
{
    const std::vector<std::vector<int>> intervals {{13, 15}, {1, 13}};

    EXPECT_EQ(1, minMeetingRoomsFA(intervals));
    EXPECT_EQ(1, minMeetingRoomsDS1(intervals));
    EXPECT_EQ(1, minMeetingRoomsDS2(intervals));
}
