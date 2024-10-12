#include <gtest/gtest.h>

#include <vector>

//! @brief First attempt to increment large integer by one
//! @param[in] digits Vector of digits from most to least significant from LtoR
//! @return Resulting vector of digits after incrementing by one
static std::vector<int> plusOneFA(std::vector<int> digits)
{
    //! @details https://leetcode.com/problems/plus-one/description/
    //!
    //!          Time complexity O(N) where N = digits.size()
    //!          Space complexity O(N) in the worst case where a "1" is inserted
    //!          at the beginning

    if (digits.back() != 9)
    {
        digits.back() += 1;
        return digits;
    }

    auto rit = digits.rbegin();
    while (rit != digits.rend() && *rit == 9)
    {
        *rit = 0;
        ++rit;
    }

    if (rit != digits.rend())
    {
        *rit += 1;
    }
    else
    {
        digits.insert(digits.begin(), 1);
    }

    return digits;

} // static std::vector<int> plusOneFA( ...

TEST(PlusOneTest, SampleTest1)
{
    const std::vector<int> expected_output {1, 2, 4};

    EXPECT_EQ(expected_output, plusOneFA({1, 2, 3}));
}

TEST(PlusOneTest, SampleTest2)
{
    const std::vector<int> expected_output {4, 3, 2, 2};

    EXPECT_EQ(expected_output, plusOneFA({4, 3, 2, 1}));
}

TEST(PlusOneTest, SampleTest3)
{
    const std::vector<int> expected_output {9};

    EXPECT_EQ(expected_output, plusOneFA({1, 0}));
}
