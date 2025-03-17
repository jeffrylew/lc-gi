#include <gtest/gtest.h>

#include <string>
#include <vector>

//! @brief First attempt to get all words on m x n board from list of words
//! @param[in] board Reference to m x n board of characters
//! @param[in] words Reference to vector of strings to search for
//! @return Vector of words from list that were found in board
static std::vector<std::string> findWordsFA(
    const std::vector<std::vector<char>>& board,
    const std::vector<std::string>&       words)
{

} // static std::vector<std::string> findWordsFA( ...

TEST(FindWordsTest, SampleTest1)
{
    const std::vector<std::vector<char>> board {
        {'o', 'a', 'a', 'n'},
        {'e', 't', 'a', 'e'},
        {'i', 'h', 'k', 'r'},
        {'i', 'f', 'l', 'v'}};

    const std::vector<std::string> words {"oath", "pea", "eat", "rain"};
    const std::vector<std::string> expected_output {"eat", "oath"};

    EXPECT_EQ(expected_output, findWordsFA(board, words));
}

TEST(FindWordsTest, SampleTest2)
{
    const std::vector<std::vector<char>> board {{'a', 'b'}, {'c', 'd'}};
    const std::vector<std::string>       words {"abcb"};

    EXPECT_TRUE(findWordsFA(board, words).empty());
}
