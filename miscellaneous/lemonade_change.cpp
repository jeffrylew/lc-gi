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

//! @brief Simulation discussion solution
//! @param[in] bills Vector of 5, 10, and 20 dollar bills from customers
//! @return True if can provide every customer with correct change, else false
static bool lemonadeChangeDS(std::vector<int> bills)
{
    //! @details https://leetcode.com/problems/lemonade-change/description/
    //!
    //!          Time complexity O(N) where N = bills.size(). The algorithm
    //!          loops over bills once, taking O(N). All operations within the
    //!          loop are constant time.
    //!          Space complexity O(1). The algorithm does not use additional
    //!          data structures that scale with the input size.

    //! Count of $5 and $10 bills in hand
    int five_dollar_bills {};
    int ten_dollar_bills {};

    //! Iterate through each customer's bill
    for (const int customer_bill : bills)
    {
        if (customer_bill == 5)
        {
            //! Add it to our count
            ++five_dollar_bills;
        }
        else if (customer_bill == 10)
        {
            //! Need to give $5 change
            if (five_dollar_bills > 0)
            {
                --five_dollar_bills;
                ++ten_dollar_bills;
            }
            else
            {
                //! Can't provide change, return false
                return false;
            }
        }
        else
        {
            //! Need to give $15 change
            if (ten_dollar_bills > 0 && five_dollar_bills > 0)
            {
                //! Give change as one $10 and one $5
                --five_dollar_bills;
                --ten_dollar_bills;
            }
            else if (five_dollar_bills >= 3)
            {
                //! Give change as three $5
                five_dollar_bills -= 3;
            }
            else
            {
                //! Can't provide change, return false
                return false;
            }
        }
    }

    //! If made it through all customers, return true
    return true;

} // static bool lemonadeChangeDS( ...

TEST(LemonadeChangeTest, SampleTest1)
{
    EXPECT_TRUE(lemonadeChangeFA({5, 5, 5, 10, 20}));
    EXPECT_TRUE(lemonadeChangeDS({5, 5, 5, 10, 20}));
}

TEST(LemonadeChangeTest, SampleTest2)
{
    EXPECT_FALSE(lemonadeChangeFA({5, 5, 10, 10, 20}));
    EXPECT_FALSE(lemonadeChangeDS({5, 5, 10, 10, 20}));
}
