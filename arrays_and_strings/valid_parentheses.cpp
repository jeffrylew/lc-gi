#include <gtest/gtest.h>

#include <stack>
#include <string>
#include <unordered_map>

//! @brief First attempt to determine if input string is valid
//! @param[in] s std::string containing '(', ')', '{', '}', '[', ']'
//! @return True if open brackets are closed by the same type in order
static bool isValidFA(std::string s)
{
    //! @details https://leetcode.com/explore/interview/card/google/59
    //!          /array-and-strings/467/
    //!
    //!          Time complexity O(N) where N = s.size()
    //!          Space complexity O(N) in the worst case where s only contains
    //!          left brackets so left_bracket_stack occupies O(N).

    std::stack<char>                     left_bracket_stack {};
    const std::unordered_map<char, char> brackets {
        {'(', ')'}, {'{', '}'}, {'[', ']'}};

    for (const char bracket : s)
    {
        if (brackets.contains(bracket))
        {
            left_bracket_stack.push(bracket);
            continue;
        }

        if (left_bracket_stack.empty())
        {
            return false;
        }

        const char left_bracket {left_bracket_stack.top()};
        left_bracket_stack.pop();

        if (brackets.at(left_bracket) != bracket)
        {
            return false;
        }
    }

    return left_bracket_stack.empty();

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

TEST(IsValidTest, SampleTest4)
{
    EXPECT_TRUE(isValidFA("([])"));
}
