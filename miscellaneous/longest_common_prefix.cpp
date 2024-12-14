#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <functional>
#include <limits>
#include <memory>
#include <string>
#include <string_view>
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
    //!          Time complexity O(S) where S = number of chars in all strings.
    //!          In the worst case, all N strings are the same. The algorithm
    //!          compares S_1 with other strings [S_2, S_N]. There are S char
    //!          comparisons.
    //!          Space complexity O(1). Only use constant extra space, excluding
    //!          the space for the output prefix.

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

//! @brief Vertical scanning discussion solution
//! @param[in] strs Vector of strings to search for longest common prefix in
//! @return Longest common prefix. If none, return empty string.
static std::string longestCommonPrefixDS2(std::vector<std::string> strs)
{
    //! @details https://leetcode.com/problems/longest-common-prefix/editorial/
    //!
    //!          Time complexity O(S) where S = number of chars in all strings.
    //!          In the worst case, there will be N equal strings of length M
    //!          and the algorithm performs S = M * N character comparisons. The
    //!          worst case is the same as in DS1 but in the best case there are
    //!          at most N * min_len comparisons, where min_len is the length of
    //!          the shortest string in the vector.
    //!          Space complexity O(1). Only use constant extra space, excluding
    //!          the space for the output prefix.

    if (strs.empty())
    {
        return {};
    }

    for (int strs0_idx = 0; strs0_idx < std::ssize(strs[0]); ++strs0_idx)
    {
        const char strs0_char {strs[0][strs0_idx]};

        for (int strs_idx = 1; strs_idx < std::ssize(strs); ++strs_idx)
        {
            if (strs0_idx == std::ssize(strs[strs_idx])
                || strs[strs_idx][strs0_idx] != strs0_char)
            {
                return strs[0].substr(0, strs0_idx);
            }
        }
    }

    return strs[0];

} // static std::string longestCommonPrefixDS2( ...

//! @brief Divide and conquer discussion solution
//! @param[in] strs Vector of strings to search for longest common prefix in
//! @return Longest common prefix. If none, return empty string.
static std::string longestCommonPrefixDS3(std::vector<std::string> strs)
{
    //! @details https://leetcode.com/problems/longest-common-prefix/editorial/
    //!
    //!          Time complexity O(S) where S = number of chars in all strings.
    //!          In the worst case, there will be N equal strings of length M
    //!          and the algorithm performs S = M * N character comparisons. The
    //!          time complexity is 2 * T(N / 2) + O(M). In the best case the
    //!          algorithm performs N * min_len comparisons, where min_len is
    //!          the length of the shortest string in the vector.
    //!          Space complexity O(M * log N). There is a memory overhead since
    //!          we store recursive calls in the execution stack. There are
    //!          log N recursive calls (for N strings) and each need O(M) space
    //!          to store the result of length M.

    const auto common_prefix =
        [](std::string_view left, std::string_view right) {
            const auto min_size =
                static_cast<int>(std::min(std::ssize(left), std::ssize(right)));

            for (int idx = 0; idx < min_size; ++idx)
            {
                if (left[idx] != right[idx])
                {
                    return std::string {left.substr(0, idx)};
                }
            }

            return std::string {left.substr(0, min_size)};
        };

    const std::function<std::string(int, int)> longest_common_prefix =
        [&](int left_idx, int right_idx) {
            if (left_idx == right_idx)
            {
                return strs[left_idx];
            }

            const int mid_idx {left_idx + (right_idx - left_idx) / 2};

            const std::string lcp_left {
                longest_common_prefix(left_idx, mid_idx)};
            const std::string lcp_right {
                longest_common_prefix(mid_idx + 1, right_idx)};

            return common_prefix(lcp_left, lcp_right);
        };

    if (strs.empty())
    {
        return {};
    }

    return longest_common_prefix(0, static_cast<int>(std::ssize(strs)) - 1);

} // static std::string longestCommonPrefixDS3( ...

//! @brief Binary search discussion solution
//! @param[in] strs Vector of strings to search for longest common prefix in
//! @return Longest common prefix. If none, return empty string.
static std::string longestCommonPrefixDS4(std::vector<std::string> strs)
{
    //! @details https://leetcode.com/problems/longest-common-prefix/editorial/
    //!
    //!          Time complexity O(S * log M) where S = number of all characters
    //!          in all strings. In the worst case, we have N equal strings with
    //!          length M. The algorithm makes log M iterations and where each
    //!          has S = M * N comparisons.
    //!          Space complexity O(1)

    if (strs.empty())
    {
        return {};
    }

    int min_len {std::numeric_limits<int>::max()};

    for (const auto& str : strs)
    {
        min_len = std::min(min_len, static_cast<int>(std::ssize(str)));
    }

    const auto is_common_prefix = [&](int substr_len) {
        const auto prefix = strs.front().substr(0, substr_len);

        for (int str_idx = 1; str_idx < std::ssize(strs); ++str_idx)
        {
            if (!strs[str_idx].starts_with(prefix))
            {
                return false;
            }
        }

        return true;
    };

    int low {1};
    int high {min_len};

    while (low <= high)
    {
        const int middle {low + (high - low) / 2};

        if (is_common_prefix(middle))
        {
            low = middle + 1;
        }
        else
        {
            high = middle - 1;
        }
    }

    return strs.front().substr(0, low + (high - low) / 2);

} // static std::string longestCommonPrefixDS4( ...

