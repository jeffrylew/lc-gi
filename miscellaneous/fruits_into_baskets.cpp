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

TEST(TotalFruitTest, SampleTest1)
{
    EXPECT_EQ(3, totalFruitFA({1, 2, 1}));
}

TEST(TotalFruitTest, SampleTest2)
{
    EXPECT_EQ(3, totalFruitFA({0, 1, 2, 2}));
}

TEST(TotalFruitTest, SampleTest3)
{
    EXPECT_EQ(4, totalFruitFA({1, 2, 3, 2, 2}));
}
