#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

//! @brief Brute force discussion solution to get longest consecutive seq length
//! @param[in] nums Reference to vector of unsorted integers
//! @return Length of the longest consecutive elements sequence
static int longestConsecutiveDS1(const std::vector<int>& nums)
{
    //! @details https://leetcode.com/problems/longest-consecutive-sequence

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

TEST(LongestConsecutiveTest, SampleTest1)
{
    const std::vector<int> nums {100, 4, 200, 1, 3, 2};

    EXPECT_EQ(4, longestConsecutiveDS1(nums));
}

TEST(LongestConsecutiveTest, SampleTest2)
{
    const std::vector<int> nums {0, 3, 7, 2, 5, 8, 4, 6, 0, 1};

    EXPECT_EQ(9, longestConsecutiveDS1(nums));
}

TEST(LongestConsecutiveTest, SampleTest3)
{
    const std::vector<int> nums {1, 0, 1, 2};

    EXPECT_EQ(3, longestConsecutiveDS1(nums));
}
