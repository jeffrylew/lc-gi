#include <gtest/gtest.h>

//! @brief First attempt to get max num of water bottles can drink
//! @param[in] numBottles Initial number of bottles filled with water
//! @param[in] numExchange Empty water bottle to full water bottle exchange rate
//! @return Max number of water bottles one can drink
static int numWaterBottlesFA(int numBottles, int numExchange)
{
    //! @details https://leetcode.com/problems/water-bottles/description/
    //!
    //!          Time complexity O(1)
    //!          Space complexity O(1)

    int num_drank {numBottles};
    int num_full {numBottles / numExchange};
    int num_empty {numBottles % numExchange};

    while (num_full > 0)
    {
        num_drank += num_full;
        num_empty += num_full;

        num_full  = num_empty / numExchange;
        num_empty = num_empty % numExchange;
    }

    return num_drank;
}

TEST(NumWaterBottlesTest, SampleTest1)
{
    EXPECT_EQ(13, numWaterBottlesFA(9, 3));
}

TEST(NumWaterBottlesTest, SampleTest2)
{
    EXPECT_EQ(19, numWaterBottlesFA(15, 4));
}
