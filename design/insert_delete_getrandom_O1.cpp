#include <gtest/gtest.h>

#include <iterator>
#include <random>
#include <unordered_set>

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
}
