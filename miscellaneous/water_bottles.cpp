#include <gtest/gtest.h>

//! @brief First attempt to get max num of water bottles can drink
//! @param[in] numBottles  Initial number of bottles filled with water
//! @param[in] numExchange Empty water bottle to full water bottle exchange rate
//! @return Max number of water bottles one can drink
static int numWaterBottlesFA(int numBottles, int numExchange)
{
    //! @details https://leetcode.com/problems/water-bottles/description/
    //!
    //!          Time complexity O(log_M N) where M = numExchange and N = num of
    //!          initial full bottles. Divide the number of full bottles
    //!          numBottles by numExchange.
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

//! @brief Simulation discussion solution
//! @param[in] numBottles  Initial number of bottles filled with water
//! @param[in] numExchange Empty water bottle to full water bottle exchange rate
//! @return Max number of water bottles one can drink
static int numWaterBottlesDS1(int numBottles, int numExchange)
{
    //! @details https://leetcode.com/problems/water-bottles/description/
    //!
    //!          Time complexity O(N) where N is the number of initial full
    //!          bottles. The max number of operations in while loop occurs when
    //!          numExchange is minimized i.e. 2. Consuming 2 bottles and adding
    //!          1 bottle means numBottles is decreased by one each iteration.
    //!          Space complexity O(1), no extra space apart from few variables

    int consumed_bottles {};

    while (numBottles >= numExchange)
    {
        //! Consume numExchange full bottles
        consumed_bottles += numExchange;
        numBottles -= numExchange;

        //! Exchange them for one full bottle
        ++numBottles;
    }

    //! Consume the remaining numBottles (< numExchange)
    return consumed_bottles + numBottles;
}

TEST(NumWaterBottlesTest, SampleTest1)
{
    EXPECT_EQ(13, numWaterBottlesFA(9, 3));
    EXPECT_EQ(13, numWaterBottlesDS1(9, 3));
}

TEST(NumWaterBottlesTest, SampleTest2)
{
    EXPECT_EQ(19, numWaterBottlesFA(15, 4));
    EXPECT_EQ(19, numWaterBottlesDS1(15, 4));
}
