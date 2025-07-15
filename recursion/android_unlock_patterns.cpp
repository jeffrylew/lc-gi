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

    std::function<int(int)> get_num_patterns = [&](int starting_key) {
        int num_patterns_from_starting_key {};

        //! Stack for <key to process, number of keys in sequence so far>
        std::stack<std::pair<int, int>> key_stack;
        key_stack.emplace(starting_key, 1);

        while (!key_stack.empty())
        {
            const auto [curr_key, curr_num_keys] = key_stack.top();
            key_stack.pop();

            visited_keys.insert(curr_key);

            if (curr_num_keys > n)
            {
                //! Exceeded max number of keys in unlock pattern
                continue;
            }

            if (m <= curr_num_keys)
            {
                ++num_patterns_from_starting_key;
            }

            for (const auto& neighbor : reachable_neighbors[curr_key])
            {
                if (visited_keys.contains(neighbor))
                {
                    //! This key is already in the pattern
                    continue;
                }

                key_stack.emplace(neighbor, curr_num_keys + 1);
            }
        }

        return num_patterns_from_starting_key;
    };

    int total_num_patterns {};

    for (int key = 1; key <= 9; ++key)
    {
        visited_keys.clear();

        total_num_patterns += get_num_patterns(key);
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
