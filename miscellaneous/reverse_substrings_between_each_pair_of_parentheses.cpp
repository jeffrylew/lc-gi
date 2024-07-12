#include <gtest/gtest.h>

#include <stack>
#include <string>

//! @brief First attempt to reverse strings in each pair of matching parentheses
//! @param[in] s std::string consisting of lower case English letters/brackets
//! @return std::string with reverse strings in each pair of matching brackets
static std::string reverseParenthesesFA(std::string s)
{
    //! @todo

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
