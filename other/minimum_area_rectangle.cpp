#include <gtest/gtest.h>

#include <algorithm>
#include <flat_map>
#include <limits>
#include <unordered_map>
#include <unordered_set>
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
    //!
    //!          Time complexity O(N ^ 2) where N = points.size().
    //!          Space complexity O(N).

    std::flat_map<int, std::vector<int>> grouped_columns;
    for (const auto& point : points)
    {
        const int x_coord {point.front()};
        const int y_coord {point.back()};
        grouped_columns[x_coord].push_back(y_coord);
    }

    int min_area {std::numeric_limits<int>::max()};

    //! Map of <id of right side formed from two y coords, shared x coord>
    std::unordered_map<int, int> last_x_coord;

    for (auto& [x_coord, y_coords_vec] : grouped_columns)
    {
        std::ranges::sort(y_coords_vec);
        const auto num_y_coords = static_cast<int>(std::ssize(y_coords_vec));

        for (int y_coord1_idx = 0; y_coord1_idx < num_y_coords; ++y_coord1_idx)
        {
            for (int y_coord2_idx = y_coord1_idx + 1;
                 y_coord2_idx < num_y_coords;
                 ++y_coord2_idx)
            {
                const int smaller_y_coord {y_coords_vec[y_coord1_idx]};
                const int larger_y_coord {y_coords_vec[y_coord2_idx]};

                const int right_side_id_from_y_coords {
                    40001 * smaller_y_coord + larger_y_coord};

                auto side_it = last_x_coord.find(right_side_id_from_y_coords);
                if (side_it != last_x_coord.end())
                {
                    const int curr_area {
                        (x_coord - side_it->second)
                         * (larger_y_coord - smaller_y_coord)};

                    min_area = std::min(min_area, curr_area);
                }

                last_x_coord[right_side_id_from_y_coords] = x_coord;
            }
        }
    }

    return min_area < std::numeric_limits<int>::max() ? min_area : 0;
}

//! @brief Count by diagonal discussion solution
//! @param[in] points A vector of points where points[i] = [x_i, y_i]
//! @return The min area of a rectangle formed from points or 0 if no rectangle
static int minAreaRectDS2(const std::vector<std::vector<int>>& points)
{
    //! @details https://leetcode.com/problems/minimum-area-rectangle/editorial/

    std::unordered_set<int> point_set;

    for (const auto& point : points)
    {
        point_set.insert(40001 * point.front() + point.back());
    }

    int min_area {std::numeric_limits<int>::max()};

    const auto num_points = static_cast<int>(std::ssize(points));

    for (int corner1 = 0; corner1 < num_points; ++corner1)
    {
        for (int corner2 = corner1 + 1; corner2 < num_points; ++corner2)
        {
            const int corner1_x {points[corner1][0]};
            const int corner2_x {points[corner2][0]};
            const int corner1_y {points[corner1][1]};
            const int corner2_y {points[corner2][1]};

            //! @todo
        }
    }
}

TEST(MinAreaRectTest, SampleTest1)
{
    const std::vector<std::vector<int>> points {
        {1, 1}, {1, 3}, {3, 1}, {3, 3}, {2, 2}};

    EXPECT_EQ(4, minAreaRectFA(points));
    EXPECT_EQ(4, minAreaRectDS1(points));
    EXPECT_EQ(4, minAreaRectDS2(points));
}

TEST(MinAreaRectTest, SampleTest1)
{
    const std::vector<std::vector<int>> points {
        {1, 1}, {1, 3}, {3, 1}, {3, 3}, {4, 1}, {4, 3}};

    EXPECT_EQ(2, minAreaRectFA(points));
    EXPECT_EQ(2, minAreaRectDS1(points));
    EXPECT_EQ(2, minAreaRectDS2(points));
}
