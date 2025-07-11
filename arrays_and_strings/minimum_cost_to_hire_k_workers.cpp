#include <gtest/gtest.h>

#include <algorithm>
#include <limits>
#include <queue>
#include <vector>

//! @brief Priority queue discussion solution
//! @param[in] quality Reference to vector of quality of workers
//! @param[in] wage    Reference to vector of minimum wage expectations
//! @param[in] k       Number of workers in paid group
//! @return Least amount of money needed to form a paid group
static double mincostToHireWorkersDS(const std::vector<int>& quality,
                                     const std::vector<int>& wage,
                                     int                     k)
{
    //! @details https://leetcode.com/explore/interview/card/google/59
    //!          /array-and-strings/3061
    //!          https://leetcode.com/problems/minimum-cost-to-hire-k-workers
    //!
    //!          Time complexity O(N * log N) where N = total number of workers.
    //!          Sorting the workers based on their wage-to-quality ratio takes
    //!          O(N * log N). Each worker is processed once. For each, perform
    //!          push and pop operations on the max heap that take O(log k)
    //!          where k = number of workers in the paid group/max heap. Thus,
    //!          processing the workers takes O(N * log k) and the total time
    //!          complexity is O(N * log N + N * log k), which simplifies to
    //!          O(N * log N) for k <= N.
    //!          Space complexity O(N). We use O(N) space to store the
    //!          wage-to-quality ratio for each worker and the max heap tracking
    //!          the highest quality workers uses O(k) space. The total space
    //!          complexity is O(N + k), which simplifies to O(N) for k <= N.
    //!          When sorting the wage_to_quality_ratio vector, std::sort has a
    //!          worst-case space complexity of O(log N) as a hybrid of Quick
    //!          Sort, Heap Sort, and Insertion Sort.

    const auto num_workers = static_cast<int>(std::ssize(quality));

    double min_total_cost {std::numeric_limits<double>::max()};
    double curr_total_quality {};

    //! Store pair of <wage-to-quality ratio, quality of worker>
    std::vector<std::pair<double, int>> wage_to_quality_ratio;
    wage_to_quality_ratio.reserve(quality.size());

    //! Calculate wage-to-quality ratio for each worker
    for (int worker_idx = 0; worker_idx < num_workers; ++worker_idx)
    {
        wage_to_quality_ratio.emplace_back(
            static_cast<double>(wage[worker_idx]) / quality[worker_idx],
            quality[worker_idx]);
    }

    //! Sort workers based on their wage-to-quality ratio
    std::sort(wage_to_quality_ratio.begin(), wage_to_quality_ratio.end());

    //! Use a max heap to keep track of the highest quality workers
    std::priority_queue<int> worker_qualities;

    for (int worker_idx = 0; worker_idx < num_workers; ++worker_idx)
    {
        const int curr_quality {wage_to_quality_ratio[worker_idx].second};
        worker_qualities.push(curr_quality);
        curr_total_quality += curr_quality;

        //! If there are more than k workers, remove the highest quality worker
        if (std::ssize(worker_qualities) > k)
        {
            curr_total_quality -= worker_qualities.top();
            worker_qualities.pop();
        }

        //! If there are k workers in the max heap then calculate the total cost
        if (std::ssize(worker_qualities) == k)
        {
            min_total_cost =
                std::min(min_total_cost,
                         curr_total_quality
                            * wage_to_quality_ratio[worker_idx].first);
        }
    }

    return min_total_cost;
}

TEST(MincostToHireWorkersTest, SampleTest1)
{
    const std::vector<int> quality {10, 20, 5};
    const std::vector<int> wage {70, 50, 30};
    constexpr int          k {2};

    EXPECT_EQ(105.0, mincostToHireWorkersDS(quality, wage, k));
}

TEST(MincostToHireWorkersTest, SampleTest2)
{
    const std::vector<int> quality {3, 1, 10, 10, 1};
    const std::vector<int> wage {4, 8, 2, 2, 7};
    constexpr int          k {3};

    EXPECT_NEAR(30.66667, mincostToHireWorkersDS(quality, wage, k), 1.0E-5);
}
