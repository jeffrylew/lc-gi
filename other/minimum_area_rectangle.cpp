#include <gtest/gtest.h>

#include <algorithm>
#include <utility>
#include <vector>

//! @brief Calculate the area of the rectangle given by points
//! @param[in] points Vector of four (x, y) points represented by std::pair
//! @return The area of the rectangle or -1 if points does not give a rectangle
[[nodiscard]] constexpr int
    calculate_rectangle_area_FA(const std::vector<std::pair<int, int>>& points)
{
    const auto sorted_pts = std::ranges::sort(points);

    const auto& [bottom_left_x, bottom_left_y]   = sorted_pts[0];
    const auto& [top_left_x, top_left_y]         = sorted_pts[1];
    const auto& [bottom_right_x, bottom_right_y] = sorted_pts[2];
    const auto& [top_right_x, top_right_y]       = sorted_pts[3];

    if (bottom_left_x != top_left_x
        || bottom_right_x != top_right_x
        || bottom_left_y != bottom_right_y
        || top_left_y != top_right_y)
    {
        return -1;
    }

    const int left_side_size {top_left_y - bottom_left_y};
    const int right_side_size {top_right_y - bottom_right_y};
    const int top_side_size {top_right_x - top_left_x};
    const int bottom_side_size {bottom_right_x - bottom_left_x};

    if (left_side_size != right_side_size
        || top_side_size != bottom_side_size)
    {
        return -1;
    }

    return left_side_size * top_side_size;
}

//! @brief First attempt soln to get min area of a rectangle formed from points
//! @param[in] points A vector of points where points[i] = [x_i, y_i]
//! @return The min area of a rectangle formed from points or 0 if no rectangle
static int minAreaRectFA(const std::vector<std::vector<int>>& points)
{
    //! @details leetcode.com/explore/interview/card/google/66/others-4/3105

    const auto num_points = static_cast<int>(std::ssize(points));
    if (num_points < 4)
    {
        return 0;
    }

    const std::vector<std::pair<int, int>> neighbors {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}};
}

//! @brief Sort by column discussion solution
//! @param[in] points A vector of points where points[i] = [x_i, y_i]
//! @return The min area of a rectangle formed from points or 0 if no rectangle
static int minAreaRectDS1(const std::vector<std::vector<int>>& points)
{
    //! @details https://leetcode.com/problems/minimum-area-rectangle/editorial/

    //! @todo
}

TEST(MinAreaRectTest, SampleTest1)
{
    const std::vector<std::vector<int>> points {
        {1, 1}, {1, 3}, {3, 1}, {3, 3}, {2, 2}};

    EXPECT_EQ(4, minAreaRectFA(points));
    EXPECT_EQ(4, minAreaRectDS1(points));
}

TEST(MinAreaRectTest, SampleTest1)
{
    const std::vector<std::vector<int>> points {
        {1, 1}, {1, 3}, {3, 1}, {3, 3}, {4, 1}, {4, 3}};

    EXPECT_EQ(2, minAreaRectFA(points));
    EXPECT_EQ(2, minAreaRectDS1(points));
}
