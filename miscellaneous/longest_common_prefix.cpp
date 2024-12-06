#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <vector>

//! @brief First attempt to find longest common prefix in vector of strings
//! @param[in] strs Vector of strings to search for longest common prefix in
//! @return Longest common prefix. If none, return empty string.
static std::string longestCommonPrefixFA(std::vector<std::string> strs)
{
    //! @details https://leetcode.com/problems/longest-common-prefix/description
    //!
    //!          Time complexity O(M * N) where M = minimum string size in strs
    //!          and N = strs.size().
    //!          Space complexity O(1), no extra space needed other than prefix.

    std::string prefix {};

    const auto shortest_str_it =
        std::min_element(strs.begin(),
                         strs.end(),
                         [](const auto& lhs, const auto& rhs) {
                             return std::ssize(lhs) < std::ssize(rhs);
                         });

    for (int idx = 0; idx < std::ssize(*shortest_str_it); ++idx)
    {
        if (std::any_of(strs.begin(),
                        strs.end(),
                        [&](const auto& str) {
                            return str[idx] != strs.front()[idx];
                        }))
        {
            return prefix;
        }

        prefix += strs.front()[idx];
    }

    return prefix;

} // static std::string longestCommonPrefixFA( ...

TEST(LongestCommonPrefixTest, SampleTest1)
{
    EXPECT_EQ("fl", longestCommonPrefixFA({"flower", "flow", "flight"}));
}

TEST(LongestCommonPrefixTest, SampleTest2)
{
    EXPECT_TRUE(longestCommonPrefixFA({"dog", "racecar", "car"}).empty());
}
