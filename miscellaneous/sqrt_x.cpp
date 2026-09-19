#include <gtest/gtest.h>

#include <cmath>

//! @brief First attempt to get the square root of x rounded down to nearest int
//! @param[in] x A non-negative integer
//! @return Non-negative square root of x rounded down to the nearest integer
static int mySqrtFA(int x)
{
    //! @details https://leetcode.com/problems/sqrtx/description/

    int x_lower_bound {1};
    int x_higher_bound {x};

    while (x_lower_bound <= x_higher_bound)
    {
        const int x_mid {x_lower_bound + (x_higher_bound - x_lower_bound) / 2};
        const int quotient {x / x_mid};

        if (x_mid == quotient)
        {
            return x_mid;
        }

        if (x_mid > quotient)
        {
            x_higher_bound = x_mid - 1;
        }
        else // x_mid < quotient
        {
            x_lower_bound = x_mid + 1;
        }
    }

    return x_higher_bound;
}

//! @brief Pocket calculator discussion solution
//! @param[in] x A non-negative integer
//! @return Non-negative square root of x rounded down to the nearest integer
static int mySqrtDS1(int x)
{
    //! @details https://leetcode.com/problems/sqrtx/editorial/
    //!
    //!          Time complexity O(1).
    //!          Space complexity O(1).

    if (x < 2)
    {
        return x;
    }

    //! sqrt(x) = exp(0.5 * log x)
    const auto x_lower_bound = static_cast<long>(std::exp(0.5 * std::log(x)));
    const long x_upper_bound {x_lower_bound + 1};

    if (x_upper_bound > x / x_upper_bound)
    {
        return static_cast<int>(x_lower_bound);
    }

    return static_cast<int>(x_upper_bound);
}

//! @brief Binary search discussion solution
//! @param[in] x A non-negative integer
//! @return Non-negative square root of x rounded down to the nearest integer
static int mySqrtDS2(int x)
{
    //! @details https://leetcode.com/problems/sqrtx/editorial/
    //!
    //!          Time complexity O(log N) using the master theorem, T(N) =
    //!          A * T(N / b) + theta(N ^ d). The equation represents dividing
    //!          the problem into A subproblems of size (N / b) in theta(N ^ d)
    //!          time. At each step, there is only one subproblem A = 1 and its
    //!          size is half of the initial problem b = 2, and all this happens
    //!          in constant time d = 0. So log_b A = d and case 2 result in
    //!          O(log N).
    //!          Space complexity O(1).

    if (x < 2)
    {
        return x;
    }

    int x_lower_bound {2};
    int x_upper_bound {x / 2};

    while (x_lower_bound <= x_upper_bound)
    {
        const int  x_mid {x_lower_bound + (x_upper_bound - x_lower_bound) / 2};
        const auto x_mid2 = static_cast<long>(x_mid) * x_mid;

        if (x_mid2 > x)
        {
            x_upper_bound = x_mid - 1;
        }
        else if (x_mid2 < x)
        {
            x_lower_bound = x_mid + 1;
        }
        else
        {
            return x_mid;
        }
    }

    return x_upper_bound;
}

//! @brief Recursion + bit shifts discussion solution
//! @param[in] x A non-negative integer
//! @return Non-negative square root of x rounded down to the nearest integer
constexpr int mySqrtDS3(int x)
{
    //! @details https://leetcode.com/problems/sqrtx/editorial/
    //!
    //!          Time complexity O(log N).
    //!          Space complexity O(log N) for the recursion stack.

    if (x < 2)
    {
        return x;
    }

    /*
     sqrt(x) = 2 * sqrt(x / 4) = 2 * sqrt(x / (2 ^ 2))
     and
     x << y = x * 2 ^ y
     x >> y = x / 2 ^ y
     so
     x >> 2 = x / (2 ^ 2) = x / 4
     */
    const int x_lower_bound {mySqrtDS3(x >> 2) << 1};
    const int x_upper_bound {x_lower_bound + 1};

    if (x_upper_bound > x / x_upper_bound)
    {
        return x_lower_bound;
    }

    return x_upper_bound;
}

TEST(MySqrtTest, SampleTest1)
{
    EXPECT_EQ(2, mySqrtFA(4));
    EXPECT_EQ(2, mySqrtDS1(4));
    EXPECT_EQ(2, mySqrtDS2(4));
    EXPECT_EQ(2, mySqrtDS3(4));
}

TEST(MySqrtTest, SampleTest2)
{
    EXPECT_EQ(2, mySqrtFA(8));
    EXPECT_EQ(2, mySqrtDS1(8));
    EXPECT_EQ(2, mySqrtDS2(8));
    EXPECT_EQ(2, mySqrtDS3(8));
}
