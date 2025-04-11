#include <gtest/gtest.h>

#include <algorithm>
#include <string>

//! @brief First attempt to get sum of binary strings a and b
//! @param[in] a First binary string
//! @param[in] b Second binary string
//! @return Binary string for sum of a and b
static std::string addBinaryFA(std::string a, std::string b)
{
    //! @details https://leetcode.com/problems/add-binary/description/
    //!
    //!          Time complexity O(max(A, B)) where A = a.size(), B = b.size().
    //!          Space complexity O(1) if output string sum excluded.

    std::string sum {};

    bool carryover {};
    auto a_rit = a.rbegin();
    auto b_rit = b.rbegin();

    const auto update_sum_at_digit_zero = [&] {
        if (carryover)
        {
            sum += '1';
            carryover = false;
        }
        else
        {
            sum += '0';
        }
    };

    const auto update_sum_at_digit_one = [&] {
        if (carryover)
        {
            sum += '0';
            carryover = true;
        }
        else
        {
            sum += '1';
        }
    };

    while (a_rit != a.rend() && b_rit != b.rend())
    {
        if (*a_rit == '0' && *b_rit == '0')
        {
            update_sum_at_digit_zero();
        }
        else if (*a_rit == '1' && *b_rit == '1')
        {
            sum += carryover ? '1' : '0';
            carryover = true;
        }
        else
        {
            update_sum_at_digit_one();
        }

        ++a_rit;
        ++b_rit;
    }

    while (a_rit != a.rend())
    {
        if (*a_rit == '0')
        {
            update_sum_at_digit_zero();
        }
        else
        {
            update_sum_at_digit_one();
        }

        ++a_rit;
    }

    while (b_rit != b.rend())
    {
        if (*b_rit == '0')
        {
            update_sum_at_digit_zero();
        }
        else
        {
            update_sum_at_digit_one();
        }

        ++b_rit;
    }

    if (carryover)
    {
        sum += '1';
    }

    std::reverse(sum.begin(), sum.end());
    return sum;

} // static std::string addBinaryFA( ...

TEST(AddBinaryTest, SampleTest1)
{
    EXPECT_EQ("100", addBinaryFA("11", "1"));
}

TEST(AddBinaryTest, SampleTest2)
{
    EXPECT_EQ("10101", addBinaryFA("1010", "1011"));
}