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

//! @brief Next array discussion solution
//! @param[in] seats Vector of empty (0) and occupied (1) seats
//! @return Max distance to the closest person
static int maxDistToClosestDS1(std::vector<int> seats)
{
    //! @details https://leetcode.com/problems
    //!          /maximize-distance-to-closest-person/editorial/
    //!
    //!          Time complexity O(N) where N = seats.size()
    //!          Space complexity O(N) for left_closest_person and
    //!          right_closest_person vectors.

    const auto num_seats = static_cast<int>(std::ssize(seats));

    std::vector<int> left_closest_person(num_seats, num_seats);
    std::vector<int> right_closest_person(num_seats, num_seats);

    for (int idx = 0; idx < num_seats; ++idx)
    {
        if (seats[idx] == 1)
        {
            left_closest_person[idx] = 0;
        }
        else if (idx > 0)
        {
            left_closest_person[idx] = left_closest_person[idx - 1] + 1;
        }
    }

    for (int idx = num_seats - 1; idx >= 0; --idx)
    {
        if (seats[idx] == 1)
        {
            right_closest_person[idx] = 0;
        }
        else if (idx < num_seats - 1)
        {
            right_closest_person[idx] = right_closest_person[idx + 1] + 1;
        }
    }

    int max_dist {};
    for (int idx = 0; idx < num_seats; ++idx)
    {
        if (seats[idx] == 0)
        {
            max_dist = std::max(max_dist,
                                std::min(left_closest_person[idx],
                                         right_closest_person[idx]));
        }
    }

    return max_dist;

} // static int maxDistToClosestDS1( ...

//! @brief Two pointer discussion solution
//! @param[in] seats Vector of empty (0) and occupied (1) seats
//! @return Max distance to the closest person
static int maxDistToClosestDS2(std::vector<int> seats)
{
    //! @details https://leetcode.com/problems
    //!          /maximize-distance-to-closest-person/editorial/
    //!
    //!          Time complexity O(N) where N = seats.size().
    //!          Space complexity O(1)

    const auto num_seats = static_cast<int>(std::ssize(seats));

    int left_closest_person {-1};
    int right_closest_person {};
    int max_dist {};

    for (int idx = 0; idx < num_seats; ++idx)
    {
        if (seats[idx] == 1)
        {
            left_closest_person = idx;
            continue;
        }

        while (right_closest_person < num_seats
               && seats[right_closest_person] == 0
               || right_closest_person < idx)
        {
            ++right_closest_person;
        }

        //! dist_to_left_person is considered infinite (= num_seats) if there is
        //! no person to the left of seat idx
        const int dist_to_left_person {
            left_closest_person == -1 ? num_seats : idx - left_closest_person};

        //! dist_to_right_person is considered infinite (= num_seats) if there
        //! is no person to the right of seat idx
        const int dist_to_right_person {
            right_closest_person == num_seats
                ? num_seats
                : right_closest_person - idx};

        max_dist = std::max(std::min(dist_to_left_person, dist_to_right_person),
                            max_dist);
    }

    return max_dist;

} // static int maxDistToClosestDS2( ...

TEST(MaxDistToClosestTest, SampleTest1)
{
    EXPECT_EQ(2, maxDistToClosestFA({1, 0, 0, 0, 1, 0, 1}));
    EXPECT_EQ(2, maxDistToClosestDS1({1, 0, 0, 0, 1, 0, 1}));
    EXPECT_EQ(2, maxDistToClosestDS2({1, 0, 0, 0, 1, 0, 1}));
}

TEST(MaxDistToClosestTest, SampleTest2)
{
    EXPECT_EQ(3, maxDistToClosestFA({1, 0, 0, 0}));
    EXPECT_EQ(3, maxDistToClosestDS1({1, 0, 0, 0}));
    EXPECT_EQ(3, maxDistToClosestDS2({1, 0, 0, 0}));
}

TEST(MaxDistToClosestTest, SampleTest3)
{
    EXPECT_EQ(1, maxDistToClosestFA({0, 1}));
    EXPECT_EQ(1, maxDistToClosestDS1({0, 1}));
    EXPECT_EQ(1, maxDistToClosestDS2({0, 1}));
}

TEST(MaxDistToClosestTest, SampleTest4)
{
    EXPECT_EQ(2, maxDistToClosestFA({0, 0, 1, 0, 1, 1}));
    EXPECT_EQ(2, maxDistToClosestDS1({0, 0, 1, 0, 1, 1}));
    EXPECT_EQ(2, maxDistToClosestDS2({0, 0, 1, 0, 1, 1}));
}
