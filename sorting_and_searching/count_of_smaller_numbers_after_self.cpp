#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

//! @brief Implement segment tree
static void update_DS1(int idx, int val, std::vector<int>& tree, int size)
{
    //! Shift the index to the leaf
    idx += size;

    //! Update from leaf to root
    tree[idx] += val;

    while (idx > 1)
    {
        idx /= 2;
        tree[idx] = tree[2 * idx] + tree[2 * idx + 1];
    }
}

[[nodiscard]] static int query_DS1(int                     left,
                                   int                     right,
                                   const std::vector<int>& tree,
                                   int                     size)
{
    //! Return sum of [left, right)
    int result {};

    //! Shift the index to the leaf
    left += size;
    right += size;

    while (left < right)
    {
        //! If left is a right node, bring value and move to parent's right node
        if (left % 2 == 1)
        {
            result += tree.at(left);
            ++left;
        }

        //! Else directly move to parent
        left /= 2;

        //! If right is right node, bring value of left node and move to parent
        if (right % 2 == 1)
        {
            --right;
            result += tree.at(right);
        }

        //! Else directly move to parent
        right /= 2;
    }

    return result;
}

//! @brief Segment Tree discussion solution
//! @param[in] nums Vector of integers
//! @return Vector where counts[i] is num smaller elements to right of nums[i]
static std::vector<int> countSmallerDS1(std::vector<int> nums)
{
    //! @details https://leetcode.com/explore/interview/card/google/63
    //!          /sorting-and-searching-4/3083/
    //!
    //!          Time complexity O(N * log M) where N = nums.size() and M = the
    //!          difference between the max and min values in nums. In the above
    //!          implementations, we fix M = 2 * 10 ^ 4. We need to iterate over
    //!          nums. For each element, finding the number of smaller elements
    //!          after it takes O(log M) and updating the counts takes O(log M).
    //!          In total, we need O(N * log M).
    //!          Space complexity O(M) to store the segment tree in a vector of
    //!          size up to 2 * M + 2. We need at most M + 1 buckets (the +1 is
    //!          for the value of 0). For the segment tree, we need twice the
    //!          number of buckets, which is 2 * (M + 1) = 2 * M + 2.

    //! Offset negative to non-negative
    constexpr int offset {10000};

    //! Total possible values in nums
    constexpr int size {2 * 10000 + 1};

    //! Since segment tree is initialized with all zeros, only need to implement
    //! update and query
    std::vector<int> tree(size * 2);
    std::vector<int> result;

    for (int idx = static_cast<int>(std::ssize(nums)) - 1; idx >= 0; --idx)
    {
        //! Query the number of elements in segment tree smaller than nums[idx]
        result.push_back(query_DS1(0, nums[idx] + offset, tree, size));

        //! Update the count of nums[idx] in the segment tree
        update_DS1(nums[idx] + offset, 1, tree, size);
    }

    std::reverse(result.begin(), result.end());
    return result;

} // static std::vector<int> countSmallerDS1( ...

//! Implement Binary Index Tree
static void update_DS2(int idx, int val, std::vector<int>& tree, int size)
{
    //! Index in BIT is 1 more than the original index
    ++idx;

    while (idx < size)
    {
        tree[idx] += val;
        index += index & -index;
    }
}

[[nodiscard]] static int query_DS2(int idx, const std::vector<int>& tree)
{
    //! Return sum of [0, idx)
    int result {};

    while (idx >= 1)
    {
        result += tree.at(idx);
        idx -= idx & -idx;
    }

    return result;
}

//! @brief Binary Indexed Tree (Fenwick Tree) discussion solution
//! @param[in] nums Vector of integers
//! @return Vector where counts[i] is num smaller elements to right of nums[i]
static std::vector<int> countSmallerDS2(std::vector<int> nums)
{
    //! @details https://leetcode.com/problems
    //!          /count-of-smaller-numbers-after-self/editorial/

    //! Offset negative to non-negative
    constexpr int offset {10000};

    //! Total possible values in nums plus one dummy
    constexpr int size {2 * 10000 + 2};

    std::vector<int> tree(size);
    std::vector<int> result;

    for (int idx = static_cast<int>(std::ssize(nums)) - 1; idx >= 0; --idx)
    {
        result.push_back(query_DS2(nums[idx] + offset, tree));

        update_DS2(nums[idx] + offset, 1, tree, size);
    }

    std::reverse(result.begin(), result.end());
    return result;

} // static std::vector<int> countSmallerDS2( ...

TEST(CountSmallerTest, SampleTest1)
{
    const std::vector<int> expected_output {2, 1, 1, 0};

    EXPECT_EQ(expected_output, countSmallerDS1({5, 2, 6, 1}));
    EXPECT_EQ(expected_output, countSmallerDS2({5, 2, 6, 1}));
}

TEST(CountSmallerTest, SampleTest2)
{
    const std::vector<int> expected_output {0};

    EXPECT_EQ(expected_output, countSmallerDS1({-1}));
    EXPECT_EQ(expected_output, countSmallerDS2({-1}));
}

TEST(CountSmallerTest, SampleTest3)
{
    const std::vector<int> expected_output {0, 0};

    EXPECT_EQ(expected_output, countSmallerDS1({-1, -1}));
    EXPECT_EQ(expected_output, countSmallerDS2({-1, -1}));
}
