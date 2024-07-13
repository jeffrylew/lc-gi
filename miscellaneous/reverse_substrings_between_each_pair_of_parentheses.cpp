#include <gtest/gtest.h>

#include <algorithm>
#include <stack>
#include <string>

//! @brief First attempt to reverse strings in each pair of matching parentheses
//! @param[in] s std::string consisting of lower case English letters/brackets
//! @return std::string with reverse strings in each pair of matching brackets
static std::string reverseParenthesesFA(std::string s)
{
    //! @details https://leetcode.com/problems/
    //!          reverse-substrings-between-each-pair-of-parentheses

    std::string output {};
    output.reserve(s.size());

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
