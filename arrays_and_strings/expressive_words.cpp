#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

//! @brief First attempt to get number of query strings that are stretchy
//! @param[in] s     std::string to check query against
//! @param[in] words Vector of query strings to check stretchiness of
//! @return Number of query strings that are stretchy
static int expressiveWordsFA(std::string s, std::vector<std::string> words)
{
    //! @details https://leetcode.com/explore/interview/card/google/59
    //!          /array-and-strings/3056/
    //!
    //!          First attempt solution passes 31 / 35 test cases but does not
    //!          pass SampleTest3

    const auto s_len = static_cast<int>(std::ssize(s));

    const auto is_stretchy = [&](std::string_view word) {
        int s_left_idx {};
        int s_right_idx {};

        int w_left_idx {};

        for (int w_right_idx = 0; w_right_idx < std::ssize(word); ++w_right_idx)
        {
            if (word[w_right_idx] == s[s_right_idx])
            {
                if (word[w_right_idx] != word[w_left_idx])
                {
                    w_left_idx = w_right_idx;
                }

                if (s[s_right_idx] != s[s_left_idx])
                {
                    s_left_idx = s_right_idx;
                }

                ++s_right_idx;
                continue;
            }

            if (w_right_idx == 0)
            {
                return false;
            }

            while (s[s_right_idx] == word[w_left_idx])
            {
                ++s_right_idx;
            }

            if (s_right_idx - s_left_idx < 3)
            {
                return false;
            }

            s_left_idx = s_right_idx;
            w_left_idx = w_right_idx;
            --w_right_idx;
        }

        while (s_right_idx < s_len && s[s_right_idx] == word[w_left_idx])
        {
            ++s_right_idx;
        }

        return s_right_idx == s_len && s_right_idx - s_left_idx >= 3;
    };

    int num_stretchy_strings {};

    for (const auto& word : words)
    {
        if (is_stretchy(word))
        {
            ++num_stretchy_strings;
        }
    }

    return num_stretchy_strings;

} // static int expressiveWordsFA( ...

//! @brief Discussion solution based on Java Solution using Two Pointers
//! @param[in] s     std::string to check query against
//! @param[in] words Vector of query strings to check stretchiness of
//! @return Number of query strings that are stretchy
static int expressiveWordsDS1(std::string s, std::vector<std::string> words)
{
    //! @details https://leetcode.com/explore/interview/card/google/59
    //!          /array-and-strings/3056/discuss/121706
    //!          /Java-Solution-using-Two-Pointers-with-Detailed-Explanation

    if (s.empty() || words.empty())
    {
        return 0;
    }

    //! Get length of substring containing repeated chars of the letter at left
    const auto get_repeated_length = [](std::string_view str, int left) {
        int right {left};

        while (right < std::ssize(str) && str[right] == str[left])
        {
            ++right;
        }

        return right - left;
    };

    const auto s_len       = static_cast<int>(std::ssize(s));
    const auto is_stretchy = [&](std::string_view word) {
        if (word.empty())
        {
            return false;
        }

        int        s_idx {};
        int        word_idx {};
        const auto word_len = static_cast<int>(std::ssize(word));

        while (s_idx < s_len && word_idx < word_len)
        {
            if (s[s_idx] != word[word_idx])
            {
                return false;
            }

            //! Letters pointed at by s_idx and word_idx must be eual
            const int s_substr_len {get_repeated_length(s, s_idx)};
            const int word_substr_len {get_repeated_length(word, word_idx)};

            //! If s_substr_len < 3 then the letter cannot be extended, so
            //! s_substr_len must equal word_substr_len to have a stretchy word.
            //! If s_substr_len >= 3 then s_substr_len >= word_substr_len must
            //! be true, otherwise there are not enough letters in s to match
            //! those in word
            if (s_substr_len < 3
                && s_substr_len != word_substr_len
                || s_substr_len >= 3
                && s_substr_len < word_substr_len)
            {
                return false;
            }

            s_idx += s_substr_len;
            word_idx += word_substr_len;
        }

        //! If word is stretchy, need to check if scanned entire string
        return s_idx == s_len && word_idx == word_len;
    };

    int num_stretchy_strings {};

    for (const auto& word : words)
    {
        if (is_stretchy(word))
        {
            ++num_stretchy_strings;
        }
    }

    return num_stretchy_strings;

} // static int expressiveWordsDS1( ...

TEST(ExpressiveWordsTest, SampleTest1)
{
    EXPECT_EQ(1, expressiveWordsFA("heeellooo", {"hello", "hi", "helo"}));
    EXPECT_EQ(1, expressiveWordsDS1("heeellooo", {"hello", "hi", "helo"}));
}

