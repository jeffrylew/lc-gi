#include <gtest/gtest.h>

#include <algorithm>
#include <deque>
#include <iterator>
#include <vector>

//! @brief First attempt to compute how much water can be trapped after rain
//! @param[in] height Vector of N non-negative ints representing elevation map
//! @return Units of rain water that can be trapped
static int trapFA(const std::vector<int>& height)
{
    //! @details https://leetcode.com/explore/interview/card/google/59
    //!          /array-and-strings/341/
    //!
    //!          Passes 222/324 test cases. Does not pass SampleTest3.
    //!          Time complexity O(N), where N = Number of heights. We loop
    //!          through the vector at most twice.
    //!          Space complexity O(N) for height_deque and remaining_heights.

    int water_units {};

    //! Deque of heights
    std::deque<int> height_deque;

    for (const int curr_height : height)
    {
        while (!height_deque.empty()
               && curr_height > height_deque.back()
               && curr_height >= height_deque.front())
        {
            const int water_level {std::min(height_deque.front(), curr_height)};

            if (water_level > 0 && water_level != height_deque.back())
            {
                water_units += water_level - height_deque.back();
            }

            height_deque.pop_back();
        }

        height_deque.push_back(curr_height);
    }

    std::vector<int> remaining_heights(std::move_iterator(height_deque.begin()),
                                       std::move_iterator(height_deque.end()));
    height_deque.clear();

    for (const int curr_height : remaining_heights)
    {
        while (!height_deque.empty() && curr_height > height_deque.back())
        {
            water_units += curr_height - height_deque.back();
            height_deque.pop_back();
        }

        height_deque.push_back(curr_height);
    }

    return water_units;

} // static int trapFA( ...

TEST(TrapTest, SampleTest1)
{
    const std::vector<int> height {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};

    EXPECT_EQ(6, trapFA(height));
}

TEST(TrapTest, SampleTest2)
{
    //           -
    // - 1 2 3 4 -
    // - 5 6 - 7 -
    // - - 8 - - -
    // - - 9 - - -
    const std::vector<int> height {4, 2, 0, 3, 2, 5};

    EXPECT_EQ(9, trapFA(height));
}

TEST(TrapTest, SampleTest3)
{
    //   -
    //   - 1 2 -
    //   - 3 4 -
    //   - 5 - -
    //   - 6 - -
    //   - 7 - -
    //   - - - -
    const std::vector<int> height {0, 7, 1, 4, 6};

    EXPECT_NE(7, trapFA(height));
    EXPECT_EQ(5, trapFA(height));
}
