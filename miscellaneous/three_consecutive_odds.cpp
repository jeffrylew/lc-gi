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

//! @brief Brute force discussion solution
//! @param[in] arr Vector of integers from 1 to 1000
//! @return True if there are three consecutive odd numbers in arr, else false
static bool threeConsecutiveOddsDS1(std::vector<int> arr)
{
    //! @detail https://leetcode.com/problems/three-consecutive-odds/description
    //!
    //!         Time complexity O(N) where N = arr.size()
    //!         Space complexity O(1)

    const auto arr_size = static_cast<int>(arr.size());

    //! Loop through vector up to third-to-last element
    for (int idx = 0; idx < arr_size - 2; ++idx)
    {
        //! Check if current element and next two elements are all odd
        if (arr[idx] % 2 == 1
            && arr[idx + 1] % 2 == 1
            && arr[idx + 2] % 2 == 1)
        {
            return true;
        }
    }

    return false;

} // static bool threeConsecutiveOddsDS1( ...

//! @brief Counting discussion solution
//! @param[in] arr Vector of integers from 1 to 1000
//! @return True if there are three consecutive odd numbers in arr, else false
static bool threeConsecutiveOddsDS2(std::vector<int> arr)
{
    //! @detail https://leetcode.com/problems/three-consecutive-odds/description
    //!
    //!         Time complexity O(N) where N = arr.size()
    //!         Space complexity O(1)

    int consecutive_odds {};

    //! Loop through each element in the vector
    for (int idx = 0; idx < static_cast<int>(arr.size()); ++idx)
    {
        //! Increment counter if number is odd, else reset the counter
        if (arr[idx] % 2 == 1)
        {
            ++consecutive_odds;
        }
        else
        {
            consecutive_odds = 0;
        }

        //! Check if there are three consecutive odd numbers
        if (consecutive_odds == 3)
        {
            return true;
        }
    }

    return false;

} // static bool threeConsecutiveOddsDS2( ...

//! @brief Product of three numbers discussion solution
//! @param[in] arr Vector of integers from 1 to 1000
//! @return True if there are three consecutive odd numbers in arr, else false
static bool threeConsecutiveOddsDS3(std::vector<int> arr)
{
    //! @detail https://leetcode.com/problems/three-consecutive-odds/description
    //!
    //!         A product is odd if all numbers are odd. Each is constrained to
    //!         10 ^ 3 so the max product is 10 ^ 9, and we won't overflow a
    //!         23-bit integer.
    //!
    //!         Time complexity O(N) where N = arr.size()
    //!         Space complexity O(1)

    const auto arr_size = static_cast<int>(arr.size());

    //! Loop through vector up to third-to-last element
    for (int idx = 0; idx < arr_size - 2; ++idx)
    {
        const int product {arr[idx] * arr[idx + 1] * arr[idx + 2]};

        if (product % 2 == 1)
        {
            return true;
        }
    }

    return false;

} // static bool threeConsecutiveOddsDS3( ...

TEST(ThreeConsecutiveOddsTest, SampleTest1)
{
    EXPECT_FALSE(threeConsecutiveOddsFA({2, 6, 4, 1}));
    EXPECT_FALSE(threeConsecutiveOddsDS1({2, 6, 4, 1}));
    EXPECT_FALSE(threeConsecutiveOddsDS2({2, 6, 4, 1}));
}

TEST(ThreeConsecutiveOddsTest, SampleTest2)
{
    const std::vector<int> arr {1, 2, 34, 3, 4, 5, 7, 23, 12};

    EXPECT_TRUE(threeConsecutiveOddsFA(arr));
    EXPECT_TRUE(threeConsecutiveOddsDS1(arr));
    EXPECT_TRUE(threeConsecutiveOddsDS2(arr));
}
