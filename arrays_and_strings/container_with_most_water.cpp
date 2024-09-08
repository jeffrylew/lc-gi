#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

//! @brief First attempt to get max amount of water a container can store
//! @param[in] height Vector of heights of length 
static int maxAreaFA(std::vector<int> height)
{
    //! @details https://leetcode.com/problems/container-with-most-water
    //!
    //!          Time complexity O(N) where N = height.size(). Need to loop
    //!          through elements in height.
    //!          Space complexity O(1)

    const auto num_heights = static_cast<int>(std::ssize(height));

    int max_area {};
    int start {};
    int end {num_heights - 1};

    while (start < end)
    {
        const int lower_height {std::min(height[start], height[end])};
        const int curr_area {(end - start) * lower_height};

        max_area = std::max(max_area, curr_area);

        if (height[start] < height[end])
        {
            ++start;
        }
        else
        {
            --end;
        }
    }

    return max_area;

} // static int maxAreaFA( ...

TEST(MaxAreaTest, SampleTest1)
{
    EXPECT_EQ(49, maxAreaFA({1, 8, 6, 2, 5, 4, 8, 3, 7}));
}

TEST(MaxAreaTest, SampleTest2)
{
    EXPECT_EQ(1, maxAreaFA({1, 1}));
}
