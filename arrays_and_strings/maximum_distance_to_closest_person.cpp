#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

//! @brief First attempt to get max distance to closest person
//! @param[in] seats Vector of empty (0) and occupied (1) seats
//! @return Max distance to the closest person
static int maxDistToClosestFA(std::vector<int> seats)
{
    //! @details leetcode.com/problems/maximize-distance-to-closest-person
    //!
    //!          Time complexity O(N) where N = seats.size()
    //!          Space complexity O(1)

    int max_dist {};
    int last_occupied_idx {-1};

    const auto seats_size = static_cast<int>(std::ssize(seats));

    for (int idx = 0; idx < seats_size; ++idx)
    {
        if (seats[idx] == 1)
        {
            max_dist = (last_occupied_idx == -1)
                           ? std::max(max_dist, idx)
                           : std::max(max_dist, (idx - last_occupied_idx) / 2);

            last_occupied_idx = idx;
        }
    }

    //! Handle occupied seat followed by unoccupied seats till end of row
    if (last_occupied_idx < seats_size - 1)
    {
        const int distance_from_end {seats_size - 1 - last_occupied_idx};
        if (distance_from_end > max_dist)
        {
            return distance_from_end;
        }
    }

    return max_dist;

} // static int maxDistToClosestFA( ...

TEST(MaxDistToClosestTest, SampleTest1)
{
    EXPECT_EQ(2, maxDistToClosestFA({1, 0, 0, 0, 1, 0, 1}));
}

TEST(MaxDistToClosestTest, SampleTest2)
{
    EXPECT_EQ(3, maxDistToClosestFA({1, 0, 0, 0}));
}

TEST(MaxDistToClosestTest, SampleTest3)
{
    EXPECT_EQ(1, maxDistToClosestFA({0, 1}));
}

TEST(MaxDistToClosestTest, SampleTest4)
{
    EXPECT_EQ(2, maxDistToClosestFA({0, 0, 1, 0, 1, 1}));
}
