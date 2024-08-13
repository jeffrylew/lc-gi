#include <gtest/gtest.h>

#include <cctype>
#include <stack>
#include <string>
#include <utility>

//! @brief First attempt to get decoded string from encoded string
//! @param[in] s String where k[encoded_string] has encoded_string repeated k
//! @return Decoded string
static std::string decodeStringFA(std::string s)
{
    std::string decoded_str {};

    //! <multiplier k, left bracket index>
    std::stack<std::pair<int, int>> k_left_bracket_idx {};

    for (int idx = 0; idx < static_cast<int>(s.size()); ++idx)
    {
        const auto curr_char = static_cast<unsigned char>(s[idx]);

        if (std::isalpha(curr_char) != 0)
        {
            decoded_str += ele;
        }
        else if (curr_char == '[')
        {
            k_left_bracket_idx.emplace(static_cast<int>(s[idx - 1] - '0'),
                                       static_cast<int>(std::ssize(s)));
        }
        else if (curr_char == ']')
        {
            //! Found right bracket
            const auto [k, start_idx] = k_left_bracket_idx.top();
            k_left_bracket_idx.pop();

            //! @todo Figure out how to repeat k times and insert the substring
            //!       from [decoded_str.begin() + start_idx, decoded_str.end()].
            //!       Maybe need another structure in addition to decoded_str.
        }

        //! If character is a digit, skip it. Already processed with '['
    }

    return decoded_str;

} // static std::string decodeStringFA( ...

TEST(DecodeStringTest, SampleTest1)
{
    EXPECT_EQ("aaabcbc", decodeStringFA("3[a]2[bc]"));
}

TEST(DecodeStringTest, SampleTest2)
{
    EXPECT_EQ("accaccacc", decodeStringFA("3[a2[c]]"));
}

TEST(DecodeStringTest, SampleTest3)
{
    EXPECT_EQ("abcabccdcdcdef", decodeStringFA("2[abc]3[cd]ef"));
}
