#include <gtest/gtest.h>

#include <string>
#include <vector>

//! @brief First attempt to get all strobogrammatic numbers of length n
//! @param[in] n Length of strobogrammatic number
//! @return Vector of strobogrammatic numbers that are of length n
static std::vector<std::string> findStrobogrammaticFA(int n)
{
    //! @details leetcode.com/explore/interview/card/google/62/recursion-4/399

    //! @todo

} // static std::vector<std::string> findStrobogrammaticFA( ...

TEST(FindStrobogrammaticTest, SampleTest1)
{
    const std::vector<std::string> expected_output {"11", "69", "88", "96"};

    EXPECT_EQ(expected_output, findStrobogrammaticFA(2));
}

TEST(FindStrobogrammaticTest, SampleTest2)
{
    const std::vector<std::string> expected_output {"0", "1", "8"};

    EXPECT_EQ(expected_output, findStrobogrammaticFA(1));
}
