#include <gtest/gtest.h>

#include <string>
#include <string_view>
#include <unordered_map>

struct Palindrome
{
    //! Start position of palindrome
    int pos {};

    //! Length of palindrome
    int len {};
};

//! @brief Check if a palindrome ending at end_idx exists
//! @param[in] end_idx Index that palindrome should end at
//! @param[in] s_in    std::string_view of input string s
//! @return True if a palindrome that ends at end_idx exists, else false
[[nodiscard]] static bool palindrome_exists(int end_idx, std::string_view s_in)
{
    if (end_idx == 0)
    {
        return true;
    }

    for (int start_idx = end_idx - 1; start_idx >= 0; --start_idx)
    {
        if (s_in[start_idx] != s_in[end_idx])
        {
            continue;
        }

        int left_idx {start_idx};
        int right_idx {end_idx};

        while (left_idx < right_idx)
        {
            if (s_in[left_idx] != s_in[right_idx])
            {
                break;
            }

            ++left_idx;
            --right_idx;
        }

        if (left_idx >= right_idx)
        {
            return true;
        }
    }

    return false;

} // [[nodiscard]] static bool palindrome_exists( ...

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
    const auto s_len = static_cast<int>(std::ssize(s_in));

    if (end_idx == 0)
    {
        //! Palindrome ending at index 0 is first char in s_in
        return {0, 1};
    }

    if (end_idx_palindrome.contains(end_idx))
    {
        return end_idx_palindrome[end_idx];
    }

    const auto& prev_longest_palindrome = end_idx_palindrome[end_idx - 1];

    /*
     * Not sure how to proceed
     *
    if (palindrome_exists(end_idx, s_in))
    {
        end_idx_palindrome[end_idx] = Palindrome {};
    }
     */

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
    //!          states to figure out the longest palindrome ending at i?

    const auto s_len = static_cast<int>(std::ssize(s));

    //! Map of end index and data of longest palindrome up to end index
    std::unordered_map<int, Palidrome> end_idx_palindrome {{0, {0, 1}}};

    const auto& longest_palindrome =
        longest_palindrome_up_to_idx(s_len - 1, s, end_idx_palindrome);

    return s.substring(longest_palindrome.pos, longest_palindrome.len);

} // static std::string longestPalindromeFA( ...

TEST(LongestPalidromeTest, SampleTest1)
{
    // EXPECT_EQ("bab", longestPalindromeFA("babad"));
    // EXPECT_EQ("aba", longestPalindromeFA("babad"));
}

TEST(LongestPalidromeTest, SampleTest2)
{
    // EXPECT_EQ("bb", longestPalindromeFA("cbbd"));
}
