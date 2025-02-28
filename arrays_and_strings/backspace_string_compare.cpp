#include <gtest/gtest.h>

#include <stack>
#include <string>

//! @brief First attempt to check if s and t are equal when typed into editors
//! @param[in] s First string
//! @param[in] t Second string to check equality with
//! @return True if s == t else false
static bool backspaceCompareFA(std::string s, std::string t)
{
    //! @details https://leetcode.com/explore/interview/card/google/59
    //!          /array-and-strings/3060/
    //!
    //!          Time complexity O(S + T) where S = s.size() and T = t.size().
    //!          Loop through each char in s and t and add it to a stack if it
    //!          isn't '#'. Then iterate through each stack, comparing chars.
    //!          Space complexity O(S + T) for s_stack and t_stack

    std::stack<char> s_stack {};
    std::stack<char> t_stack {};

    for (const char ch : s)
    {
        if (ch == '#')
        {
            if (!s_stack.empty())
            {
                s_stack.pop();
            }
            continue;
        }

        s_stack.push(ch);
    }

    for (const char ch : t)
    {
        if (ch == '#')
        {
            if (!t_stack.empty())
            {
                t_stack.pop();
            }
            continue;
        }

        t_stack.push(ch);
    }

    while (!s_stack.empty() && !t_stack.empty())
    {
        if (s_stack.top() != t_stack.top())
        {
            return false;
        }

        s_stack.pop();
        t_stack.pop();
    }

    return s_stack.empty() && t_stack.empty();

} // static bool backspaceCompareFA( ...

TEST(BackspaceCompareTest, SampleTest1)
{
    EXPECT_TRUE(backspaceCompareFA("ab#c", "ad#c"));
}

TEST(BackspaceCompareTest, SampleTest2)
{
    EXPECT_TRUE(backspaceCompareFA("ab##", "c#d#"));
}

TEST(backspaceCompareTest, SampleTest3)
{
    EXPECT_FALSE(backspaceCompareFA("a#c", "b"));
}
