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

//! @brief Horizontal scanning discussion solution
//! @param[in] strs Vector of strings to search for longest common prefix in
//! @return Longest common prefix. If none, return empty string.
static std::string longestCommonPrefixDS1(std::vector<std::string> strs)
{
    //! @details https://leetcode.com/problems/longest-common-prefix/editorial/
    //!
    //!          Time complexity O(S) where S = sum of all chars in all strings.
    //!          In the worst case, all N strings are the same. The algorithm
    //!          compares S_1 with other strings [S_2, S_N]. There are S char
    //!          comparisons.
    //!          Space complexity O(1). Only use constant extra space.

    if (strs.empty())
    {
        return {};
    }

    std::string prefix {strs.front()};

    for (int idx = 1; idx < std::ssize(strs); ++idx)
    {
        while (!strs[idx].starts_with(prefix))
        {
            prefix.pop_back();
            if (prefix.empty())
            {
                return {};
            }
        }
    }

    return prefix;

} // static std::string longestCommonPrefixDS1( ...

TEST(LongestCommonPrefixTest, SampleTest1)
{
    EXPECT_EQ("fl", longestCommonPrefixFA({"flower", "flow", "flight"}));
    EXPECT_EQ("fl", longestCommonPrefixDS1({"flower", "flow", "flight"}));
}

TEST(LongestCommonPrefixTest, SampleTest2)
{
    EXPECT_TRUE(longestCommonPrefixFA({"dog", "racecar", "car"}).empty());
    EXPECT_TRUE(longestCommonPrefixDS1({"dog", "racecar", "car"}).empty());
}
