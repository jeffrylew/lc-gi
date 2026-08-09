#include <gtest/gtest.h>

#include <algorithm>
#include <flat_map>
#include <stack>
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

        auto ceil_it  = val_idxs.lower_bound(curr_val);

        //! val_idxs orders keys from smallest to largest.
        //! floor_it points to the next greater key in val_idxs.
        //! If val_idxs has no greater keys then floor_it is val_idxs.end().
        //! The floor_it != val_idxs.begin() if statement block sets floor_it to
        //! the largest key in val_idxs, which is the largest value that is less
        //! than the current key (for even-numbered jumps, arr[i] >= arr[j]).
        //!
        //! If val_idxs does have a greater key then the if statement block sets
        //! floor_it to the key that is less than or equal to the current key.
        auto floor_it = val_idxs.upper_bound(curr_val);

        if (ceil_it != val_idxs.end())
        {
            //! Case when we can jump to a greater or equal value in arr
            can_jump_to_greater_eq_elem[idx] =
                can_jump_to_lesser_eq_elem[ceil_it->second];
        }

        if (floor_it != val_idxs.begin())
        {
            //! Case when we can jump to a lesser or equal value in arr
            --floor_it;
            can_jump_to_lesser_eq_elem[idx] =
                can_jump_to_greater_eq_elem[floor_it->second];
        }

        if (can_jump_to_greater_eq_elem[idx])
        {
            ++num_good_start_idxs;
        }

        val_idxs[curr_val] = idx;
    }

    return num_good_start_idxs;
}

//! @brief Monotonic stack discussion solution
//! @param[in] arr Reference to a vector of ints
//! @return The number of good starting indices
static int oddEvenJumpsDS2(const std::vector<int>& arr)
{
    //! @details https://leetcode.com/problems/odd-even-jump/editorial/

    const auto arr_size = static_cast<int>(std::ssize(arr));

    const auto init_next_idxs = [&](std::vector<int>& indices_of_sorted_vals,
                                    std::vector<int>& next_indices) {
        //! Invariant: stack is monotonically decreasing
        std::stack<int> idx_stack;

        for (const int curr_index : indices_of_sorted_vals)
        {
            while (!idx_stack.empty() && curr_index > idx_stack.top())
            {
                indices_of_sorted_vals[idx_stack.top()] = curr_index;
                idx_stack.pop();
            }

            idx_stack.push(curr_index);
        }
    };

    std::vector<int> odd_jump_next_idxs(arr.size(), -1);
    std::vector<int> even_jump_next_idxs(arr.size(), -1);

    //! @todo
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
    EXPECT_EQ(2, oddEvenJumpsDS2(arr));
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
    EXPECT_EQ(3, oddEvenJumpsDS2(arr));
}

TEST(OddEvenJumpsTest, Example3)
{
    //!                  index  0  1  2  3  4
    const std::vector<int> arr {5, 1, 3, 4, 2};

    /*
     Before for loop in DS1:
              index    0      1      2      3      4
       jump_to_ge = [false, false, false, false, true]
       jump_to_le = [false, false, false, false, true]
       val_idxs = {{val: 2, idx: 4}}

     idx = 3:
       curr_val = 4
       ceil_it  -> val_idxs.end()
       floor_it -> val_idxs.end()
         Can't jump to greater or equal value since ceil_it == val_idxs.end():
           jump_to_ge[3] = false
         Can jump to lesser or equal value:
           floor_it -> {val: 2, idx: 4}
           jump_to_le[3] = jump_to_ge[4] = true // Jump from val: 4 to val: 2
       At end of iteration:
                index    0      1      2      3      4
         jump_to_ge = [false, false, false, false, true]
         jump_to_le = [false, false, false, true,  true]
         val_idxs = {{val: 2, idx: 4}, {val: 4, idx: 3}}
         num_good_starting_idxs = 1

     idx = 2:
       curr_val = 3
       ceil_it  -> {val: 4, idx: 3}
       floor_it -> {val: 4, idx: 3}
         Can jump to greater or equal value:
           jump_to_ge[2] = jump_to_le[3] = true // Jump from val: 3 to val: 4
         Can jump to lesser or equal value:
           floor_it -> {val: 2, idx: 4}
           jump_to_le[2] = jump_to_ge[4] = true // Jump from val: 3 to val: 2
       jump_to_ge[2] == true -> num_good_start_idxs = 2
       At end of iteration:
                index    0      1      2     3      4
         jump_to_ge = [false, false, true, false, true]
         jump_to_le = [false, false, true, true,  true]
         val_idxs = {{val: 2, idx: 4}, {val: 3, idx: 2}, {val: 4, idx: 3}}
         num_good_starting_idxs = 2

     idx = 1:
       curr_val = 1
       ceil_it  -> {val: 2, idx: 4}
       floor_it -> {val: 2, idx: 4}
         Can jump to greater or equal value:
           jump_to_ge[1] = jump_to_le[4] = true // Jump from val: 1 to val: 2
         Can't jump to lesser or equal value since floor_it == val_idxs.begin():
           jump_to_le[1] = false
       jump_to_ge[1] == true -> num_good_starting_idxs = 3
       At end of iteration:
                index    0      1      2     3      4
         jump_to_ge = [false, true,  true, false, true]
         jump_to_le = [false, false, true,  true, true]
         val_idxs = {{val: 2, idx: 4}, {val: 3, idx: 2}, {val: 4, idx: 3}}
         num_good_starting_idxs = 3

     idx = 0:
       curr_val = 5
       ceil_it  -> val_idxs.end()
       floor_it -> val_idxs.end()
         Can't jump to greater or equal value since ceil_it == val_idxs.end():
           jump_to_ge[0] = false
         Can jump to lesser or equal value:
           floor_it -> {val: 4, idx: 3}
           jump_to_le[0] = jump_to_ge[3] = false
       At end of iteration:
                index    0      1      2     3      4
         jump_to_ge = [false, true,  true, false, true]
         jump_to_le = [false, false, true,  true, true]
         val_idxs = {
          {val: 1, idx: 0}, {val: 2, idx: 4}, {val: 3, idx: 2}, {val: 4, idx: 3}
         }
         num_good_starting_idxs = 3 // Indices 1, 2, and 4
     */

    //! Can reach the end from starting indices 1, 2, and 4
    EXPECT_EQ(3, oddEvenJumpsFA(arr));
    EXPECT_EQ(3, oddEvenJumpsDS1(arr));
    EXPECT_EQ(3, oddEvenJumpsDS2(arr));
}

TEST(OddEvenJumpsTest, Example4)
{
    const std::vector<int> arr {1, 2, 3, 2, 1, 4, 4, 5};

    EXPECT_EQ(2, oddEvenJumpsFA(arr));
    EXPECT_NE(6, oddEvenJumpsFA(arr));
    EXPECT_EQ(6, oddEvenJumpsDS1(arr));
    EXPECT_EQ(6, oddEvenJumpsDS2(arr));
}
