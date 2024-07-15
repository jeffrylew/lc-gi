#include <gtest/gtest.h>

#include <algorithm>
#include <stack>
#include <string>
#include <vector>

//! @brief First attempt to reverse strings in each pair of matching parentheses
//! @param[in] s std::string consisting of lower case English letters/brackets
//! @return std::string with reverse strings in each pair of matching brackets
static std::string reverseParenthesesFA(std::string s)
{
    //! @details https://leetcode.com/problems/
    //!          reverse-substrings-between-each-pair-of-parentheses
    //!
    //!          First attempt solution does not pass SampleTest4

    if (s.size() == 1U)
    {
        return s;
    }

    std::string output {};
    output.reserve(s.size());

    std::vector<std::string> tokenize {};

    for (int idx = 0; idx < static_cast<int>(std::ssize(s)); ++idx)
    {
        if (s[idx] != '(' && s[idx] != ')')
        {
            if (tokenize.empty())
            {
                tokenize.emplace_back() += s[idx];
            }
            else
            {
                tokenize.back() += s[idx];
            }
        }
        else
        {
            tokenize.emplace_back();
        }
    }

    if (tokenize.back().empty())
    {
        tokenize.pop_back();
    }

    const auto tokenize_size = static_cast<int>(std::ssize(tokenize));
    const auto mid           = tokenize_size / 2;
    output                   = tokenize[mid];
    std::reverse(output.begin(), output.end());

    for (int offset = 1; offset <= mid; ++offset)
    {
        output = tokenize[mid - offset] + output + tokenize[mid + offset];
        if (offset != mid || s[0] == '(')
        {
            std::reverse(output.begin(), output.end());
        }
    }

    /*
    std::stack<std::string> substrs {};

    for (const auto& ele : s)
    {
        if (ele == '(')
        {
            substrs.emplace();
        }
        else if (ele == ')')
        {
            auto str = substrs.top();
            substrs.pop();

            std::reverse(str.begin(), str.end());
            std::reverse(output.begin(), output.end());
            output.append(str);
        }
        else
        {
            substrs.top().append(ele);
        }
    }
    */

    return output;

} // static std::string reverseParenthesesFA( ...


//! @brief Straightforward way discussion solution
//! @param[in] s std::string consisting of lower case English letters/brackets
//! @return std::string with reverse strings in each pair of matching brackets
static std::string reverseParenthesesDS1(std::string s)
{
    //! @details https://leetcode.com/problems/
    //!          reverse-substrings-between-each-pair-of-parentheses
    //!
    //!          Time complexity O(N ^ 2) where N = s.size(). The algorithm
    //!          iterates through each input string char once. For each char,
    //!          - If it is (, push to stack in O(1)
    //!          - If it is ), pop from stack and reverse portion of result
    //!            - Popping is O(1)
    //!            - Reverse operation can take up to O(N) in worst case when
    //!              reversing entire string
    //!          - For other characters, append to result in O(1) amortized
    //!          Worst-case scenario occurs when have to reverse large portions
    //!          of string multiple times. In worst case, have to reverse entire
    //!          string for each closing parenthesis.
    //!          Space complexity O(N). In worst case when all chars are opening
    //!          parentheses, stack could take O(N) space (technically O(N / 2)
    //!          = O(N) since the problem guarantees matching parentheses). The
    //!          reverse function typically doesn't use extra space proportional
    //!          to the input size.

    std::string     result {};
    std::stack<int> open_parentheses_indices {};

    for (const char current_char : s)
    {
        if (current_char == '(')
        {
            //! Store the current length as start index for future reversal
            open_parentheses_indices.push(static_cast<int>(std::ssize(result)));
        }
        else if (current_char == ')')
        {
            const int start_idx {open_parentheses_indices.top()};
            open_parentheses_indices.pop();

            //! Reverse substring between matching parentheses
            std::reverse(result.begin() + start_idx, result.end());
        }
        else
        {
            //! Character is not a parenthesis, append to processed string
            result += current_char;
        }
    }

    return result;

} // static std::string reverseParenthesesDS1( ...

//! @brief Wormhole teleportation discussion solution
//! @param[in] s std::string consisting of lower case English letters/brackets
//! @return std::string with reverse strings in each pair of matching brackets
static std::string reverseParenthesesDS2(std::string s)
{
    //! @details https://leetcode.com/problems/
    //!          reverse-substrings-between-each-pair-of-parentheses
    //!
    //!          Time complexity O(N) where N = s.size(). Iterate through string
    //!          once to pair parentheses using a stack. Each char is processed
    //!          once in O(N). After pairing, we iterate through string again to
    //!          construct final result string. Each char is processed once and
    //!          navigate through pairs in constant time, resulting in O(N).
    //!          Space complexity O(N). Use stack to track opening parentheses
    //!          indices. In worst case, stack can hold up to O(N / 2) elements
    //!          when all are opening parentheses. A vector paired_indices of
    //!          size N stores indices of matching parentheses.

    const auto       s_len = static_cast<int>(std::ssize(s));
    std::stack<int>  open_parentheses_indices {};
    std::vector<int> paired_indices(s_len);

    //! First pass: Pair up parentheses
    for (int idx = 0; idx < s_len; ++idx)
    {
        if (s[idx] == '(')
        {
            open_parentheses_indices.push(idx);
        }
        else if (s[idx] == ')')
        {
            const int open_parenthesis_idx {open_parentheses_indices.top()};
            open_parentheses_indices.pop();

            paired_indices[idx]                  = open_parenthesis_idx;
            paired_indices[open_parenthesis_idx] = idx;
        }
    }

    //! Second pass: Build the result string
    std:string result;
    int        direction {1};

    for (int curr_idx = 0; curr_idx < s_len; curr_idx += direction)
    {
        if (s[curr_idx] == '(' || s[curr_idx] == ')')
        {
            curr_idx  = paired_indices[curr_idx];
            direction = -direction;
        }
        else
        {
            result += s[curr_idx];
        }
    }

    return result;

} // static std::string reverseParenthesesDS2( ...

TEST(ReverseParenthesesTest, SampleTest1)
{
    EXPECT_EQ("dcba", reverseParenthesesFA("(abcd)"));
    EXPECT_EQ("dcba", reverseParenthesesDS1("(abcd)"));
    EXPECT_EQ("dcba", reverseParenthesesDS2("(abcd)"));
}

TEST(ReverseParenthesesTest, SampleTest2)
{
    EXPECT_EQ("iloveu", reverseParenthesesFA("(u(love)i)"));
    EXPECT_EQ("iloveu", reverseParenthesesDS1("(u(love)i)"));
    EXPECT_EQ("iloveu", reverseParenthesesDS2("(u(love)i)"));
}

TEST(ReverseParenthesesTest, SampleTest3)
{
    EXPECT_EQ("leetcode", reverseParenthesesFA("(ed(et(oc))el)"));
    EXPECT_EQ("leetcode", reverseParenthesesDS1("(ed(et(oc))el)"));
    EXPECT_EQ("leetcode", reverseParenthesesDS2("(ed(et(oc))el)"));
}

TEST(ReverseParenthesesTest, SampleTest4)
{
    EXPECT_NE("yfgnxf", reverseParenthesesFA("yfgnxf"));
    EXPECT_EQ("yfgnxf", reverseParenthesesDS1("yfgnxf"));
    EXPECT_EQ("yfgnxf", reverseParenthesesDS2("yfgnxf"));
}
