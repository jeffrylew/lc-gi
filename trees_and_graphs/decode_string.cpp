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

//! @brief Using stack discussion solution to get decoded string
//! @param[in] s String where k[encoded_string] has encoded_string repeated k
//! @return Decoded string
static std::string decodeStringDS1(std::string s)
{
    //! @details https://leetcode.com/problems/decode-string/editorial/
    //!
    //!          Time complexity O(maxK ^ countK * n) where maxK is max value of
    //!          k, countK is the count of nested k values, and n is max length
    //!          of encoded string. For example, for s = 20[a10[bc]], maxK = 20,
    //!          countK = 2 since there are two nested k values (20 and 10), and
    //!          n = 2 which is max length of encoded string between a and bc.
    //!          Worst case scenario is when there are multiple nested patterns.
    //!          Space complexity O(sum(maxK ^ countK * n)) where maxK is max
    //!          value of k, countK is the count of nested k values, and n is
    //!          the max length of encoded string. The max stack size is the sum
    //!          of all decoded strings in the form maxK[nmaxK[n]].

    std::stack<char> stack {};

    for (const char ele : s)
    {
        if (ele != ']')
        {
            //! Push current character to stack
            stack.push(ele);
            continue;
        }

        // ele == ']'
        std::string decoded_string {};

        //! Get encoded string
        while (stack.top() != '[')
        {
            decoded_string += stack.top();
            stack.pop();
        }

        //! Pop [ from stack
        stack.pop();

        int base {1};
        int k {};

        //! Get the number k
        while (!stack.empty()
               && std::isdigit(static_cast<unsigned char>(stack.top())))
        {
            k += static_cast<int>(stack.top() - '0') * base;
            stack.pop();
            base *= 10;
        }

        //! Decode k[decoded_string] by pushing decoded_string k times on stack
        while (k != 0)
        {
            for (int idx = static_cast<int>(std::ssize(decoded_string)) - 1;
                 idx >= 0;
                 --idx)
            {
                stack.push(decoded_string[idx]);
            }

            --k;
        }

    } // for (const char ele : s)

    //! Get result from stack
    const auto  stack_size = static_cast<int>(std::ssize(stack));
    std::string result {};
    result.reserve(stack_size);

    for (int idx = stack_size - 1; idx >= 0; --idx)
    {
        result = stack.top() + result;
        stack.pop();
    }

    return result;

} // static std::string decodeStringDS1( ...

//! @brief Using two stacks discussion solution to get decoded string
//! @param[in] s String where k[encoded_string] has encoded_string repeated k
//! @return Decoded string
static std::string decodeStringDS2(std::string s)
{
    //! @details https://leetcode.com/problems/decode-string/editorial/
    //!
    //!          Time complexity O(maxK * n) where maxK is max value of k and n
    //!          is s.size(). We traverse string of size n and iterate k times
    //!          to decode each pattern of form k[string]. Thus, the worst case
    //!          time complexity is O(maxK * n).
    //!          Space complexity O(m + n) where m is number of letters a-z and
    //!          n is the number of digits 0-9 in string s. In the worst case,
    //!          the max size of string_stack and count_stack are m and n.

    //! Stores all integer k
    std::stack<int> count_stack {};

    //! Stores all decoded strings
    std::stack<std::string> string_stack {};

    std::string current_string {};
    int         k {};

    for (const char ch : s)
    {
        if (std::isdigit(static_cast<unsigned int>(ch)))
        {
            k = k * 10 + static_cast<int>(ch - '0');
        }
        else if (ch == '[')
        {
            //! Push k to count_stack
            count_stack.push(k);

            //! Push current_string to string_stack
            string_stack.push(std::move(current_string));

            //! Reset current_string and k
            current_string.clear();
            k = 0;
        }
        else if (ch == ']')
        {
            //! Begin decoding process. string_stack contains the previously
            //! decoded string
            auto decoded_string = string_stack.top();
            string_stack.pop();

            //! Append current_string k times to decode curr_k[current_string]
            //! Update: decoded_string = decoded_string + curr_k[current_string]
            for (int curr_k = count_stack.top(); curr_k > 0; --curr_k)
            {
                decoded_string += current_string;
            }

            count_stack.pop();
            current_string = decoded_string;
        }
        else
        {
            //! Current character is a letter
            current_string += ch;
        }

    } // for (const char ch : s)

    return current_string;

} // static std::string decodeStringDS2( ...

TEST(DecodeStringTest, SampleTest1)
{
    EXPECT_EQ("aaabcbc", decodeStringFA("3[a]2[bc]"));
    EXPECT_EQ("aaabcbc", decodeStringDS1("3[a]2[bc]"));
    EXPECT_EQ("aaabcbc", decodeStringDS2("3[a]2[bc]"));
}

TEST(DecodeStringTest, SampleTest2)
{
    EXPECT_EQ("accaccacc", decodeStringFA("3[a2[c]]"));
    EXPECT_EQ("accaccacc", decodeStringDS1("3[a2[c]]"));
    EXPECT_EQ("accaccacc", decodeStringDS2("3[a2[c]]"));
}

TEST(DecodeStringTest, SampleTest3)
{
    EXPECT_EQ("abcabccdcdcdef", decodeStringFA("2[abc]3[cd]ef"));
    EXPECT_EQ("abcabccdcdcdef", decodeStringDS1("2[abc]3[cd]ef"));
    EXPECT_EQ("abcabccdcdcdef", decodeStringDS2("2[abc]3[cd]ef"));
}
