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

TEST(MySqrtTest, SampleTest1)
{
    EXPECT_EQ(2, mySqrtFA(4));
    EXPECT_EQ(2, mySqrtDS1(4));
}

TEST(MySqrtTest, SampleTest2)
{
    EXPECT_EQ(2, mySqrtFA(8));
    EXPECT_EQ(2, mySqrtDS1(8));
}
