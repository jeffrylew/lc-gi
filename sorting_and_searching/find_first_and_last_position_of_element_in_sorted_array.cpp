#include <gtest/gtest.h>

#include <vector>

//! @brief First attempt to find start and end position of target value
//! @param[in] nums   Vector of sorted ints in non-decreasing order
//! @param[in] target Value to find start and end position of
//! @return Vector containing start and end position of target, or [-1, -1]
static std::vector<int> searchRangeFA(std::vector<int> nums, int target)
{
    //! @details https://leetcode.com/explore/interview/card/google/63
    //!          /sorting-and-searching-4/3081/
    //!
    //!          Time complexity O(log N) where N = nums.size()
    //!          Space complexity O(1)

    if (nums.empty())
    {
        return {-1, -1};
    }

    const auto nums_size = static_cast<int>(std::ssize(nums));

    int start_pos {};
    int right_pos {nums_size};

    while (start_pos < right_pos)
    {
        const int mid {start_pos + (right_pos - start_pos) / 2};

        if (nums[mid] >= target)
        {
            right_pos = mid;
        }
        else
        {
            start_pos = mid + 1;
        }
    }

    if (start_pos == nums_size || nums[start_pos] != target)
    {
        return {-1, -1};
    }

    int end_pos {start_pos};
    right_pos = nums_size;

    while (end_pos < right_pos)
    {
        const int mid {end_pos + (right_pos - end_pos) / 2};

        if (nums[mid] > target)
        {
            right_pos = mid;
        }
        else
        {
            end_pos = mid + 1;
        }
    }

    return {start_pos, end_pos - 1};

} // static std::vector<int> searchRangeFA( ...

//! @brief Discussion solution to find start and end position of target value
//! @param[in] nums   Vector of sorted ints in non-decreasing order
//! @param[in] target Value to find start and end position of
//! @return Vector containing start and end position of target, or [-1, -1]
static std::vector<int> searchRangeDS(std::vector<int> nums, int target)
{
    //! @details https://leetcode.com/problems
    //!          /find-first-and-last-position-of-element-in-sorted-array
    //!
    //!          Time complexity O(log N) where N = nums.size()
    //!          Space complexity O(1)

    const auto find_bound = [&](bool searching_left) -> int {
        int bound_index {-1};

        int left {};
        int right {static_cast<int>(std::ssize(nums)) - 1};

        while (left <= right)
        {
            const int mid {left + (right - left) / 2};

            if (nums[mid] == target)
            {
                bound_index = mid;

                if (searching_left)
                {
                    right = mid - 1;
                }
                else
                {
                    left = mid + 1;
                }
            }
            else if (nums[mid] > target)
            {
                right = mid - 1;
            }
            else
            {
                left = mid + 1;
            }
        }

        return bound_index;
    };

    if (nums.empty())
    {
        return {-1, -1};
    }

    const int left_idx {find_bound(true)};
    if (left_idx == -1)
    {
        return {-1, -1};
    }

    return {left_idx, find_bound(false)};

} // static std::vector<int> searchRangeDS( ...

TEST(SearchRangeTest, SampleTest1)
{
    const std::vector<int> expected_output {3, 4};

    EXPECT_EQ(expected_output, searchRangeFA({5, 7, 7, 8, 8, 10}, 8));
    EXPECT_EQ(expected_output, searchRangeDS({5, 7, 7, 8, 8, 10}, 8));
}

TEST(SearchRangeTest, SampleTest2)
{
    const std::vector<int> expected_output {-1, -1};

    EXPECT_EQ(expected_output, searchRangeFA({5, 7, 7, 8, 8, 10}, 6));
    EXPECT_EQ(expected_output, searchRangeDS({5, 7, 7, 8, 8, 10}, 6));
}

TEST(SearchRangeTest, SampleTest3)
{
    const std::vector<int> expected_output {-1, -1};

    EXPECT_EQ(expected_output, searchRangeFA({}, 0));
    EXPECT_EQ(expected_output, searchRangeDS({}, 0));
}

TEST(SearchRangeTest, SampleTest4)
{
    const std::vector<int> expected_output {-1, -1};

    EXPECT_EQ(expected_output, searchRangeFA({2, 2}, 3));
    EXPECT_EQ(expected_output, searchRangeDS({2, 2}, 3));
}
