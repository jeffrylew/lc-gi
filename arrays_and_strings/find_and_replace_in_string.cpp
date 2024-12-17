#include <gtest/gtest.h>

#include <string>
#include <string_view>
#include <utility>
#include <vector>

//! @struct Find_replace
//! @brief Stores index, source, and target string for each find and replace op
struct Find_replace
{
    int         idx {};
    std::string source {};
    std::string target {};
};

//! @brief 1st attempt to replace sources[i] with targets[i] in s at indices[i]
//! @param[in] s       Original std::string to perform replacements in
//! @param[in] indices Vector of indices in the original s to replace at
//! @param[in] sources Vector of substrings that must exist in s at indices[i]
//! @param[in] targets Vector of substrings to replace with in s at indices[i]
static std::string findReplaceStringFA(std::string              s,
                                       std::vector<int>         indices,
                                       std::vector<std::string> sources,
                                       std::vector<std::string> targets)
{
    //! @details https://leetcode.com/explore/interview/card/google/59
    //!          /array-and-strings/3057/
    //!
    //!          Time complexity O(N * log N) where N = indices.size().
    //!          Space complexity O(N) for candidate vector

    std::string replacement {};
    const auto  indices_size = static_cast<int>(std::ssize(indices));

    std::vector<Find_replace> candidates {};
    candidates.reserve(indices_size);

    for (int idx = 0; idx < indices_size; ++idx)
    {
        candidates.emplace_back(indices[idx],
                                std::move(sources[idx]),
                                std::move(targets[idx]));
    }
    std::sort(candidates.begin(),
              candidates.end(),
              [](const auto& lhs_candidate, const auto& rhs_candidate) {
                  return lhs_candidate.idx < rhs_candidate.idx;
              });

    int                    s_idx {};
    const std::string_view s_view {s};

    for (const auto& candidate : candidates)
    {
        const int              curr_idx {candidate.idx};
        const std::string_view curr_source {candidate.source};
        const std::string_view curr_target {candidate.target};

        const auto curr_source_size = static_cast<int>(std::ssize(curr_source));
        const auto curr_target_size = static_cast<int>(std::ssize(curr_target));

        while (s_idx < curr_idx)
        {
            replacement += s_view[s_idx++];
        }

        if (s_view.substr(curr_idx, curr_source_size) == curr_source)
        {
            replacement.append(curr_target);
            s_idx = curr_idx + curr_source_size;
        }
    }

    replacement.append(s_view.substr(s_idx));

    return replacement;

} // static std::string findReplaceStringFA( ...

TEST(FindReplaceStringTest, SampleTest1)
{
    EXPECT_EQ("eeebffff", findReplaceStringFA("abcd",
                                              {0, 2},
                                              {"a", "cd"},
                                              {"eee", "ffff"}));
}

TEST(FindReplaceStringTest, SampleTest2)
{
    EXPECT_EQ("eeecd", findReplaceStringFA("abcd",
                                           {0, 2},
                                           {"ab", "ec"},
                                           {"eee", "ffff"}));
}

TEST(FindReplaceStringTest, SampleTest3)
{
    const std::string      s {"vmokgggqzp"};
    const std::vector<int> indices {3, 5, 1};
    const std::vector<std::string> sources {"kg", "ggq", "mo"};
    const std::vector<std::string> targets {"s", "so", "bfr"};

    EXPECT_EQ("vbfrssozp", findReplaceStringFA(s, indices, sources, targets));
}