TEST(ExpressiveWordsTest, SampleTest2)
{
    EXPECT_EQ(3, expressiveWordsFA("zzzzzyyyyy", {"zzyy", "zy", "zyy"}));
    EXPECT_EQ(3, expressiveWordsDS1("zzzzzyyyyy", {"zzyy", "zy", "zyy"}));
}

TEST(ExpressiveWordsTest, SampleTest3)
{
    constexpr const char* s {
        "vvvppppeeezzzzztttttkkkkkkugggggbbffffffywwwwwwbbbccccddddddkkkkkssspp"
        "ppddpzzzzzhhhhbbbbbmmmy"};

    const std::vector<std::string> words {
        "vvpeezttkkuggbbfywwbbccddkkspdpzhbbmmyy",
        "vvppeeztkkugbfywwbccddkksspdppzhhbmyy",
        "vppezzttkkugbffyywbccddksspddpzhhbmy",
        "vvppezztkugbffyywwbbccddkssppddpzzhhbbmmy",
        "vvppezttkuggbfyywwbbcddkspdppzhhbmy",
        "vppeezzttkkuugbfyywwbbccdkkssppdpzzhbbmy",
        "vpeezztkkugbbffyywwbbccddkksppdpzzhhbbmmy",
        "vppeeztkkuuggbffywbbccddkksppdppzhhbmyy",
        "vpeeztkkuggbfyywbbccdksppdpzhbmy",
        "vpeezztkkugbffywwbbccdkkssppddppzzhhbbmmy",
        "vvpeztkkugbbfyywbcdkssppddpzzhhbbmyy",
        "vpezztkugbbffyywwbcddksppddpzzhbbmy",
        "vvpeezztkkugbbffywwbccdkkspddpzzhbmmyy",
        "vvpeezzttkkuuggbbffyywbbccdkspdppzhhbmy",
        "vvppeezztkkuggbbfywbcdkspdpzhhbmyy",
        "vvppeezzttkkuugbffyywwbbccddkkspddpzzhbmyy",
        "vppezztkuuggbffywwbcdksspdppzhhbmyy",
        "vvppeezzttkuuggbffywbccddkksspddppzzhhbmmy",
        "vvppezzttkuggbffywbbccdkspddppzzhhbmy",
        "vvpezzttkuugbbfywwbccdkssppdpzhbbmmy",
        "vvpeezzttkuugbbffyywbccdksppddppzhhbmyy",
        "vpeezzttkkuggbbffywbccddksppddpzhhbbmy",
        "vvpezttkuuggbffywwbbccddkspdppzhhbmmyy",
        "vppeezzttkkuugbffywbccddksppddpzhhbmmyy",
        "vvpezttkkuugbbfywbccdkspddppzzhbbmmy",
        "vppezzttkkuugbbffywwbcddkssppddpzhhbmmy",
        "vppezzttkugbfywbbcdksppddppzzhhbmyy",
        "vppeeztkuggbbffywbbccdkkspddppzzhbbmmy",
        "vvpeeztkuuggbbfywbcdkksspddppzhhbbmmyy",
        "vpezttkkuuggbbffyywwbbcdksspddppzhhbmy",
        "vpeezzttkkuuggbffywwbccdkksspddppzzhbmyy",
        "vpezttkkuugbffyywbccdksspddppzhbbmmyy",
        "vvppezztkugbbffyywbbccdkksppdppzhbmyy",
        "vvpeezttkuggbbfyywwbbcddkksppdpzzhbbmyy",
        "vvpeztkuuggbffyywbbccdkksspddppzzhbbmy",
        "vppeezzttkugbbffyywbccddksppdppzzhbmmyy",
        "vppeezttkkuugbbfywwbccddkksspdpzhhbmmy",
        "vpeezzttkugbbffywbbccdkksspddppzhbbmyy",
        "vpeezttkkuugbbfywbbccddksppddppzzhhbmmy",
        "vpeezztkuuggbbffywwbbccddksspddpzzhhbbmmyy",
        "vppeezttkkuggbbffyywwbccdksspdpzzhbmy",
        "vpezzttkkuugbbfyywbbcdksspdppzzhbbmyy",
        "vvppezttkkuggbbfyywbbccdkksspddpzhbbmyy",
        "vvpezzttkuggbbffyywbbcdkksppdpzzhbmmyy",
        "vvpeztkugbfywwbccddkkspddpzhhbbmyy",
        "vvppezttkuugbbfyywwbcddkksspdppzhhbbmy",
        "vvpeeztkkuuggbbfywwbcdkspddpzzhhbmmy",
        "vvpeezttkugbffywbbccdkkssppddppzhhbbmyy",
        "vpeztkuuggbbfyywwbcddksppddpzhbbmy",
        "vppeztkuggbbfyywbcdksspdppzzhhbmy",
        "vppeezttkkugbbffyywbccddkksppdpzhhbmy",
        "vvppeeztkugbfyywbcdkksppdppzhbmyy",
        "vpezttkuugbbffywbcdksppddpzzhhbbmmy",
        "vppezzttkuugbfyywbcddkksspdpzhbbmmy",
        "vppezzttkkuggbffywbbcdksspdpzzhhbbmmyy",
        "vpezzttkuggbfyywbbccdksspdpzhhbbmmy",
        "vvppezttkkugbffyywbcdkssppdpzzhbmy",
        "vvpeezttkkuuggbbfyywbbccdkspdppzhhbmy",
        "vpeezttkkuugbfywbccddkksppddpzzhhbmmy",
        "vvppezttkuuggbbffywbbccdkksppdpzzhhbbmmy",
        "vvppeeztkuggbbffyywbccdksspddppzzhbmmyy",
        "vvppeezztkuggbfywwbccddkkspddpzhbbmy",
        "vpezttkuuggbfyywwbcdkkspdpzhhbbmmyy",
        "vppezzttkuggbffywbbcdkkssppddppzhhbmyy",
        "vppeztkuuggbffyywbccdkkspdppzzhhbmmyy",
        "vppeezztkuuggbfywbccddkksspddppzhhbbmyy",
        "vvppeztkuugbfywwbccdkkspddppzzhhbmmy",
        "vvpezztkuugbbffyywwbbccddksppdpzhbbmmyy",
        "vvpezzttkkuuggbffyywwbbcdkspdpzhbmmyy",
        "vvppeztkkuuggbbfyywbbccdksppdppzzhbmmyy",
        "vvppezztkuggbffyywwbcddkkssppdpzhbmmyy",
        "vvpezzttkkuggbbffywwbcddkksspdpzzhhbbmmy",
        "vpezztkkuuggbfyywwbccddkssppdppzhhbbmmy",
        "vvppezztkuugbffywwbccdkkspdppzhhbmmy",
        "vpeztkugbfyywwbcdkksspdppzzhbmmy",
        "vvpeezzttkkugbbfywwbcdkkspdpzzhhbmmy",
        "vpezzttkuuggbbfywbccdkspddppzzhhbbmmy",
        "vppeztkkuugbffyywwbbcddksspddppzhbbmyy",
        "vpeztkkuggbffyywbbccddkssppdppzhbmyy",
        "vvppeezztkuggbffyywwbcddkksppdppzhbmyy",
        "vpeezztkugbfyywbbccdkkspdppzhbmmyy",
        "vvppeezttkugbfywwbcddkkssppdppzhbmmyy",
        "vpeeztkuggbffywwbbccddksspdppzzhhbmmy",
        "vvppeeztkuugbfywbcddkssppddppzzhhbbmyy",
        "vpezzttkuggbbffyywwbbccdkssppddppzhbbmy",
        "vpezttkugbfywbbcddkksspddppzhbbmy",
        "vpeezzttkkuggbbffyywwbccddkspddppzhbmyy",
        "vppeezzttkugbffywbccdkkspddpzhhbbmyy",
        "vpezzttkuggbbfyywbbccdkksspddpzzhhbmmy",
        "vvppezttkugbfywwbbcdkksspddpzzhhbbmyy",
        "vppezztkkuggbffyywbcddkkssppddpzzhhbbmmy",
        "vppeztkkuggbfywwbccdkksppdppzhhbmmy",
        "vvpeezzttkugbffyywwbbcddkssppddpzzhbmmy",
        "vvpezztkkuuggbfyywbccdkksspddpzhhbbmyy",
        "vpezttkuuggbffywbbccdksppdpzhbmmyy",
        "vvpezzttkuggbbfywbccddksspdpzzhhbmmy",
        "vvpeezzttkkugbbfywwbcdkksppddpzhbmy",
        "vppeezttkkuugbbfyywwbcddkkspdpzhhbbmmyy",
        "vvppeeztkkuugbbfyywwbbcddkksspdppzhbbmyy",
        "vvpeezzttkkuugbfywwbbcddkspdpzzhbbmyy"};

    EXPECT_NE(2, expressiveWordsFA(s, words));
    EXPECT_EQ(0, expressiveWordsFA(s, words));
    EXPECT_EQ(2, expressiveWordsDS1(s, words));
}
