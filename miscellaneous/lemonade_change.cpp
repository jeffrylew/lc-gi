#include <gtest/gtest.h>

#include <vector>

//! @brief First attempt to determine if correct change can be given
//! @param[in] bills Vector of 5, 10, and 20 dollar bills from customers
//! @return True if can provide every customer with correct change, else false
static bool lemonadeChangeFA(std::vector<int> bills)
{
    //! @details https://leetcode.com/problems/lemonade-change/description/
    //!
    //!          Time complexity O(N) where N = bills.size()
    //!          Space complexity O(1)

    int num_fives {};
    int num_tens {};

    for (const int bill : bills)
    {
        if (bill == 5)
        {
            ++num_fives;
        }
        else if (bill == 10)
        {
            ++num_tens;

            if (num_fives == 0)
            {
                return false;
            }

            --num_fives;
        }
        else
        {
            if (num_fives == 0)
            {
                return false;
            }

            if (num_tens > 0)
            {
                --num_tens;
                --num_fives;
            }
            else if (num_fives > 2)
            {
                num_fives -= 3;
            }
            else
            {
                return false;
            }
        }

    } // for (const int bill : bills)

    return true;

} // static bool lemonadeChangeFA( ...

TEST(LemonadeChangeTest, SampleTest1)
{
    EXPECT_TRUE(lemonadeChangeFA({5, 5, 5, 10, 20}));
}

TEST(LemonadeChangeTest, SampleTest2)
{
    EXPECT_FALSE(lemonadeChangeFA({5, 5, 10, 10, 20}));
}
