#include <gtest/gtest.h>

#include <vector>

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

    //! @todo
}

TEST(MinAreaRectTest, SampleTest1)
{
    const std::vector<std::vector<int>> points {
        {1, 1}, {1, 3}, {3, 1}, {3, 3}, {2, 2}};

    EXPECT_EQ(4, minAreaRectFA(points));
}

TEST(MinAreaRectTest, SampleTest1)
{
    const std::vector<std::vector<int>> points {
        {1, 1}, {1, 3}, {3, 1}, {3, 3}, {4, 1}, {4, 3}};

    EXPECT_EQ(2, minAreaRectFA(points));
}
