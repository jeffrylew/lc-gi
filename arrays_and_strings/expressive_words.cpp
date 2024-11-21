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

TEST(ExpressiveWordsTest, SampleTest1)
{
    EXPECT_EQ(1, expressiveWordsFA("heeellooo", {"hello", "hi", "helo"}));
}

TEST(ExpressiveWordsTest, SampleTest2)
{
    EXPECT_EQ(3, expressiveWordsFA("zzzzzyyyyy", {"zzyy", "zy", "zyy"}));
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
}
