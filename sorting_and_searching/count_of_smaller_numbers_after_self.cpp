#include <gtest/gtest.h>

#include <vector>

//! @brief First attempt to get vector of num smaller elements to right of idx
//! @param[in] nums Vector of integers
//! @return Vector where counts[i] is num smaller elements to right of nums[i]
static std::vector<int> countSmallerFA(std::vector<int> nums)
{

} // static std::vector<int> countSmallerFA( ...

TEST(CountSmallerTest, SampleTest1)
{
    const std::vector<int> expected_output {2, 1, 1, 0};

    EXPECT_EQ(expected_output, countSmallerFA({5, 2, 6, 1}));
}

TEST(CountSmallerTest, SampleTest2)
{
    const std::vector<int> expected_output {0};

    EXPECT_EQ(expected_output, countSmallerFA({-1}));
}

TEST(CountSmallerTest, SampleTest3)
{
    const std::vector<int> expected_output {0, 0};

    EXPECT_EQ(expected_output, countSmallerFA({-1, -1}));
}
