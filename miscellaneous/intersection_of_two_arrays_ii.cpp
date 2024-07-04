#include <gtest/gtest.h>

#include <unordered_map>
#include <vector>

//! @brief Get intersection of two vectors
//! @param[in] nums1 Vector of integers from 0 to 1000
//! @param[in] nums2 Vector of integers from 0 to 1000
//! @return Intersection of nums1 and nums2
static std::vector<int> intersectFA(std::vector<int> nums1,
                                    std::vector<int> nums2)
{
    //! @details https://leetcode.com/problems/intersection-of-two-arrays-ii
    //!
    //!          Time complexity O(N + M) where N = nums1.size() and
    //!          M = nums2.size()
    //!          Space complexity O(N) for nums1_counts. Output vector
    //!          intersection is not considered, although this would be
    //!          O(min(N, M))

    std::vector<int> intersection {};

    std::unordered_map<int, int> nums1_counts {};
    for (const int num : nums1)
    {
        ++nums1_counts[num];
    }

    for (const int num : nums2)
    {
        auto num_it = nums1_counts.find(num);
        if (num_it == nums1_counts.end())
        {
            continue;
        }

        if (--num_it->second == 0)
        {
            nums1_counts.erase(num_it);
        }
        intersection.push_back(num);
    }

    return intersection;

} // static std::vector<int> intersectFA( ...

TEST(IntersectTest, SampleTest1)
{
    const std::vector<int> nums1 {1, 2, 2, 1};
    const std::vector<int> nums2 {2, 2};
    const std::vector<int> expected_output {2, 2};

    EXPECT_EQ(expected_output, intersectFA(nums1, nums2));
}

TEST(IntersectTest, SampleTest2)
{
    const std::vector<int> nums1 {4, 9, 5};
    const std::vector<int> nums2 {9, 4, 9, 8, 4};
    const std::vector<int> expected_output {4, 9};

    EXPECT_EQ(expected_output, intersectFA(nums1, nums2));
}
