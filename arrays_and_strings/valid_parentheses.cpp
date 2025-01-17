#include <gtest/gtest.h>

#include <stack>
#include <string>
#include <unordered_map>

//! @brief First attempt to determine if input string is valid
//! @param[in] s std::string containing '(', ')', '{', '}', '[', ']'
//! @return True if open brackets are closed by the same type in order
static bool isValidFA(std::string s)
{

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
