#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

static int get_max_profit_FA(const std::vector<int>&        prices,
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

} // static int get_max_profit_FA( ...

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
    return get_max_profit_FA(prices, profit_state, num_days, false);

} // static int maxProfitFA( ...

//! @brief Get max profit we can achieve if on curr_day with is_holding stock
[[nodiscard]] static int get_max_profit_SA(
    const std::vector<int>&        prices,
    std::vector<std::vector<int>>& profit_state,
    int                            curr_day,
    bool                           is_holding)
{
    //! At each state, we have the following decisions:
    //!
    //! A) If we are not holding stock then we can either buy today or skip
    //!    If we buy then profit is
    //!      -prices[curr_day] + get_max_profit_SA(prices, curr_day + 1, true)
    //!      ^ spend money       ^ move onto next day, are now holding stock
    //!
    //! B) If we are holding stock then we can either sell today or skip
    //!    If we sell then profit is
    //!      prices[curr_day] + get_max_profit_SA(prices, curr_day + 1, false)
    //!      ^ gain money       ^ move onto next day, no longer holding stock
    //!
    //! C) If we decide to skip then the profit in cases A) and B) is
    //!      get_max_profit_SA(prices, curr_day + 1, is_holding)
    //!      ^ move onto next day without changing anything else
    //!
    //! Recurrence relation:
    //!   get_max_profit_SA(curr_day, is_holding) = max(skip, buy, sell) where
    //!     skip = get_max_profit_SA(curr_day + 1, is_holding)
    //!     buy = -prices[curr_day] + get_max_profit_SA(curr_day + 1, true),
    //!            only considered if is_holding = false
    //!     sell = prices[curr_day] + get_max_profit_SA(curr_day + 1, false),
    //!            only considered if is_holding = true

    //! Base case: If reached the end of the input then can't make transaction
    if (curr_day == std::ssize(prices))
    {
        return 0;
    }

    const auto holding = static_cast<int>(is_holding);
    if (profit_state[curr_day][holding] != -1)
    {
        return profit_state[curr_day][holding];
    }

    int curr_max_profit {get_max_profit_SA(prices,
                                           profit_state,
                                           curr_day + 1,
                                           is_holding)};
    if (is_holding)
    {
        curr_max_profit =
            std::max(curr_max_profit,
                     prices[curr_day] + get_max_profit_SA(prices,
                                                          profit_state,
                                                          curr_day + 1,
                                                          false));
    }
    else
    {
        curr_max_profit =
            std::max(curr_max_profit,
                     -prices[curr_day] + get_max_profit_SA(prices,
                                                           profit_state,
                                                           curr_day + 1,
                                                           true));
    }

    profit_state[curr_day][holding] = curr_max_profit;
    return curr_max_profit;

} // static int get_max_profit_SA( ...

//! @brief Second attempt after "Best Time to Buy and Sell Stock IV" editorial
//! @param[in] prices Vector of prices where prices[i] is price on the ith day
//! @return Max profit achieved from transaction
static int maxProfitSA(std::vector<int> prices)
{
    std::vector<std::vector<int>> profit_state(prices.size(),
                                               std::vector<int>(2U, -1));

    //! Get max profit we can achieve starting on day 0 without holding stock
    return get_max_profit_SA(prices, profit_state, 0, false);
}

TEST(MaxProfitTest, SampleTest1)
{
    // EXPECT_EQ(5, maxProfitFA({7, 1, 5, 3, 6, 4}));
    // EXPECT_EQ(5, maxProfitSA({7, 1, 5, 3, 6, 4}));
}

TEST(MaxProfitTest, SampleTest2)
{
    // EXPECT_EQ(0, maxProfitFA({7, 6, 4, 3, 1}));
    // EXPECT_EQ(0, maxProfitSA({7, 6, 4, 3, 1}));
}
