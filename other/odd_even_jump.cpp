#include <gtest/gtest.h>

#include <vector>

//! @brief First attempt to get the number of good starting indices
//! @param[in] arr Reference to a vector of ints
//! @return The number of good starting indices
static int oddEvenJumpsFA(const std::vector<int>& arr)
{
    //! @details leetcode.com/explore/interview/card/google/67/sql-2/3045
}

TEST(OddEvenJumpsTest, Example1)
{
    //!                  index   0   1   2   3   4
    const std::vector<int> arr {10, 13, 12, 14, 15};

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
}

TEST(OddEvenJumpsTest, Example2)
{
    //!                  index  0  1  2  3  4
    const std::vector<int> arr {2, 3, 1, 1, 4};

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
}

TEST(OddEvenJumpsTest, Example3)
{
    //!                  index  0  1  2  3  4
    const std::vector<int> arr {5, 1, 3, 4, 2};

    //! Can reach the end from starting indices 1, 2, and 4
    EXPECT_EQ(3, oddEvenJumpsFA(arr));
}
