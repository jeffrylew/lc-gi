#include <gtest/gtest.h>

#include <unordered_map>
#include <vector>

//! @brief First attempt to get max number of fruits
//! @param[in] fruits Vector of fruits where fruits[i] is the type of fruit at i
//! @return Max number of fruits if you have two baskets for two types of fruit
static int totalFruitFA(std::vector<int> fruits)
{
    //! @details https://leetcode.com/problems/fruit-into-baskets/description/
    //!
    //!          Time complexity O(N) where N = fruits.size().
    //!          Space complexity O(1) since the fruit_count map holds 2 + 1
    //!          entries at most (constant value).

    int max_fruits {};

    //! Map of <fruit type, count in window of trees>
    std::unordered_map<int, int> fruit_count {};

    //! Total number of fruit trees in the row
    const auto num_trees = static_cast<int>(std::ssize(fruits));

    for (int right_tree = 0; right_tree < num_trees; ++right_tree)
    {
        ++fruit_count[fruits[right_tree]];

        if (std::ssize(fruit_count) <= 2)
        {
            ++max_fruits;
            continue;
        }

        const int left_fruit_type {fruits[right_tree - max_fruits]};
        if (--fruit_count[left_fruit_type] == 0)
        {
            fruit_count.erase(left_fruit_type);
        }
    }

    return max_fruits;

} // static int totalFruitFA( ...

//! Skip Brute Force (DS1) and Optimized Brute Force (DS2) discussion solutions

//! @brief Sliding window discussion solution to get max number of fruits
//! @param[in] fruits Vector of fruits where fruits[i] is the type of fruit at i
//! @return Max number of fruits if you have two baskets for two types of fruit
static int totalFruitDS3(std::vector<int> fruits)
{
    //! @details https://leetcode.com/problems/fruit-into-baskets/editorial/
    //!
    //!          Time complexity O(N) where N = fruits.size(). left and right
    //!          monotonically increase so we have at most 2 * N steps. At each
    //!          step, we update the hash map by adding and deleting fruits
    //!          which take constant time.
    //!          Space complexity O(N). In the worst case scenario, there might
    //!          be O(N) types of fruits in the window. For example, if we have
    //           fruits = [1, 1, 1, ..., 1, 2, 3, 4, ..., N/2 + 1] then
    //!          fruit_count.size() == 1 for fruit type = 1. In the second half
    //!          of the vector, fruit_count.size() grows to N / 2.

    //! Map of <fruit type, count in window of trees>
    std::unordered_map<int, int> fruit_count {};

    int left {};
    int right {};

    while (right < std::ssize(fruits))
    {
        //! Add fruit from the right index
        ++fruit_count[fruits[right]];

        //! If current window has more than two types of fruit,
        //! remove one fruit from the left index of the window
        if (std::ssize(fruit_count) > 2)
        {
            if (--fruit_count[fruits[left]] == 0)
            {
                fruit_count.erase(fruits[left]);
            }

            ++left;
        }

        ++right;
    }

    //! Indices left and right will contain the longest valid subarray
    //! right == fruits.size() after the while loop
    return right - left;

} // static int totalFruitDS3( ...

TEST(TotalFruitTest, SampleTest1)
{
    EXPECT_EQ(3, totalFruitFA({1, 2, 1}));
    EXPECT_EQ(3, totalFruitDS3({1, 2, 1}));
}

TEST(TotalFruitTest, SampleTest2)
{
    EXPECT_EQ(3, totalFruitFA({0, 1, 2, 2}));
    EXPECT_EQ(3, totalFruitDS3({0, 1, 2, 2}));
}

TEST(TotalFruitTest, SampleTest3)
{
    EXPECT_EQ(4, totalFruitFA({1, 2, 3, 2, 2}));
    EXPECT_EQ(4, totalFruitDS3({1, 2, 3, 2, 2}));
}
