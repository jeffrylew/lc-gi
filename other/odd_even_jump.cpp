#include <gtest/gtest.h>

#include <algorithm>
#include <flat_map>
#include <utility>
#include <vector>

//! @brief First attempt to get the number of good starting indices
//! @param[in] arr Reference to a vector of ints
//! @return The number of good starting indices
static int oddEvenJumpsFA(const std::vector<int>& arr)
{
    //! @details leetcode.com/explore/interview/card/google/67/sql-2/3045
    //!
    //!          First attempt solution does not pass Example 4.

    //! Vector of <val in arr, index in arr>
    std::vector<std::pair<int, int>> val_idxs;
    val_idxs.reserve(arr.size());

    const auto arr_size = static_cast<int>(std::ssize(arr));
    for (int idx = 0; idx < arr_size; ++idx)
    {
        val_idxs.emplace_back(arr[idx], idx);
    }
    std::ranges::stable_sort(val_idxs,
                             [](const auto& lhs, const auto& rhs) -> bool {
                                 return lhs.first < rhs.first;   
                             });

    int       num_good_start_idxs {};
    const int end_val {arr.back()};

    for (int arr_idx = 0; arr_idx < arr_size; ++arr_idx)
    {
        const int arr_val {arr[arr_idx]};

        auto start_it = std::ranges::lower_bound(
            val_idxs,
            arr_val,
            {},
            [](const std::pair<int, int>& ele) -> int {
                return ele.first;
            });
        const auto start_idx =
            static_cast<int>(std::ranges::distance(val_idxs.begin(), start_it));

        int prev_arr_idx {arr_idx};
        int curr_idx {start_idx};
        int is_odd_jump {true};

        while (val_idxs[curr_idx].first != end_val)
        {
            if (is_odd_jump)
            {
                while (curr_idx < arr_size)
                {
                    if (val_idxs[curr_idx].second <= prev_arr_idx)
                    {
                        ++curr_idx;
                    }
                    else
                    {
                        prev_arr_idx = val_idxs[curr_idx].second;
                        break;
                    }
                }

                if (curr_idx == arr_size)
                {
                    break;
                }
            }
            else
            {
                while (curr_idx >= 0)
                {
                    if (val_idxs[curr_idx].second <= prev_arr_idx)
                    {
                        --curr_idx;
                    }
                    else
                    {
                        const int prev_idx {curr_idx - 1};
                        if (prev_idx >= 0
                            && val_idxs[prev_idx].first
                                == val_idxs[curr_idx].first)
                        {
                            --curr_idx;
                            continue;
                        }

                        prev_arr_idx = val_idxs[curr_idx].second;
                        break;
                    }
                }

                if (curr_idx < 0)
                {
                    break;
                }
            }

            is_odd_jump = !is_odd_jump;
        }

        if (curr_idx >= 0
            && curr_idx < arr_size
            && val_idxs[curr_idx].first == end_val)
        {
            ++num_good_start_idxs;
        }
    }

    return num_good_start_idxs;
}

//! @brief DP using map discussion solution
//! @param[in] arr Reference to a vector of ints
//! @return The number of good starting indices
static int oddEvenJumpsDS1(const std::vector<int>& arr)
{
    //! @details https://leetcode.com/explore/interview/card/google/67/sql-2
    //!          /3045/discuss/217981/JavaC++Python-DP-using-Map-or-Stack/
    //!
    //!          Time complexity O(N * log N) where N = arr.size() for binary
    //!          searching with lower_bound and upper_bound N times.
    //!          Space complexity O(N) for two vectors and flat map.

    const auto arr_size = static_cast<int>(std::ssize(arr));

    int num_good_start_idxs {1};

    std::vector<bool> can_jump_to_greater_eq_elem(arr.size());
    std::vector<bool> can_jump_to_lesser_eq_elem(arr.size());

    can_jump_to_greater_eq_elem.back() = true;
    can_jump_to_lesser_eq_elem.back()  = true;

    //! Map of <arr val, index in arr>
    std::flat_map<int, int> val_idxs;
    val_idxs[arr.back()] = arr_size - 1;

    for (int idx = arr_size - 2; idx >= 0; --idx)
    {
        const int curr_val {arr[idx]};

        auto curr_it = val_idxs.lower_bound(curr_val);
        auto next_it = val_idxs.upper_bound(curr_val);

        if (curr_it != val_idxs.end())
        {
            can_jump_to_greater_eq_elem[idx] =
                can_jump_to_lesser_eq_elem[curr_it->second];
        }

        if (next_it != val_idxs.begin())
        {
            --next_it;
            can_jump_to_lesser_eq_elem[idx] =
                can_jump_to_greater_eq_elem[next_it->second];
        }

        if (can_jump_to_greater_eq_elem[idx])
        {
            ++num_good_start_idxs;
        }

        val_idxs[curr_val] = idx;
    }

    return num_good_start_idxs;
}

