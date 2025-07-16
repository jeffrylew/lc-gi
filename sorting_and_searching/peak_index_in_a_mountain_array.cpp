#include <gtest/gtest.h>

#include <vector>

//! @brief First attempt to get index of peak element
//! @param[in] arr Vector of ints forming a mountain
//! @return Index of the peak element
static int peakIndexInMountainArrayFA(std::vector<int> arr)
{
    //! @details https://leetcode.com/explore/interview/card/google/63
    //!          /sorting-and-searching-4/3084/
    //!
    //!          Time complexity O(log N) where N = arr.size() for binary search
    //!          Space complexity O(1)

    const auto arr_size = static_cast<int>(std::ssize(arr));

    int left_idx {};
    int peak_idx {arr.front() >= arr.back() ? 0 : arr_size - 1};
    int right_idx {arr_size - 1};

    while (left_idx < right_idx)
    {
        const int mid_idx {left_idx + (right_idx - left_idx) / 2};

        const int prev_elem {arr[mid_idx - 1]};
        const int curr_elem {arr[mid_idx]};
        const int next_elem {arr[mid_idx + 1]};

        if (prev_elem < curr_elem && next_elem < curr_elem)
        {
            peak_idx = mid_idx;
            break;
        }

        if (prev_elem < next_elem)
        {
            left_idx = mid_idx;
        }
        else
        {
            right_idx = mid_idx;
        }
    }

    return peak_idx;
}

//! @brief Linear scan discussion solution
//! @param[in] arr Vector of ints forming a mountain
//! @return Index of the peak element
static int peakIndexInMountainArrayDS1(std::vector<int> arr)
{
    //! @details https://leetcode.com/problems/peak-index-in-a-mountain-array
    //!
    //!          Time complexity O(N). We do a linear scan and compare adjacent
    //!          elements until we get the peak of the mountain. In the worst
    //!          case, the peak is the second-to-last element of arr which would
    //!          result in O(N).
    //!          Space complexity O(1) for the integer idx.

    int idx {};

    while (arr[idx] < arr[idx + 1])
    {
        ++idx;
    }

    return idx;
}

TEST(PeakIndexInMountainArrayTest, SampleTest1)
{
    EXPECT_EQ(1, peakIndexInMountainArrayFA({0, 1, 0}));
    EXPECT_EQ(1, peakIndexInMountainArrayDS1({0, 1, 0}));
}

TEST(PeakIndexInMountainArrayTest, SampleTest2)
{
    EXPECT_EQ(1, peakIndexInMountainArrayFA({0, 2, 1, 0}));
    EXPECT_EQ(1, peakIndexInMountainArrayDS1({0, 2, 1, 0}));
}

TEST(PeakIndexInMountainArrayTest, SampleTest3)
{
    EXPECT_EQ(1, peakIndexInMountainArrayFA({0, 10, 5, 2}));
    EXPECT_EQ(1, peakIndexInMountainArrayDS1({0, 10, 5, 2}));
}
