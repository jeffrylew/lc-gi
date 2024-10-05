#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <vector>

//! @brief First attempt to see if can reach last index
//! @param[in] nums Vector of max jump lengths
//! @return True if can reach the last index, else false
static bool canJumpFA(std::vector<int> nums)
{
    //! @details https://leetcode.com/problems/jump-game/description/
    //!
    //!          Time complexity O(N) where N = nums.size()
    //!          Space complexity O(1)

    const auto nums_size = static_cast<int>(std::ssize(nums));
    if (nums_size == 1)
    {
        return true;
    }

    if (nums[0] == 0)
    {
        return false;
    }

    for (int idx = 1; idx < nums_size - 1; ++idx)
    {
        nums[idx] = std::max(idx + nums[idx], nums[idx - 1]);

        if (nums[idx] < idx + 1)
        {
            return false;
        }
    }

    return nums[nums_size - 2] >= (nums_size - 1);

} // static bool canJumpFA( ...

//! @brief Backtracking discussion solution (not accepted: TLE)
//! @param[in] nums Vector of max jump lengths
//! @return True if can reach the last index, else false
static bool canJumpDS1(std::vector<int> nums)
{
    //! @details https://leetcode.com/problems/jump-game/description/
    //!
    //!          Time complexity O(2 ^ N) where N = nums.size(). There are 2 ^ N
    //!          (upper bound) ways of jumping from the first position to last.
    //!          Space complexity O(N). Recursion requires memory for stack.

    const auto nums_size = static_cast<int>(std::ssize(nums));

    //! Try every jump pattern from first position to the last
    const std::function<bool(int)> can_jump_from_position = [&](int position) {
        if (position == nums_size - 1)
        {
            return true;
        }

        const int furthest_jump {
            std::min(position + nums[position], nums_size - 1)};

        /*
         * Optimization: Check next_position from right to left - i.e. try to
         * make the biggest jump to reach the end as soon as possible.
         * The theoretical worst case performance is the same though. If last
         * index is unreachable, all combinations will be tried.
         *
        for (int next_position = position + 1;
             next_position <= furthest_jump;
             ++next_position)
         */
        for (int next_position = furthest_jump;
             next_position > position;
             --next_position)
        {
            if (can_jump_from_position(next_position))
            {
                return true;
            }
        }

        return false;
    };

    return can_jump_from_position(0);

} // static bool canJumpDS1( ...

//! @brief Top down dynamic programming discussion solution
//! @param[in] nums Vector of max jump lengths
//! @return True if can reach the last index, else false
static bool canJumpDS2(std::vector<int> nums)
{
    //! @details https://leetcode.com/problems/jump-game/description/
    //!
    //!          Time complexity O(N ^ 2) where N = nums.size(). For every
    //!          element in nums, we look at nums[i] elements to its right to
    //!          find a GOOD index. nums[i] can be at most N.
    //!          Space complexity O(2 * N) = O(N). First N originates from
    //!          recursion and the second N comes from the memo vector.

    enums class Index
    {
        UNKNOWN,
        GOOD,
        BAD
    };

    const auto nums_size = static_cast<int>(std::ssize(nums));

    std::vector<Index> memo(nums.size());
    memo.back() = Index::GOOD;

    const std::function<bool(int)> can_jump_from_position = [&](int position) {
        if (memo[position] != Index::UNKNOWN)
        {
            return memo[position] == Index::GOOD ? true : false;
        }

        const int furthest_jump {
            std::min(position + nums[position], nums_size - 1)};

        for (int next_position = position + 1;
             next_position <= furthest_jump;
             ++next_position)
        {
            if (can_jump_from_position(next_position))
            {
                memo[position] = Index::GOOD;
                return true;
            }
        }

        memo[position] = Index::BAD;
        return false;
    };

    return can_jump_from_position(0);

} // static bool canJumpDS2( ...

//! @brief Bottom up dynamic programming discussion solution
//! @param[in] nums Vector of max jump lengths
//! @return True if can reach the last index, else false
static bool canJumpDS3(std::vector<int> nums)
{
    //! @details https://leetcode.com/problems/jump-game/description/
    //!
    //!          Time complexity O(N ^ 2) where N = nums.size(). For every
    //!          element in nums, we look at nums[i] elements to its right to
    //!          find a GOOD index. nums[i] can be at most N.
    //!          Space complexity O(N) for usage of memo vector

    enum class Index
    {
        UNKNOWN,
        GOOD,
        BAD
    };

    const auto nums_size = static_cast<int>(std::ssize(nums));

    std::vector<Index> memo(nums.size());
    memo.back() = Index::GOOD;

    for (int curr_idx = nums_size - 2; curr_idx >= 0; --curr_idx)
    {
        const int furthest_jump {
            std::min(curr_idx + nums[curr_idx], nums_size - 1)};

        for (int next_idx = curr_idx + 1; next_idx <= furthest_jump; ++next_idx)
        {
            if (memo[next_idx] == Index::GOOD)
            {
                memo[curr_idx] = Index::GOOD;
                break;
            }
        }
    }

    return memo[0] == Index::GOOD;

} // static bool canJumpDS3( ...

TEST(CanJumpTest, SampleTest1)
{
    EXPECT_TRUE(canJumpFA({2, 3, 1, 1, 4}));
    EXPECT_TRUE(canJumpDS1({2, 3, 1, 1, 4}));
    EXPECT_TRUE(canJumpDS2({2, 3, 1, 1, 4}));
    EXPECT_TRUE(canJumpDS3({2, 3, 1, 1, 4}));
}

TEST(CanJumpTest, SampleTest2)
{
    EXPECT_FALSE(canJumpFA({3, 2, 1, 0, 4}));
    EXPECT_FALSE(canJumpDS1({3, 2, 1, 0, 4}));
    EXPECT_FALSE(canJumpDS2({3, 2, 1, 0, 4}));
    EXPECT_FALSE(canJumpDS3({3, 2, 1, 0, 4}));
}
