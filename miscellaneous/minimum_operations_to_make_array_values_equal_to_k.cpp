#include <gtest/gtest.h>

#include <set>
#include <unordered_set>
#include <vector>

//! @brief First attempt to get min num ops to make every element equal to k
//! @param[in] nums Vector of ints
//! @param[in] k    Number to make all elements in nums equal to
//! @return Min number of operations to make every element in nums equal to k
static int minOperationsFA(std::vector<int> nums, int k)
{
    //! @details https://leetcode.com/problems
    //!          /minimum-operations-to-make-array-values-equal-to-k
    //!
    //!          Time complexity O(N * log N) where N = nums.size() for set
    //!          creation.
    //!          Space complexity O(N) worst case if all elements in nums are
    //!          unique.

    int           min_ops {};
    std::set<int> unique_vals(nums.begin(), nums.end());

    for (const int val : unique_vals)
    {
        if (val < k)
        {
            return -1;
        }

        if (val > k)
        {
            ++min_ops;
        }
    }

    return min_ops;

} // static int minOperationsFA( ...

//! @brief Hash set discussion solution
//! @param[in] nums Vector of ints
//! @param[in] k    Number to make all elements in nums equal to
//! @return Min number of operations to make every element in nums equal to k
static int minOperationsDS1(std::vector<int> nums, int k)
{
    //! @details https://leetcode.com/problems
    //!          /minimum-operations-to-make-array-values-equal-to-k/editorial/
    //!
    //!          Time complexity O(N) where N = nums.size() to traverse nums.
    //!          Space complexity O(N) for the hash set.

    std::unordered_set<int> unique_vals {};

    for (const int num : nums)
    {
        if (num < k)
        {
            return -1;
        }

        if (num > k)
        {
            unique_vals.insert(nums);
        }
    }

    return static_cast<int>(std::ssize(unique_vals));
}

TEST(MinOperationsTest, SampleTest1)
{
    EXPECT_EQ(2, minOperationsFA({5, 2, 5, 4, 5}, 2));
    EXPECT_EQ(2, minOperationsDS1({5, 2, 5, 4, 5}, 2));
}

TEST(MinOperationsTest, SampleTest2)
{
    EXPECT_EQ(-1, minOperationsFA({2, 1, 2}, 2));
    EXPECT_EQ(-1, minOperationsDS1({2, 1, 2}, 2));
}

TEST(MinOperationsTest, SampleTest3)
{
    EXPECT_EQ(4, minOperationsFA({9, 7, 5, 3}, 1));
    EXPECT_EQ(4, minOperationsDS1({9, 7, 5, 3}, 1));
}
