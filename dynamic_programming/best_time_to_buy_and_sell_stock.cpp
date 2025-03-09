#include <gtest/gtest.h>

#include <vector>

static int get_max_profit(const std::vector<int>&        prices,
                          std::vector<std::vector<int>>& profit_state,
                          int                            curr_day,
                          bool                           is_holding)
{
    //! Options at each day:
    //! A) Buy if not currently holding stock
    //! B) Sell if currently holding stock
    //! C) Skip to next day without buying or selling
    //!
    //! Profit from options at curr_day:
    //! A) prev_day_profit - prices[curr_day]
    //! B) prev_day_profit + prices[curr_day]
    //! C) Maintain prev_day_profit
    //!
    //! Recurrence relation:
    //! Max profit at curr_day = max(
    //!     prev_day_profit - prices[curr_day] (buy if not holding),
    //!     prev_day_profit + prices[curr_day] (sell if holding),
    //!     prev_day_profit (skip buy/sell)
    //! )

    //! @todo Review solution, run out of time

    return std::max({});

} // static int get_max_profit( ...

//! @brief First attempt to get max profit from buying and selling stock
//! @param[in] prices Vector of prices where prices[i] is price on the ith day
//! @return Max profit achieved from transaction
static int maxProfitFA(std::vector<int> prices)
{
    //! @details https://leetcode.com/explore/interview/card/google/64
    //!          /dynamic-programming-4/3086/

    const auto num_days = static_cast<int>(std::ssize(prices));

    //! State consist of current day, holding/not holding, and current profit
    //! profit_state[curr_day][is_holding] = curr_profit
    std::vector<std::vector<int>> profit_state(num_days + 1,
                                               std::vector<int>(2U, 0));

    //! Want max profit on the last day - should not be holding stock then
    return get_max_profit(prices, profit_state, num_days, false);

} // static int maxProfitFA( ...

TEST(MaxProfitTest, SampleTest1)
{
    // EXPECT_EQ(5, maxProfitFA({7, 1, 5, 3, 6, 4}));
}

TEST(MaxProfitTest, SampleTest2)
{
    // EXPECT_EQ(0, maxProfitFA({7, 6, 4, 3, 1}));
}
