#include <gtest/gtest.h>

#include <stack>
#include <string>
#include <unordered_map>

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

//! @brief Stack discussion solution
//! @param[in] s std::string consisting of parentheses only "()[]{}"
//! @return True if input string is valid
static bool isValidDS(std::string s)
{
    //! @details https://leetcode.com/problems/valid-parentheses/description/
    //!
    //!          Time complexity O(N) where N = s.size(). Traverse given string
    //!          one character at a time. Push/pop operations on stack are O(1).
    //!          Space complexity O(N) since we push all opening brackets onto
    //!          the stack. In the worst case, we end up pushing all brackets
    //!          onto the stack, e.g. "((((((".

    const std::unordered_map<char, char> mappings {
        {')', '('}, {'}', '{'}, {']', '['}};

    std::stack<char> stk {};

    for (const char ch : s)
    {
        if (!mappings.contains(ch))
        {
            //! Opening bracket
            stk.push(ch);
            continue;
        }

        //! Get the top element of the stack
        //! If stack is empty, set a dummy value '#'
        const char top_element = stk.empty() ? '#' : stk.top();
        stk.pop();

        //! If mapping for this bracket doesn't match stack top, return false
        if (top_element != mappings[ch])
        {
            return false;
        }
    }

    return stk.empty();

} // static bool isValidDS( ...

TEST(IsValidTest, SampleTest1)
{
    EXPECT_TRUE(isValidFA("()"));
    EXPECT_TRUE(isValidDS("()"));
}

TEST(IsValidTest, SampleTest2)
{
    EXPECT_TRUE(isValidFA("()[]{}"));
    EXPECT_TRUE(isValidDS("()[]{}"));
}

TEST(IsValidTest, SampleTest3)
{
    EXPECT_FALSE(isValidFA("(]"));
    EXPECT_FALSE(isValidDS("(]"));
}
