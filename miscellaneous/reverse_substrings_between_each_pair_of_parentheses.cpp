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

TEST(ReverseParenthesesTest, SampleTest1)
{
    EXPECT_EQ("dcba", reverseParenthesesFA("(abcd)"));
}

TEST(ReverseParenthesesTest, SampleTest2)
{
    EXPECT_EQ("iloveu", reverseParenthesesFA("(u(love)i)"));
}

TEST(ReverseParenthesesTest, SampleTest3)
{
    EXPECT_EQ("leetcode", reverseParenthesesFA("(ed(et(oc))el)"));
}

TEST(ReverseParenthesesTest, SampleTest4)
{
    EXPECT_NE("yfgnxf", reverseParenthesesFA("yfgnxf"));
}
