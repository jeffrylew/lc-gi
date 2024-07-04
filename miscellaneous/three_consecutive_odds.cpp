#include <gtest/gtest.h>

#include <vector>

//! @brief First attempt solution to check for three consecutive odds numbers
//! @param[in] arr Vector of integers from 1 to 1000
//! @return True if there are three consecutive odd numbers in arr, else false
static bool threeConsecutiveOddsFA(std::vector<int> arr)
{
    //! @detail https://leetcode.com/problems/three-consecutive-odds/description
    //!
    //!         Time complexity O(N) where N = arr.size()
    //!         Space complexity O(1)

    if (arr.size() < 3U)
    {
        return false;
    }

    int num_sum {};

    for (const int num : arr)
    {
        const int is_odd {num % 2};
        if (is_odd == 0)
        {
            num_sum = 0;
            continue;
        }

        if (++num_sum == 3)
        {
            return true;
        }
    }

    return false;

} // static bool threeConsecutiveOddsFA( ...

TEST(ThreeConsecutiveOddsTest, SampleTest1)
{
    EXPECT_FALSE(threeConsecutiveOddsFA({2, 6, 4, 1}));
}

TEST(ThreeConsecutiveOddsTest, SampleTest2)
{
    const std::vector<int> arr {1, 2, 34, 3, 4, 5, 7, 23, 12};

    EXPECT_TRUE(threeConsecutiveOddsFA(arr));
}
