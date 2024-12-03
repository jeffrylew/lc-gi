#include <gtest/gtest.h>

#include <string>
#include <vector>

static std::string findReplaceStringFA(std::string              s,
                                       std::vector<int>         indices,
                                       std::vector<std::string> sources,
                                       std::vector<std::string> targets)
{
    //! @todo

} // static std::string findReplaceStringFA( ...

TEST(FindReplaceStringTest, SampleTest1)
{
    EXPECT_EQ("eeebffff", findReplaceStringFA("abcd",
                                              {0, 2},
                                              {"a", "cd"},
                                              {"eee", "ffff"}));
}

TEST(FindReplaceStringTest, SampleTest2)
{
    EXPECT_EQ("eeecd", findReplaceStringFA("abcd",
                                           {0, 2},
                                           {"ab", "ec"},
                                           {"eee", "ffff"}));
}
