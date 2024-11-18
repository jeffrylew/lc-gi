#include <gtest/gtest.h>

#include <string>
#include <vector>

//! @brief First attempt to get number of query strings that are stretchy
//! @param[in] s     std::string to check query against
//! @param[in] words Vector of query strings to check stretchiness of
//! @return Number of query strings that are stretchy
static int expressiveWordsFA(std::string s, std::vector<std::string> words)
{
    //! @todo

} // static int expressiveWordsFA( ...

TEST(ExpressiveWordsTest, SampleTest1)
{
    EXPECT_EQ(1, expressiveWordsFA("heeellooo", {"hello", "hi", "helo"}));
}

TEST(ExpressiveWordsTest, SampleTest2)
{
    EXPECT_EQ(3, expressiveWordsFA("zzzzzyyyyy", {"zzyy", "zy", "zyy"}));
}
