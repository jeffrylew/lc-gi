#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

//! @brief First attempt to get max amount of water a container can store
//! @param[in] height Vector of heights of length N
//! @return Max amount of water a container can store
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

//! @brief Brute force discussion solution
//! @param[in] height Vector of heights of length N
//! @return Max amount of water a container can store
static int maxAreaDS1(std::vector<int> height)
{
    //! @details https://leetcode.com/problems/container-with-most-water
    //!
    //!          Time complexity O(N ^ 2) where N = height.size(). Calculate
    //!          area for all N(N - 1) / 2 height pairs
    //!          Space complexity O(1), constant extra space is used.

    const auto num_heights = static_cast<int>(std::ssize(height));

    int max_area {};

    for (int left = 0; left < num_heights; ++left)
    {
        for (int right = left + 1; right < num_heights; ++right)
        {
            const int width {right - left};
            const int smaller_height {std::min(height[left], height[right])};

            max_area = std::max(max_area, smaller_height * width);
        }
    }

    return max_area;

} // static int maxAreaDS1( ...

//! @brief Two pointer approach discussion solution
//! @param[in] height Vector of heights of length N
//! @return Max amount of water a container can store
static int maxAreaDS2(std::vector<int> height)
{
    //! @details https://leetcode.com/problems/container-with-most-water
    //!
    //!          Time complexity O(N) where N = height.size(). Single pass.
    //!          Space complexity O(1)

    int max_area {};
    int left {};
    int right {static_cast<int>(std::ssize(height)) - 1};

    while (left < right)
    {
        const int width {right - left};

        max_area = std::max(max_area,
                            std::min(height[left], height[right]) * width);

        if (height[left] <= height[right])
        {
            ++left;
        }
        else
        {
            --right;
        }
    }

    return max_area;

} // static int maxAreaDS2( ...

TEST(MaxAreaTest, SampleTest1)
{
    EXPECT_EQ(49, maxAreaFA({1, 8, 6, 2, 5, 4, 8, 3, 7}));
    EXPECT_EQ(49, maxAreaDS1({1, 8, 6, 2, 5, 4, 8, 3, 7}));
    EXPECT_EQ(49, maxAreaDS2({1, 8, 6, 2, 5, 4, 8, 3, 7}));
}

TEST(MaxAreaTest, SampleTest2)
{
    EXPECT_EQ(1, maxAreaFA({1, 1}));
    EXPECT_EQ(1, maxAreaDS1({1, 1}));
    EXPECT_EQ(1, maxAreaDS2({1, 1}));
}
