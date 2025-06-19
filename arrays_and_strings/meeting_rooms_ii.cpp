#include <gtest/gtest.h>

#include <algorithm>
#include <queue>
#include <vector>

//! @brief First attempt to get min number of conference rooms required
//! @param[in] intervals Reference to vector of meeting time intervals
//! @return Minimum number of conference rooms required
static int minMeetingRoomsFA(const std::vector<std::vector<int>>& intervals)
{
    auto sorted_intervals = intervals;
    std::sort(sorted_intervals.begin(),
              sorted_intervals.end(),
              [](const std::vector<int>& lhs, const std::vector<int>& rhs) {
                  return lhs[0] < rhs[0];
              });

    //! @todo
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
