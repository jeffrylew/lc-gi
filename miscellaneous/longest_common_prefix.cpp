#include <gtest/gtest.h>

#include <string>
#include <vector>

//! @brief First attempt to find longest common prefix in vector of strings
//! @param[in] strs Vector of strings to search for longest common prefix in
//! @return Longest common prefix. If none, return empty string.
static std::string longestCommonPrefixFA(std::vector<std::string> strs)
{

} // static std::string longestCommonPrefixFA( ...

TEST(LongestCommonPrefixTest, SampleTest1)
{
    EXPECT_EQ("fl", longestCommonPrefixFA({"flower", "flow", "flight"}));
}

TEST(LongestCommonPrefixTest, SampleTest2)
{
    EXPECT_TRUE(longestCommonPrefixFA({"dog", "racecar", "car"}).empty());
}
