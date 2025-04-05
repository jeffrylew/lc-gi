#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

//! @brief Implement segment tree
static void update(int idx, int val, std::vector<int>& tree, int size)
{
    //! Shift the index to the leaf
    idx += size;

    //! Update from leaf to root
    tree[idx] += val;

    while (idx > 1)
    {
        idx /= 2;
        tree[idx] = tree[idx * 2] + tree[idx * 2 + 1];
    }
}

[[nodiscard]] static int query(int                     left,
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

    //! Offset negative to non-negative
    constexpr int offset {1E4};

    //! Total possible values in nums
    constexpr int size {2 * 1E4 + 1};

    std::vector<int> tree(size * 2);
    std::vector<int> result;

    for (int idx = static_cast<int>(std::ssize(nums)); idx >= 0; --idx)
    {
        result.push_back(query(0, nums[idx] + offset, tree, size));
        update(nums[idx] + offset, 1, tree, size);
    }

    std::reverse(result.begin(), result.end());
    return result;

} // static std::vector<int> countSmallerDS1( ...

TEST(CountSmallerTest, SampleTest1)
{
    const std::vector<int> expected_output {2, 1, 1, 0};

    EXPECT_EQ(expected_output, countSmallerDS1({5, 2, 6, 1}));
}

TEST(CountSmallerTest, SampleTest2)
{
    const std::vector<int> expected_output {0};

    EXPECT_EQ(expected_output, countSmallerDS1({-1}));
}

TEST(CountSmallerTest, SampleTest3)
{
    const std::vector<int> expected_output {0, 0};

    EXPECT_EQ(expected_output, countSmallerDS1({-1, -1}));
}
