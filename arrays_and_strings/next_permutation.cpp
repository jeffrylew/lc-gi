#include <gtest/gtest.h>

#include <vector>

//! @brief First attempt solution to find next permutation of nums
//! @param[in, out] nums Reference to vector of ints to find next permutation of
static void nextPermutationFA(std::vector<int>& nums)
{

} // static void nextPermutationFA( ...

TEST(NextPermutationTest, SampleTest1)
{
    const std::vector<int> expected_output {1, 3, 2};
    std::vector<int>       nums {1, 2, 3};

    nextPermutationFA(nums);
    EXPECT_EQ(expected_output, nums);
}

TEST(NextPermutationTest, SampleTest2)
{
    const std::vector<int> expected_output {1, 2, 3};
    std::vector<int>       nums {3, 2, 1};

    nextPermutationFA(nums);
    EXPECT_EQ(expected_output, nums);
}

TEST(NextPermutationTest, SampleTest3)
{
    const std::vector<int> expected_output {1, 5, 1};
    std::vector<int>       nums {1, 1, 5};

    nextPermutationFA(nums);
    EXPECT_EQ(expected_output, nums);
}
