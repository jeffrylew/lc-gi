#include <gtest/gtest.h>

#include <functional>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <utility>

//! @brief First attempt to get number of unlock patterns with [m, n] keys
//! @param[in] m Minimum number of keys in unlock pattern
//! @param[in] n Maximum number of keys in unlock pattern
//! @return Number of unique and valid unlock patterns with [m, n] keys
static int numberOfPatternsFA(int m, int n)
{
    //! @details leetcode.com/explore/interview/card/google/62/recursion-4/484

    //! Map of <key, reachable neighborings keys from key>
    std::unordered_map<int, std::unordered_set<int>> reachable_neighbors {
        {1, {2, 4, 5, 6, 8}},
        {2, {1, 3, 4, 5, 6, 7, 9}},
        {3, {2, 4, 5, 6, 8}},
        {4, {1, 2, 3, 5, 7, 8, 9}},
        {5, {1, 2, 3, 4, 6, 7, 8, 9}},
        {6, {1, 2, 3, 5, 7, 8, 9}},
        {7, {2, 4, 5, 6, 8}},
        {8, {1, 3, 4, 5, 6, 7, 9}},
        {9, {2, 4, 5, 6, 8}}};

    //! Map of <key, vector<conditionally reachable key, required neighbor>>>
    std::unordered_map<int, std::vector<std::pair<int, int>>>
        conditional_neighbors {
        {1, {{3, 2}, {7, 4}, {9, 5}}},
        {2, {{8, 5}}},
        {3, {{1, 2}, {7, 5}, {9, 6}}},
        {4, {{6, 5}}},
        {5, {}},
        {6, {{4, 5}}},
        {7, {{1, 4}, {3, 5}, {9, 8}}},
        {8, {{2, 5}}},
        {9, {{1, 5}, {3, 6}, {7, 8}}}};

    //! Set of keys that have been visited already
    std::unordered_set<int> visited_keys;

    std::function<int(int, int)> get_num_patterns =
        [&](int key, int num_keys_in_sequence) {

        visited_keys.insert(key);

        if (num_keys_in_sequence > n)
        {
            //! Exceeded max number of keys in unlock pattern
            return 0;
        }

        int num_patterns_from_key {};

        if (m <= num_keys_in_sequence)
        {
            ++num_patterns_from_key;
        }

        for (const auto& neighbor : reachable_neighbors[key])
        {
            if (visited_keys.contains(neighbor))
            {
                //! This key is already in the pattern
                continue;
            }

            num_patterns_from_key += get_num_patterns(neighbor,
                                                      num_keys_in_sequence + 1);

            //! Remove neighbor from visited_keys so the following sequences are
            //! not affected by the current neighbor
            visited_keys.erase(neighbor);
        }

        for (const auto& [conditional_neighbor, required_neighbor]
             : conditional_neighbors[key])
        {
            if (visited_keys.contains(conditional_neighbor))
            {
                //! This key is already in the pattern
                continue;
            }

            if (!visited_keys.contains(required_neighbor))
            {
                //! The required neighbor key isn't in the pattern yet so the
                //! conditional_neighbor key cannot be used
                continue;
            }

            num_patterns_from_key += get_num_patterns(conditional_neighbor,
                                                      num_keys_in_sequence + 1);

            //! Remove conditional_neighbor from visited_keys to backtrack
            visited_keys.erase(conditional_neighbor);
        }

        return num_patterns_from_key;
    };

    int total_num_patterns {};

    for (int key = 1; key <= 9; ++key)
    {
        visited_keys.clear();

        total_num_patterns += get_num_patterns(key, 1);
    }

    return total_num_patterns;
}

TEST(NumberOfPatternsTest, SampleTest1)
{
    EXPECT_EQ(9, numberOfPatternsFA(1, 1));
}

TEST(NumberOfPatternsTest, SampleTest2)
{
    EXPECT_EQ(65, numberOfPatternsFA(1, 2));
}
