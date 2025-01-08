#include <gtest/gtest.h>

#include <string>

//! @brief First attempt to find length of last word in the string
//! @param[in] s std::string consisting of words and spaces
//! @return Length of the last word in the string
static int lengthOfLastWordFA(std::string s)
{
    //! @details https://leetcode.com/problems/length-of-last-word/description/
    //!
    //!          Time complexity O(N), N = s.size()
    //!          Space complexity O(1)

    int last_word_length {};

    auto rit = s.rbegin();

    //! Skip any trailing spaces
    while (rit != s.rend())
    {
        if (*rit != ' ')
        {
            break;
        }

        ++rit;
    }

    //! Count last word length up to the first space before the last word
    while (rit != s.rend())
    {
        if (*rit == ' ')
        {
            break;
        }

        ++last_word_length;
        ++rit;
    }

    return last_word_length;

} // static int lengthOfLastWordFA( ...

TEST(LengthOfLastWordTest, SampleTest1)
{
    EXPECT_EQ(5, lengthOfLastWordFA("Hello World"));
}

TEST(LengthOfLastWordTest, SampleTest2)
{
    EXPECT_EQ(4, lengthOfLastWordFA("   fly me   to   the moon  "));
}

TEST(LengthOfLastWordTest, SampleTest3)
{
    EXPECT_EQ(6, lengthOfLastWordFA("luffy is still joyboy"));
}
