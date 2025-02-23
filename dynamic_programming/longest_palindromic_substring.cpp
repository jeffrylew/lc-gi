#include <gtest/gtest.h>

#include <string>
#include <unordered_map>

struct Palindrome
{
    //! Start position of palindrome
    int pos {};

    //! Length of palindrome
    int len {};
};

//! @brief Dynamic programming helper to get longest palindrome up to end index
//! @param[in]      end_idx            Consider palindromes until this end index
//! @param[in]      s_in               Reference to input string
//! @param[in, out] end_idx_palindrome Reference to map of <end idx, palindrome>
//! @return struct Palindrome with <end idx, length of longest palindrome>
[[nodiscard]] static Palindrome longest_palindrome_up_to_idx(
    int                                 end_idx,
    const std::string&                  s_in,
    std::unordered_map<int, Palidrome>& end_idx_palindrome)
{
    const auto s_len = static_cast<int>(std::ssize(s));

    if (end_idx == 0)
    {
        //! Palindrome ending at index 0 is first char in s_in
        return {0, 1};
    }

    if (end_idx_palindrome.contains(end_idx))
    {
        return end_idx_palindrome[end_idx];
    }

} // [[nodiscard]] static Palindrome longest_palindrome_up_to_idx( ...

//! @brief First attempt to get longest palindromic substring in s
//! @param[in] s std::string to get longest palindromic substring from
//! @return Longest palindromic substring in s
static std::string longestPalindromeFA(std::string s)
{
    //! @details https://leetcode.com/explore/interview/card/google/64
    //!          /dynamic-programming-4/451/
    //!
    //!          If we take an element as part of a palindrome, it changes the
    //!          chars we can take in the future. If we take a char, the length
    //!          increases but the subsequent chars must form a palindrome. If
    //!          we are at the ith element, how can we leverage info about prior
    //!          states to figure out the longest palindrome ending at i? Only
    //!          use the current element i if a palindrome ends at i. So only
    //!          consider indices j in the range [0, i) where s[j] == s[i - j].
    //!          longest_palindrome_up_to_idx(j) returns the palindrome data
    //!          that ends with the jth element. If s[j] == s[i - j], then can
    //           take the palindrome that ends at

    const auto s_len = static_cast<int>(std::ssize(s));

    //! Map of end index and data of longest palindrome up to end index
    std::unordered_map<int, Palidrome> end_idx_palindrome {{0, {0, 1}}};

    int longest_palindrome_pos {};
    int longest_palindrome_len {};

    for (int idx = 0; idx < s_len - 1; ++idx)
    {
        const auto& palindrome_data =
            longest_palindrome_up_to_idx(idx, s, end_idx_palindrome);

        if (palindrome_data.len > longest_palindrome_len)
        {
            longest_palindrome_len = palindrome_data.len;
            longest_palindrome_pos = palindrome_data.pos;
        }
    }

    return s.substring(longest_palindrome_pos, longest_palindrome_len);

} // static std::string longestPalindromeFA( ...

TEST(LongestPalidromeTest, SampleTest1)
{
    EXPECT_EQ("bab", longestPalindromeFA("babad"));
    // EXPECT_EQ("aba", longestPalindromeFA("babad"));
}

TEST(LongestPalidromeTest, SampleTest2)
{
    EXPECT_EQ("bb", longestPalindromeFA("cbbd"));
}
