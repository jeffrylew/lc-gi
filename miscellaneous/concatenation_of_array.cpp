#include <gtest/gtest.h>

#include <vector>

//! @brief First attempt to concatenate two nums vectors
//! @param[in] nums Reference to a vector of ints to concatenate
//! @return A vector where the elements of nums are present twice
static std::vector<int> getConcatenationFA(const std::vector<int>& nums)
{
    //! @details leetcode.com/problems/concatenation-of-array/description
    //!
    //!          Time complexity O(N) where N = nums.size(). std::vector::insert
    //!          is linear wrt the number of elements in nums and initializing
    //!          concatenated_vec also takes O(N).
    //!          Space complexity O(1) since concatenated_vec is the output.

    auto concatenated_vec = nums;
    concatenated_vec.reserve(2U * nums.size());
    concatenated_vec.insert(concatenated_vec.end(), nums.begin(), nums.end());
    return concatenated_vec;
}

TEST(GetConcatenationTest, SampleTest1)
{
    const std::vector<int> nums {1, 2, 1};
    const std::vector<int> expected_output {1, 2, 1, 1, 2, 1};

    EXPECT_EQ(expected_output, getConcatenationFA(nums));
}

TEST(GetConcatenationTest, SampleTest2)
{
    const std::vector<int> nums {1, 3, 2, 1};
    const std::vector<int> expected_output {1, 3, 2, 1, 1, 3, 2, 1};

    EXPECT_EQ(expected_output, getConcatenationFA(nums));
}
