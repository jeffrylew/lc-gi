#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

//! @brief Brute force discussion solution to get longest consecutive seq length
//! @param[in] nums Reference to vector of unsorted integers
//! @return Length of the longest consecutive elements sequence
static int longestConsecutiveDS1(const std::vector<int>& nums)
{
    //! @details https://leetcode.com/problems/longest-consecutive-sequence
    //!
    //!          Time complexity O(N ^ 3) where N = nums.size(). The outer loop
    //!          runs N times and curr_num increments by 1 during each iteration
    //!          so it runs in O(N). During each iteration of the while loop, an
    //!          O(N) lookup in nums is performed.
    //!          Space complexity O(1).

    const auto nums_contains = [&](int query_num) -> bool {
        return std::ranges::find(nums, query_num) != nums.end();
    };

    int longest_streak_len {};

    for (const int num : nums)
    {
        int curr_num        = num;
        int curr_streak_len = 1;

        while (nums_contains(curr_num + 1))
        {
            ++curr_num;
            ++curr_streak_len;
        }

        longest_streak_len = std::max(longest_streak_len, curr_streak_len);
    }

    return longest_streak_len;
}

//! @brief Sorting discussion solution to get longest consecutive seq length
//! @param[in] nums Reference to vector of unsorted integers
//! @return Length of the longest consecutive elements sequence
static int longestConsecutiveDS2(const std::vector<int>& nums)
{
    //! @details leetcode.com/problems/longest-consecutive-sequence/editorial

    if (nums.empty())
    {
        return 0;
    }

    auto sorted_nums = nums;
    std::ranges::sort(sorted_nums);

    int longest_streak_len {1};
    int curr_streak_len {1};

    for (int idx = 1; idx < std::ssize(sorted_nums); ++idx)
    {
        if (sorted_nums[idx] == sorted_nums[idx - 1])
        {
            //! Current sequence is neither extended nor broken so move on
            continue;
        }

        if (sorted_nums[idx] == sorted_nums[idx - 1] + 1)
        {
            ++curr_streak_len;
        }
        else
        {
            //! Sequence broke, reset curr_streak_len to 1 for num that broke it
            longest_streak_len = std::max(longest_streak_len, curr_streak_len);
            curr_streak_len    = 1;
        }
    }

    //! The last element of nums could be part of the longest sequence
    return std::max(longest_streak_len, curr_streak_len);
}

TEST(LongestConsecutiveTest, SampleTest1)
{
    const std::vector<int> nums {100, 4, 200, 1, 3, 2};

    EXPECT_EQ(4, longestConsecutiveDS1(nums));
    EXPECT_EQ(4, longestConsecutiveDS2(nums));
}

TEST(LongestConsecutiveTest, SampleTest2)
{
    const std::vector<int> nums {0, 3, 7, 2, 5, 8, 4, 6, 0, 1};

    EXPECT_EQ(9, longestConsecutiveDS1(nums));
    EXPECT_EQ(9, longestConsecutiveDS2(nums));
}

TEST(LongestConsecutiveTest, SampleTest3)
{
    const std::vector<int> nums {1, 0, 1, 2};

    EXPECT_EQ(3, longestConsecutiveDS1(nums));
    EXPECT_EQ(3, longestConsecutiveDS2(nums));
}
