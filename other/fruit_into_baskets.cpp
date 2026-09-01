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
    //!
    //!          Time complexity O(N ^ 3) where N = the number of trees. We have
    //!          three nested loops - the first loop for the left index, the
    //!          second for the right index, and the third loop for curr_tree.
    //!          In each iteration, we add the current fruit type to the basket
    //!          set in constant time.
    //!          Space complexity O(N). We count the types of fruits in every
    //!          subarray and store them in a hash set. In the worst case, there
    //!          could be O(N) types in some subarrays.

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

//! @brief Optimized brute force discussion solution
//! @param[in] fruits Reference to vector of trees where fruits[i] = fruit type
//! @return Max number of fruits that I can pick from selecting two fruit types
static int totalFruitDS2(const std::vector<int>& fruits)
{
    //! @details https://leetcode.com/problems/fruit-into-baskets/editorial/
    //!
    //!          Time complexity O(N ^ 2) where N = the number of trees. There
    //!          are two nested loops. In each iteration, we add the current
    //!          fruit type to the hash set basket in constant time.
    //!          Space complexity O(1). We use basket to count the fruit types
    //!          in each subarray and the number of fruit types won't exceed 3.

    const auto num_trees = static_cast<int>(std::ssize(fruits));

    int max_fruits_picked {};

    std::unordered_set<int> basket;

    for (int left_tree = 0; left_tree < num_trees; ++left_tree)
    {
        basket.clear();

        int right_tree {left_tree};

        while (right_tree < num_trees)
        {
            if (!basket.contains(fruits[right_tree]) && std::ssize(basket) == 2)
            {
                break;
            }

            basket.insert(fruits[right_tree]);
            ++right_tree;
        }

        max_fruits_picked = std::max(max_fruits_picked, right_tree - left_tree);
    }

    return max_fruits_picked;
}

//! @brief Sliding window discussion solution
//! @param[in] fruits Reference to vector of trees where fruits[i] = fruit type
//! @return Max number of fruits that I can pick from selecting two fruit types
static int totalFruitDS3(const std::vector<int>& fruits)
{
    //! @details https://leetcode.com/problems/fruit-into-baskets/editorial/
    //!
    //!          Time complexity O(N) where N = the number of trees. Indices
    //!          left_tree and right_tree only monotonically increase during an
    //!          iteration, so we have at most 2 * N steps. In each step, we
    //!          update the hash map by adding and maybe deleting a fruit type
    //!          in constant time.
    //!          Space complexity O(N) since there might be O(N) fruit types in
    //!          the window in the worst case.

    //! Map of <fruit type, count>
    std::unordered_map<int, int> basket_counts;

    int left_tree {};
    for (int right_tree = 0; right_tree < std::ssize(fruits); ++right_tree)
    {
        ++basket_counts[fruits[right_tree]];

        if (std::ssize(basket_counts) > 2)
        {
            --basket_counts[fruits[left_tree]];

            if (basket_counts[fruits[left_tree]] == 0)
            {
                basket_counts.erase(fruits[left_tree]);
            }

            ++left_tree;
        }
    }

    return right_tree - left_tree;
}

//! @brief Sliding window II discussion solution
//! @param[in] fruits Reference to vector of trees where fruits[i] = fruit type
//! @return Max number of fruits that I can pick from selecting two fruit types
static int totalFruitDS4(const std::vector<int>& fruits)
{
    //! @details https://leetcode.com/problems/fruit-into-baskets/editorial/

    //! Map of <fruit type, count>
    std::unordered_map<int, int> basket_counts;

    int max_num_fruits {};
    int left_tree {};

    for (int right_tree = 0; right_tree < std::ssize(fruits); ++right_tree)
    {
        ++basket_counts[fruits[right_tree]];

        while (std::ssize(basket_counts) > 2L)
        {
            --basket_counts[fruits[left_tree]];

            if (basket_counts[fruits[left_tree]] == 0)
            {
                basket_counts.erase(fruits[left_tree]);
            }

            ++left_tree;
        }

        max_num_fruits = std::max(max_num_fruits, right_tree - left_tree + 1);
    }

    return max_num_fruits;
}

TEST(TotalFruitTest, SampleTest1)
{
    const std::vector<int> fruits {1, 2, 1};

    EXPECT_EQ(3, totalFruitFA(fruits));
    EXPECT_EQ(3, totalFruitDS1(fruits));
    EXPECT_EQ(3, totalFruitDS2(fruits));
    EXPECT_EQ(3, totalFruitDS3(fruits));
    EXPECT_EQ(3, totalFruitDS4(fruits));
}

TEST(TotalFruitTest, SampleTest2)
{
    const std::vector<int> fruits {0, 1, 2, 2};

    EXPECT_EQ(3, totalFruitFA(fruits));
    EXPECT_EQ(3, totalFruitDS1(fruits));
    EXPECT_EQ(3, totalFruitDS2(fruits));
    EXPECT_EQ(3, totalFruitDS3(fruits));
    EXPECT_EQ(3, totalFruitDS4(fruits));
}

TEST(TotalFruitTest, SampleTest3)
{
    const std::vector<int> fruits {1, 2, 3, 2, 2};

    EXPECT_EQ(4, totalFruitFA(fruits));
    EXPECT_EQ(4, totalFruitDS1(fruits));
    EXPECT_EQ(4, totalFruitDS2(fruits));
    EXPECT_EQ(4, totalFruitDS3(fruits));
    EXPECT_EQ(4, totalFruitDS4(fruits));
}
