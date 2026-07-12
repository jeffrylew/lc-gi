#include <gtest/gtest.h>

#include <string>
#include <unordered_map>

//! @brief First attempt to check if num is a strobogrammatic number
//! @param[in] num String that represents an integer of length up to 50
//! @return True if num looks the same when rotated 180 degrees, else false
static bool isStrobogrammaticFA(const std::string& num)
{
    //! @details leetcode.com/explore/featured/card/google/66/others-4/3099
    //!
    //!          Time complexity O(N) where N = nums.size().
    //!          Space complexity O(1) for a constant size mirrored_nums map.

    const std::unordered_map<char, char> mirrored_nums {
        {'0', '0'}, {'1', '1'}, {'6', '9'}, {'8', '8'}, {'9', '6'}};

    const auto num_size = static_cast<int>(std::ssize(num));

    //! num has an odd number of chars. The middle char is not in mirrored_nums.
    if (num_size % 2 == 1)
    {
        const char middle_num {num[num_size / 2]};
        if (!mirrored_nums.contains(middle_num))
        {
            return false;
        }

        if (middle_num == '6' || middle_num == '9')
        {
            return false;
        }
    }

    int right_idx {num_size - 1};

    for (int left_idx = 0; left_idx < num_size / 2; ++left_idx)
    {
        const char left_num {num[left_idx]};
        const char right_num {num[right_idx]};
        --right_idx;

        auto left_num_it  = mirrored_nums.find(left_num);
        auto right_num_it = mirrored_nums.find(right_num);

        if (left_num_it == mirrored_nums.end()
            || right_num_it == mirrored_nums.end())
        {
            return false;
        }

        if (right_num != left_num_it->second)
        {
            return false;
        }
    }

    return true;
}

TEST(IsStrobogrammatic, SampleTest1)
{
    EXPECT_TRUE(isStrobogrammaticFA("69"));
}

TEST(IsStrobogrammatic, SampleTest2)
{
    EXPECT_TRUE(isStrobogrammaticFA("88"));
}

TEST(IsStrobogrammatic, SampleTest3)
{
    EXPECT_TRUE(isStrobogrammaticFA("962"));
}

TEST(IsStrobogrammatic, SampleTest4)
{
    EXPECT_FALSE(isStrobogrammaticFA("6"));
}

TEST(IsStrobogrammatic, SampleTest5)
{
    EXPECT_FALSE(isStrobogrammaticFA("868"));
}