TEST(OddEvenJumpsTest, Example1)
{
    //!                  index   0   1   2   3   4
    const std::vector<int> arr {10, 13, 12, 14, 15};

    /*
     val_idxs: {10, 0}, {12, 2}, {13, 1}, {14, 3}, {15, 4}
     */

    /*
     Starting index 0:
       - Make 1st jump to index 2 since arr[2] is the smallest among arr[1],
         arr[2], arr[3], and arr[4] that is greater or equal to arr[0]
       - Cannot jump any more afterwards
     Starting indices 1 and 2: Make 1st jump to index 3 then cannot jump after
     Starting index 3: Make 1st jump to index 4 and reach the end
     Starting index 4: Reached the end already
     Thus, there are 2 good indices (3, 4) where we can reach the end
     */
    EXPECT_EQ(2, oddEvenJumpsFA(arr));
    EXPECT_EQ(2, oddEvenJumpsDS1(arr));
}

TEST(OddEvenJumpsTest, Example2)
{
    //!                  index  0  1  2  3  4
    const std::vector<int> arr {2, 3, 1, 1, 4};

    /*
     val_idxs: {1, 2}, {1, 3}, {2, 0}, {3, 1}, {4, 4}
     */

    /*
     Starting index 0:
       - Make jumps to indices 1, 2, 3
       - During 1st jump (odd), jump to arr[1] since it is the smallest value in
         arr[1], arr[2], arr[3], arr[4] that is greater than or equal to arr[0]
       - During 2nd jump (even), jump from arr[1] to arr[2] since arr[2] is the
         largest value in arr[2], arr[3], arr[4] that is <= arr[1]. arr[3] is
         also the largest value but 2 is a smaller index so we only jump to 2
       - During 3rd jump (odd), jump from arr[2] to arr[3] since arr[3] is the
         smallest value in arr[3], arr[4] that is >= arr[2].
       - Can't jump from arr[3] to arr[4] so starting index 0 is not good
     Starting index 1: Jump to arr[4] so can reach the end
     Starting index 2: Jump to arr[3] and then can't jump any more
     Starting index 3: Jump to arr[4] so can reach the end
     Starting index 4: Already at the end
     Thus, there are 3 good starting indices (1, 3, 4) where can reach the end
     */
    EXPECT_EQ(3, oddEvenJumpsFA(arr));
    EXPECT_EQ(3, oddEvenJumpsDS1(arr));
}

TEST(OddEvenJumpsTest, Example3)
{
    //!                  index  0  1  2  3  4
    const std::vector<int> arr {5, 1, 3, 4, 2};

    //! Can reach the end from starting indices 1, 2, and 4
    EXPECT_EQ(3, oddEvenJumpsFA(arr));
    EXPECT_EQ(3, oddEvenJumpsDS1(arr));
}

TEST(OddEvenJumpsTest, Example4)
{
    const std::vector<int> arr {1, 2, 3, 2, 1, 4, 4, 5};

    EXPECT_EQ(2, oddEvenJumpsFA(arr));
    EXPECT_NE(6, oddEvenJumpsFA(arr));
    EXPECT_EQ(6, oddEvenJumpsDS1(arr));
}
