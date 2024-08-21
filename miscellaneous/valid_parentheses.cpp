#include <gtest/gtest.h>

#include <stack>
#include <string>

//! @brief First attempt to determine if input string is valid
//! @param[in] s std::string consisting of parentheses only "()[]{}"
//! @return True if input string is valid
static bool isValidFA(std::string s)
{
    //! @details https://leetcode.com/problems/valid-parentheses/description/
    //!
    //!          Time complexity O(N) where N = s.size()
    //!          Space complexity O(N) where N = s.size()

    std::stack<char> left_parens {};

    for (const char ch : s)
    {
        if (ch == '(' || ch == '[' || ch == '{')
        {
            left_parens.push(ch);
            continue;
        }

        if (left_parens.empty())
        {
            return false;
        }

        const char left_parenthesis = left_parens.top();
        left_parens.pop();

        if ((left_parenthesis == '(' && ch != ')')
            || (left_parenthesis == '[' && ch != ']')
            || (left_parenthesis == '{' && ch != '}'))
        {
            return false;
        }
    }

    return left_parens.empty();

} // static bool isValidFA( ...

TEST(IsValidTest, SampleTest1)
{
    EXPECT_TRUE(isValidFA("()"));
}

TEST(IsValidTest, SampleTest2)
{
    EXPECT_TRUE(isValidFA("()[]{}"));
}

TEST(IsValidTest, SampleTest3)
{
    EXPECT_FALSE(isValidFA("(]"));
}
