#include <gtest/gtest.h>

#include <unordered_map>
#include <vector>

//! @brief First attempt to get majority element (appears more than N/2 times)
//! @param[in] nums Vector of size N
//! @return The majority element in nums
static int majorityElementFA(const std::vector<int>& nums)
{
    //! @details https://leetcode.com/problems/majority-element
    //!
    //!          Time complexity O(N) where N = nums.size().
    //!          Space complexity O(N) in the worst case where the first N/2 - 1
    //!          elements are unique and the last N/2 + 1 elements are the same
    //!          majority element.

    const auto half_nums_size = static_cast<int>(std::ssize(nums) / 2);

    //! Map of <num, count>
    std::unordered_map<int, int> num_count;

    for (const int num : nums)
    {
        ++num_count[num];

        if (num_count[num] > half_nums_size)
        {
            return num;
        }
    }

    return 0;
}

//! @brief Brute force discussion solution
//! @param[in] nums Vector of size N
//! @return The majority element in nums
static int majorityElementDS1(const std::vector<int>& nums)
{
    //! @details https://leetcode.com/problems/majority-element/editorial
    //!
    //!          Time complexity O(N ^ 2) where N = nums.size(). The brute force
    //!          algorithm contains two nested for loops that each run for N
    //!          iterations, adding up to quadratic time complexity.
    //!          Space complexity O(1).

    const auto majority_count = static_cast<int>(std::ssize(nums) / 2);

    for (const int start_num : nums)
    {
        int curr_count {};

        for (const int curr_num : nums)
        {
            if (curr_num == start_num)
            {
                ++curr_count;
            }
        }

        if (curr_count > majority_count)
        {
            return start_num;
        }
    }

    return -1;
}

//! @brief Hashmap discussion solution
//! @param[in] nums Vector of size N
//! @return The majority element in nums
static int majorityElementDS2(const std::vector<int>& nums)
{
    //! @details https://leetcode.com/problems/majority-element/editorial
    //!
    //!          Time complexity O(N) where N = nums.size(). We iterate over
    //!          nums once and make constant time insertions.
    //!          Space complexity O(N). At most, the map can contain
    //!          N - floor(N / 2) elements. nums is guaranteed to contain a
    //!          majority element, which occupies floor(n / 2) + 1 indices in
    //!          nums at minimum. Therefore, N - (floor(N / 2) + 1) indices in
    //!          nums can be occupied by distinct elements.

    std::unordered_map<int, int> num_counts;
    for (const int num : nums)
    {
        ++num_counts[num];
    }

    for (const auto& [num, count] : num_counts)
    {
        if (count > std::ssize(nums) / 2)
        {
            return num;
        }
    }

    return 0;
}

TEST(MajorityElementTest, SampleTest1)
{
    const std::vector<int> nums {3, 2, 3};

    EXPECT_EQ(3, majorityElementFA(nums));
    EXPECT_EQ(3, majorityElementDS1(nums));
    EXPECT_EQ(3, majorityElementDS2(nums));
}

TEST(MajorityElementTest, SampleTest2)
{
    const std::vector<int> nums {2, 2, 1, 1, 1, 2, 2};

    EXPECT_EQ(2, majorityElementFA(nums));
    EXPECT_EQ(2, majorityElementDS1(nums));
    EXPECT_EQ(2, majorityElementDS2(nums));
}
