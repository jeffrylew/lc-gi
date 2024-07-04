#include <gtest/gtest.h>

#include <algorithm>
#include <unordered_map>
#include <vector>

//! @brief First attempt solution to get intersection of two vectors
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

//! @brief Hash map discussion solution to get intersection of two vectors
//! @param[in] nums1 Vector of integers from 0 to 1000
//! @param[in] nums2 Vector of integers from 0 to 1000
//! @return Intersection of nums1 and nums2
static std::vector<int> intersectDS1(std::vector<int> nums1,
                                     std::vector<int> nums2)
{
    //! @details https://leetcode.com/problems/intersection-of-two-arrays-ii
    //!
    //!          Time complexity O(N + M) where N = nums1.size() and
    //!          M = nums2.size().
    //!          Space complexity O(min(N, M)). Use a hash map to store numbers
    //!          and their counts from the smaller vector

    //! Check which is smaller so can create hash map for smaller vector
    if (nums1.size() > nums2.size())
    {
        nums1.swap(nums2);
    }

    std::unordered_map<int, int> nums1_counts {};
    for (const int num : nums1)
    {
        ++nums1_counts[num];
    }

    int output_idx {};
    for (const int num : nums2)
    {
        auto it = nums1_counts.find(num);
        if (it != nums1_counts.end() && --it->second >= 0)
        {
            nums1[output_idx++] = num; 
        }
    }

    return std::vector<int>(nums1.begin(), nums1.begin() + output_idx);

} // static std::vector<int> intersectDS1( ...

//! @brief Sort discussion solution to get intersection of two vectors
//! @param[in] nums1 Vector of integers from 0 to 1000
//! @param[in] nums2 Vector of integers from 0 to 1000
//! @return Intersection of nums1 and nums2
static std::vector<int> intersectDS2(std::vector<int> nums1,
                                     std::vector<int> nums2)
{
    //! @details https://leetcode.com/problems/intersection-of-two-arrays-ii
    //!
    //!          Time complexity O(N * log N + M * log M) where N = nums1.size()
    //!          and M = nums2.size(). We sort two vectors and do a linear scan.
    //!          Space complexity O(log N + log M) for C++'s std::sort.

    std::sort(nums1.begin(), nums1.end());
    std::sort(nums2.begin(), nums2.end());

    int nums1_idx {};
    int nums2_idx {};
    int output_idx {};

    while (nums1_idx < static_cast<int>(nums1.size())
           && nums2_idx < static_cast<int>(nums2.size()))
    {
        if (nums1[nums1_idx] < nums2[nums2_idx])
        {
            ++nums1_idx;
        }
        else if (nums1[nums1_idx] > nums2[nums2_idx])
        {
            ++nums2_idx;
        }
        else
        {
            nums1[output_idx++] = nums1[nums1_idx++];
            ++nums2_idx;
        }
    }

    return std::vector<int>(nums1.begin(), nums1.begin() + output_idx);

} // static std::vector<int> intersectDS2( ...

//! @brief Built-in intersection discussion solution
//! @param[in] nums1 Vector of integers from 0 to 1000
//! @param[in] nums2 Vector of integers from 0 to 1000
//! @return Intersection of nums1 and nums2
static std::vector<int> intersectDS3(std::vector<int> nums1,
                                     std::vector<int> nums2)
{
    //! @details https://leetcode.com/problems/intersection-of-two-arrays-ii
    //!
    //!          Time complexity O(N * log N + M * log M) where N = nums1.size()
    //!          and M = nums2.size() to sort two vectors. std::set_intersection
    //!          takes at most O(2 * (N + M)).
    //!          Space complexity O(log N + log M) for C++'s std::sort.

    std::sort(nums1.begin(), nums1.end());
    std::sort(nums2.begin(), nums2.end());

    nums1.erase(std::set_intersection(nums1.begin(),
                                      nums1.end(),
                                      nums2.begin(),
                                      nums2.end(),
                                      nums1.begin()),
                nums1.end());

    return nums1;

} // static std::vector<int> intersectDS3( ...

TEST(IntersectTest, SampleTest1)
{
    const std::vector<int> nums1 {1, 2, 2, 1};
    const std::vector<int> nums2 {2, 2};
    const std::vector<int> expected_output {2, 2};

    EXPECT_EQ(expected_output, intersectFA(nums1, nums2));
    EXPECT_EQ(expected_output, intersectDS1(nums1, nums2));
    EXPECT_EQ(expected_output, intersectDS2(nums1, nums2));
    EXPECT_EQ(expected_output, intersectDS3(nums1, nums2));
}

TEST(IntersectTest, SampleTest2)
{
    const std::vector<int> nums1 {4, 9, 5};
    const std::vector<int> nums2 {9, 4, 9, 8, 4};
    const std::vector<int> expected_output {4, 9};

    EXPECT_EQ(expected_output, intersectFA(nums1, nums2));
    EXPECT_EQ(expected_output, intersectDS1(nums1, nums2));
    EXPECT_EQ(expected_output, intersectDS2(nums1, nums2));
    EXPECT_EQ(expected_output, intersectDS3(nums1, nums2));
}
