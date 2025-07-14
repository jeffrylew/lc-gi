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

    //! Set of keys that have been visited already
    std::unordered_set<int> visited_keys;

    std::function<int(int)> get_num_patterns = [&](int starting_key) {
        //! Stack for <key to process, number of keys in sequence so far>
        std::stack<std::pair<int, int>> key_stack;
        key_stack.emplace(starting_key, 1);

        //! @todo Keep track of pattern in e.g. vector

        while (!key_stack.empty())
        {
            const auto [curr_key, curr_num_keys] = key_stack.top();
            key_stack.pop();

            if (curr_num_keys > n)
            {
                //! Exceeded max number of keys in unlock pattern
                continue;
            }


        }
    };

    int num_patterns {};

    for (int key = 1; key <= 9; ++key)
    {
        visited_keys.clear();

        num_patterns += get_num_patterns(key);
    }

    return num_patterns;
}

TEST(NumberOfPatternsTest, SampleTest1)
{
    EXPECT_EQ(9, numberOfPatternsFA(1, 1));
}

TEST(NumberOfPatternsTest, SampleTest2)
{
    EXPECT_EQ(65, numberOfPatternsFA(1, 2));
}