constexpr int get_idx(char ch)
{
    return static_cast<int>(ch - 'a');
}

struct TrieNode
{
    bool is_end {};

    //! Count number of children
    int link_count {};

    std::array<std::unique_ptr<TrieNode>, 26> children {};

    void put(char ch, std::unique_ptr<TrieNode>&& node)
    {
        if (children[get_idx(ch)] == nullptr)
        {
            children[get_idx(ch)] = std::move(node);
            ++link_count;
        }
    }

    bool contains(char ch) const
    {
        return children[get_idx(ch)] != nullptr;
    }

    int getLinks() const
    {
        return link_count;
    }
};

class Trie
{
    TrieNode root;

public:
    void insert(std::string_view word)
    {
        auto* node = &root;

        for (char ch : word)
        {
            if (!node->contains(ch))
            {
                node->put(ch, std::make_unique<TrieNode>());
            }
            node = node->children[get_idx(ch)].get();
        }

        node->is_end = true;
    }

    std::string search_longest_prefix(std::string_view query)
    {
        auto*       node = &root;
        std::string prefix;

        for (char ch : query)
        {
            if (node->contains(ch)
                && node->get_links() == 1
                && !node->is_end)
            {
                prefix += ch;
                node = node->children[get_idx(ch)].get();
            }
            else
            {
                break;
            }
        }

        return prefix;
    }
};

//! @brief Trie discussion solution
//! @param[in] strs Vector of strings to search for longest common prefix in
//! @return Longest common prefix. If none, return empty string.
static std::string longestCommonPrefixDS5(std::vector<std::string> strs,
                                          std::string              query)
{
    //! @details https://leetcode.com/problems/longest-common-prefix/editorial/
    //!
    //!          Time complexity O(S) where S = number of all characters in the
    //!          vector. In the worst case, query has length M and is equal to
    //!          all N strings of the vector. Preprocessing takes O(S) and the
    //!          longest common prefix query takes O(M). Building the trie takes
    //!          O(S). To find the common prefix of query in the Trie takes O(M)
    //!          in the worst case.
    //!          Space complexity O(S). Only used additional S space for trie.

    if (strs.empty())
    {
        return {};
    }

    const auto strs_size = static_cast<int>(std::ssize(strs));
    if (strs_size == 1)
    {
        return strs.front()
    }

    //! First word in strs is used as the query so idx starts at 1
    Trie trie;
    for (int idx = 1; idx < strs_size; ++idx)
    {
        trie.insert(strs[idx]);
    }

    return trie.search_longest_prefix(query);

} // static std::string longestCommonPrefixDS5( ...

TEST(LongestCommonPrefixTest, SampleTest1)
{
    EXPECT_EQ("fl", longestCommonPrefixFA({"flower", "flow", "flight"}));
    EXPECT_EQ("fl", longestCommonPrefixDS1({"flower", "flow", "flight"}));
    EXPECT_EQ("fl", longestCommonPrefixDS2({"flower", "flow", "flight"}));
    EXPECT_EQ("fl", longestCommonPrefixDS3({"flower", "flow", "flight"}));
    EXPECT_EQ("fl", longestCommonPrefixDS4({"flower", "flow", "flight"}));
    EXPECT_EQ("fl", longestCommonPrefixDS5({"flower", "flow", "flight"},
                                            "flower"));
}

TEST(LongestCommonPrefixTest, SampleTest2)
{
    EXPECT_TRUE(longestCommonPrefixFA({"dog", "racecar", "car"}).empty());
    EXPECT_TRUE(longestCommonPrefixDS1({"dog", "racecar", "car"}).empty());
    EXPECT_TRUE(longestCommonPrefixDS2({"dog", "racecar", "car"}).empty());
    EXPECT_TRUE(longestCommonPrefixDS3({"dog", "racecar", "car"}).empty());
    EXPECT_TRUE(longestCommonPrefixDS4({"dog", "racecar", "car"}).empty());
    EXPECT_TRUE(longestCommonPrefixDS5({"dog", "racecar", "car"},
                                        "dog").empty());
}
