#include <gtest/gtest.h>

#include <iterator>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <vector>

//! @class RandomizedSetFA
//! @brief First attempt solution
//! @details https://leetcode.com/explore/interview/card/google/65/design-4/3094
//!
//!          Time complexity O(1).
//!          Space complexity O(N) where N = number of ints in elements set.
class RandomizedSetFA
{
public:
    RandomizedSetFA()
        : mersenne_twister_engine(random_num_engine_seed_source())
    {
    }

    bool insert(int val)
    {
        if (elements.contains(val))
        {
            return false;
        }

        elements.insert(val);
        return true;
    }

    bool remove(int val)
    {
        if (elements.contains(val))
        {
            elements.erase(val);
            return true;
        }

        return false;
    }

    int getRandom()
    {
        std::uniform_int_distribution<> distribution(
            0, static_cast<int>(std::ssize(elements)) - 1);

        const int elements_offset {distribution(mersenne_twister_engine)};

        return *std::next(elements.begin(), elements_offset);
    }

private:
    std::unordered_set<int> elements;

    std::random_device random_num_engine_seed_source;
    std::mt19937       mersenne_twister_engine;
};

//! @class RandomizedSetDS
//! @brief HashMap + Arraylist (vector) discussion solution
//! @details https://leetcode.com/problems/insert-delete-getrandom-o1/editorial
//!
//!          Time complexity O(1).
//!          Space complexity O(N) where N = number of ints in elements set.
class RandomizedSetDS
{
public:
    RandomizedSetDS()
        : mersenne_twister_engine(random_num_engine_seed_source())
    {
    }

    bool insert(int val)
    {
        if (value_idx_map.contains(val))
        {
            return false;
        }

        value_idx_map.try_emplace(val, static_cast<int>(std::ssize(values)));
        values.push_back(val);
        return true;
    }

    bool remove(int val)
    {
        if (!value_idx_map.contains(val))
        {
            return false;
        }

        //! Swap the last element with the element at idx
        const int idx {value_idx_map[val]};
        value_idx_map[values.back()] = idx;
        std::swap(values[idx], values.back());

        //! Delete the last element
        values.pop_back();
        value_idx_map.erase(val);
        return true;
    }

    int getRandom()
    {
        std::uniform_int_distribution<> distribution(
            0, static_cast<int>(std::ssize(values)) - 1);

        return values[distribution(mersenne_twister_engine)];
    }

private:
    std::random_device random_num_engine_seed_source;
    std::mt19937       mersenne_twister_engine;

    //! Vector of element values
    std::vector<int> values;

    //! Map of <element value, index in values vector>
    std::unordered_map<int, int> value_idx_map;
};

TEST(RandomizedSetTest, SampleTest1)
{
    RandomizedSetFA randomized_set_fa;

    EXPECT_TRUE(randomized_set_fa.insert(1));
    EXPECT_FALSE(randomized_set_fa.remove(2));
    EXPECT_TRUE(randomized_set_fa.insert(2));
    EXPECT_GE(randomized_set_fa.getRandom(), 1);
    EXPECT_TRUE(randomized_set_fa.remove(1));
    EXPECT_FALSE(randomized_set_fa.insert(2));
    EXPECT_EQ(randomized_set_fa.getRandom(), 2);

    RandomizedSetDS randomized_set_ds;

    EXPECT_TRUE(randomized_set_ds.insert(1));
    EXPECT_FALSE(randomized_set_ds.remove(2));
    EXPECT_TRUE(randomized_set_ds.insert(2));
    EXPECT_GE(randomized_set_ds.getRandom(), 1);
    EXPECT_TRUE(randomized_set_ds.remove(1));
    EXPECT_FALSE(randomized_set_ds.insert(2));
    EXPECT_EQ(randomized_set_ds.getRandom(), 2);
}
