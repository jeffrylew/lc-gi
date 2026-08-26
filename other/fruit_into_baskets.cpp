#include <gtest/gtest.h>

#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>

//! @brief First attempt to get max number of fruits that I can pick
//! @param[in] fruits Reference to vector of trees where fruits[i] = fruit type
//! @return Max number of fruits that I can pick from selecting two fruit types
static int totalFruitFA(const std::vector<int>& fruits)
{
    //! @details leetcode.com/explore/interview/card/google/67/sql-2/3046
    //!
    //!          Time complexity O(N) where N = the number of trees. We iterate
    //!          through the fruits vector with two pointers.
    //!          Space complexity O(1) since the fruit_type_counts map stores up
    //!          to three elements.

    const auto num_trees = static_cast<int>(std::ssize(fruits));

    //! Tracks unique fruit types and their counts within the sliding window
    std::unordered_map<int, int> fruit_type_counts;

    int max_num_fruits {};
    int left_tree {};

    for (int right_tree = 0; right_tree < num_trees; ++right_tree)
    {
        ++fruit_type_counts[fruits[right_tree]];

        while (std::ssize(fruit_type_counts) > 2L)
        {
            const int left_fruit_type {fruits[left_tree]};

            --fruit_type_counts[left_fruit_type];

            if (fruit_type_counts[left_fruit_type] == 0)
            {
                fruit_type_counts.erase(left_fruit_type);
            }

            ++left_tree;
        }

        max_num_fruits = std::max(max_num_fruits, right_tree - left_tree + 1);
    }

    return max_num_fruits;
}

//! @brief Brute force discussion solution
//! @param[in] fruits Reference to vector of trees where fruits[i] = fruit type
//! @return Max number of fruits that I can pick from selecting two fruit types
static int totalFruitDS1(const std::vector<int>& fruits)
{
    //! @details https://leetcode.com/problems/fruit-into-baskets/editorial/

    const auto num_trees = static_cast<int>(std::ssize(fruits));

    int max_fruits_picked {};

    std::unordered_set<int> basket;

    for (int left_tree = 0; left_tree < num_trees; ++left_tree)
    {
        for (int right_tree = 0; right_tree < num_trees; ++right_tree)
        {
            basket.clear();

            for (int curr_tree = left_tree;
                 curr_tree <= right_tree;
                 ++curr_tree)
            {
                basket.insert(fruits[curr_tree]);
            }

            if (std::ssize(basket) <= 2)
            {
                max_fruits_picked =
                    std::max(max_fruits_picked, right_tree - left_tree + 1);
            }
        }
    }

    return max_fruits_picked;
}

TEST(TotalFruitTest, SampleTest1)
{
    const std::vector<int> fruits {1, 2, 1};

    EXPECT_EQ(3, totalFruitFA(fruits));
    EXPECT_EQ(3, totalFruitDS1(fruits));
}

TEST(TotalFruitTest, SampleTest2)
{
    const std::vector<int> fruits {0, 1, 2, 2};

    EXPECT_EQ(3, totalFruitFA(fruits));
    EXPECT_EQ(3, totalFruitDS1(fruits));
}

TEST(TotalFruitTest, SampleTest3)
{
    const std::vector<int> fruits {1, 2, 3, 2, 2};

    EXPECT_EQ(4, totalFruitFA(fruits));
    EXPECT_EQ(4, totalFruitDS1(fruits));
}
