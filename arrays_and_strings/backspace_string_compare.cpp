#include <gtest/gtest.h>

#include <stack>
#include <string>
#include <string_view>

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

//! @brief Build string discussion solution
//! @param[in] s First string
//! @param[in] t Second string to check equality with
//! @return True if s == t else false
static bool backspaceCompareDS1(std::string s, std::string t)
{
    //! @details leetcode.com/problems/backspace-string-compare/editorial
    //!
    //!          Time complexity O(S + T) where S = s.size() and T = t.size()
    //!          Space complexity O(S + T) 

    const auto build_string = [](std::string_view str_before_backspaces) {
        std::string str_after_backspaces {};

        for (const char ch : str_before_backspaces)
        {
            if (ch != '#')
            {
                str_after_backspaces += ch;
            }
            else if (!str_after_backspaces.empty())
            {
                str_after_backspaces.pop();
            }
        }

        return str_after_backspaces;
    };

    return build_string(s) == build_string(t);

} // static bool backspaceCompareDS1( ...

//! @brief Two pointer discussion solution to check if s and t are equal
//! @param[in] s First string
//! @param[in] t Second string to check equality with
//! @return True if s == t else false
static bool backspaceCompareDS2(std::string s, std::string t)
{
    //! @details leetcode.com/problems/backspace-string-compare/editorial
    //!
    //!          Time complexity O(S + T) where S = s.size() and T = t.size()
    //!          Space complexity O(1)

    int s_idx {static_cast<int>(std::ssize(s)) - 1};
    int t_idx {static_cast<int>(std::ssize(t)) - 1};

    int s_num_backspaces {};
    int t_num_backspaces {};

    //! While there are chars in s or t
    while (s_idx >= 0 || t_idx >= 0)
    {
        //! Find position of next non-backspace char in s
        while (s_idx >= 0)
        {
            if (s[s_idx] == '#')
            {
                ++s_num_backspaces;
                --s_idx;
            }
            else if (s_num_backspaces > 0)
            {
                --s_num_backspaces;
                --s_idx;
            }
            else
            {
                break;
            }
        }

        //! Find position of next non-backspace char in t
        while (t_idx >= 0)
        {
            if (t[t_idx] == '#')
            {
                ++t_num_backspaces;
                --t_idx;
            }
            else if (t_num_backspaces > 0)
            {
                --t_num_backspaces;
                --t_idx;
            }
            else
            {
                break;
            }
        }

        //! If one index is < 0 while the other is >= 0
        if ((s_idx >= 0) != (t_idx >= 0))
        {
            return false;
        }

        //! If two non-backspace chars are different
        if (s_idx >= 0 && t_idx >= 0 && s[s_idx] != t[t_idx])
        {
            return false;
        }

        --s_idx;
        --t_idx;

    } // while (s_idx >= 0 || t_idx >= 0)

    return true;

} // static bool backspaceCompareDS2( ...

TEST(BackspaceCompareTest, SampleTest1)
{
    EXPECT_TRUE(backspaceCompareFA("ab#c", "ad#c"));
    EXPECT_TRUE(backspaceCompareDS1("ab#c", "ad#c"));
    EXPECT_TRUE(backspaceCompareDS2("ab#c", "ad#c"));
}

TEST(BackspaceCompareTest, SampleTest2)
{
    EXPECT_TRUE(backspaceCompareFA("ab##", "c#d#"));
    EXPECT_TRUE(backspaceCompareDS1("ab##", "c#d#"));
    EXPECT_TRUE(backspaceCompareDS2("ab##", "c#d#"));
}

TEST(backspaceCompareTest, SampleTest3)
{
    EXPECT_FALSE(backspaceCompareFA("a#c", "b"));
    EXPECT_FALSE(backspaceCompareDS1("a#c", "b"));
    EXPECT_FALSE(backspaceCompareDS2("a#c", "b"));
}
