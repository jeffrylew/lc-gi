#include <gtest/gtest.h>

#include <functional>
#include <queue>
#include <vector>

//! @brief Get median of two sorted vectors of size m and n in O(log (m + n))
//! @param[in] nums1 Sorted vector of size m
//! @param[in] nums2 Sorted vector of size n
//! @return Median of sorted vectors in O(log (m + n))
static double findMedianSortedArraysFA(std::vector<int> nums1,
                                       std::vector<int> nums2)
{
    //! @details https://leetcode.com/problems/median-of-two-sorted-arrays
    //!
    //!          Time complexity O(log (m + n)) where m = nums1.size() and
    //!          n = nums2.size(). Need to insert all elements from nums1 and
    //           nums2 into either min_heap or max_heap. For m + n insertions,
    //!          std::priority_queue::push has time complexity O(log (m + n)).
    //!          The number of pops from the larger priority queue will be less
    //!          than m + n so its contribution to time complexity is ignored.
    //!          Space complexity O(m + n) to store all elements from nums1 and
    //!          nums2 into the two priority_queues.

    std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap;

    std::priority_queue<int> max_heap;

    const auto nums1_len = static_cast<int>(std::ssize(nums1));
    const auto nums2_len = static_cast<int>(std::ssize(nums2));

    int idx1 {};
    int idx2 {};

    const auto balance_heaps = [&] {
        if (std::size(min_heap) > std::ssize(max_heap))
        {
            max_heap.push(min_heap.top());
            min_heap.pop();
        }
    };

    while (idx1 < nums1_len && idx2 < nums2_len)
    {
        if (nums1[idx1] < nums2[idx2])
        {
            min_heap.push(nums1[idx1++]);
        }
        else
        {
            min_heap.push(nums2[idx2++]);
        }

        balance_heaps();
    }

    while (idx1 < nums1_len)
    {
        min_heap.push(nums1[idx1++]);

        balance_heaps();
    }

    while (idx2 < nums2_len)
    {
        min_heap.push(nums2[idx2++]);

        balance_heaps();
    }

    //! Don't need to worry about integer overflow since converting to double
    const auto min_heap_top = static_cast<double>(min_heap.top());
    const auto max_heap_top = static_cast<double>(max_heap.top());

    if (std::ssize(max_heap) > std::ssize(min_heap))
    {
        return max_heap_top;
    }

    return (max_heap_top + min_heap_top) / 2.0;

} // static double findMedianSortedArraysFA( ...

//! @brief Merge sort discussion solution
//! @param[in] nums1 Sorted vector of size m
//! @param[in] nums2 Sorted vector of size n
//! @return Median of sorted vectors in O(log (m + n))
static double findMedianSortedArraysDS1(std::vector<int> nums1,
                                        std::vector<int> nums2)
{
    //! @details leetcode.com/problems/median-of-two-sorted-arrays/editorial
    //!
    //!          Time complexity O(m + n) where m = nums1.size() and
    //!          n = nums2.size(). Get smallest element from values at idx1 and
    //!          idx2. Need to traverse half of the vectors before reaching the
    //!          median element(s).
    //!          Space complexity O(1) for two pointers idx1 and idx2.

    const auto nums1_len = static_cast<int>(std::ssize(nums1));
    const auto nums2_len = static_cast<int>(std::ssize(nums2));

    int idx1 {};
    int idx2 {};

    //! Get smaller value between nums1[idx1] and nums2[idx2] and increment idx
    const auto get_min = [&] {
        if (idx1 < nums1_len && idx2 < nums2_len)
        {
            return nums1[idx1] < nums2[idx2] ? nums1[idx1++] : nums2[idx2++];
        }
        else if (idx1 < nums1_len)
        {
            return nums1[idx1++];
        }
        else if (idx2 < nums2_len)
        {
            return nums2[idx2++];
        }

        return -1;
    };

    if ((nums1_len + nums2_len) % 2 == 0)
    {
        for (int idx = 0; idx < (nums1_len + nums2_len) / 2 - 1; ++idx)
        {
            get_min();
        }

        return static_cast<double>(get_min() + get_min()) / 2.0;
    }

    for (int idx = 0; idx < (nums1_len + nums2_len) / 2; ++idx)
    {
        get_min();
    }

    return get_min();

} // static double findMedianSortedArraysDS1( ...

TEST(FindMedianSortedArraysTest, SampleTest1)
{
    EXPECT_EQ(2.0, findMedianSortedArraysFA({1, 3}, {2}));
    EXPECT_EQ(2.0, findMedianSortedArraysDS1({1, 3}, {2}));
}

TEST(FindMedianSortedArraysTest, SampleTest2)
{
    EXPECT_EQ(2.5, findMedianSortedArraysFA({1, 2}, {3, 4}));
    EXPECT_EQ(2.5, findMedianSortedArraysDS1({1, 2}, {3, 4}));
}
