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

//! @brief Recursive binary search discussion solution
//! @param[in] nums1 Sorted vector of size m
//! @param[in] nums2 Sorted vector of size n
//! @return Median of sorted vectors in O(log (m + n))
static double findMedianSortedArraysDS2(std::vector<int> nums1,
                                        std::vector<int> nums2)
{
    //! @details leetcode.com/problems/median-of-two-sorted-arrays/editorial
    //!
    //!          Time complexity O(log (m * n)) where m = nums1.size() and
    //!          n = nums2.size(). At each step, cut half from either nums1 or
    //!          nums2. If one of the vectors is empty, can get the target from
    //!          the other in constant time. Therefore, the total time spent
    //!          depends on when one of the vectors is cut into an empty vector.
    //!          In the worst case, need to cut both vectors before finding the
    //!          target element. It take logarithmic time to empty a vector and
    //!          the times to empty two vectors are independent of each other.
    //!          The time complexity is O(log m + log n) = O(log (m * n)).
    //!          Space complexity O(log m + log n). The recursion depends on the
    //!          number of iterations to cut a vector into an empty vector. In
    //!          the worst case, we need O(log m + log n) steps.

    const auto nums1_len = static_cast<int>(std::ssize(nums1));
    const auto nums2_len = static_cast<int>(std::ssize(nums2));
    const auto total_len = nums1_len + nums2_len;

    const std::function<int(int, int, int, int, int)> solve =
        [&](int kth_smallest_idx, int start1, int end1, int start2, int end2) {
            //! If segment of one vector is empty, have passed all its elements.
            //! Return corresponding element in other vector.
            if (end1 < start1)
            {
                return nums2[kth_smallest_idx - start1];
            }

            if (end2 < start2)
            {
                return nums1[kth_smallest_idx - start2];
            }

            //! Get middle indices and values of nums1 and nums2
            const int mid1_idx {(start1 + end1) / 2};
            const int mid2_idx {(start2 + end2) / 2};
            const int mid1_val {nums1[mid1_idx]};
            const int mid2_val {nums2[mid2_idx]};

            //! If kth_smallest_idx is in the right half of nums1 + nums2 then
            //! remove the smaller left half
            if (mid1_idx + mid2_idx < kth_smallest_idx)
            {
                if (mid1_val > mid2_val)
                {
                    return solve(kth_smallest_idx,
                                 start1,
                                 end1,
                                 mid2_idx + 1,
                                 end2);
                }

                return solve(kth_smallest_idx,
                             mid1_idx + 1,
                             end1,
                             start2,
                             end2);
            }

            //! Otherwise, remove the larger half
            if (mid1_val > mid2_val)
            {
                return solve(kth_smallest_idx,
                             start1,
                             mid1_idx - 1,
                             start2,
                             end2);
            }

            return solve(kth_smallest_idx,
                         start1,
                         end1,
                         start2,
                         mid2_idx - 1);

        }; // const std::function<int(int, int, int, int, int)> solve = ...

    if (total_len % 2 == 1)
    {
        return 1.0 * solve(total_len / 2, 0, nums1_len - 1, 0, nums2_len - 1);
    }

    return (solve(total_len / 2 - 1, 0, nums1_len - 1, 0, nums2_len - 1)
            + solve(total_len / 2, 0, nums1_len - 1, 0, nums2_len - 1)) * 0.5;

} // static double findMedianSortedArraysDS2( ...

TEST(FindMedianSortedArraysTest, SampleTest1)
{
    EXPECT_EQ(2.0, findMedianSortedArraysFA({1, 3}, {2}));
    EXPECT_EQ(2.0, findMedianSortedArraysDS1({1, 3}, {2}));
    EXPECT_EQ(2.0, findMedianSortedArraysDS2({1, 3}, {2}));
}

TEST(FindMedianSortedArraysTest, SampleTest2)
{
    EXPECT_EQ(2.5, findMedianSortedArraysFA({1, 2}, {3, 4}));
    EXPECT_EQ(2.5, findMedianSortedArraysDS1({1, 2}, {3, 4}));
    EXPECT_EQ(2.5, findMedianSortedArraysDS2({1, 2}, {3, 4}));
}
