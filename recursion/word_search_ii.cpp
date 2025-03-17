#include <gtest/gtest.h>

#include <functional>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

//! @struct TrieNode
//! @brief Defines a node in a trie/prefix tree
struct TrieNode
{
    std::unordered_map<char, TrieNode> children;
};

//! @brief Build a trie from a vector of words for first attempt solution
//! @param[in] words Reference to a vector of strings to add to the trie
//! @return The root TrieNode of the trie
[[nodiscard]] static TrieNode build_trie_FA(
    const std::vector<std::string>& words)
{
    //! @details Time complexity O(N * K) where N = words.size() and K = average
    //!          length of strings in words.

    TrieNode root;

    for (const auto& word : words)
    {
        auto* curr_node = &root;

        for (const char ch : word)
        {
            const auto& [ch_it, did_insert] =
                curr_node.children.try_emplace(ch, TrieNode {});

            curr_node = &(ch_it->second);
        }

        //! At this point, have a complete word
    }

    return root;

} // [[nodiscard]] static TrieNode build_trie_FA( ...

//! @brief First attempt to get all words on m x n board from list of words
//! @param[in] board Reference to m x n board of characters
//! @param[in] words Reference to vector of strings to search for
//! @return Vector of words from list that were found in board
static std::vector<std::string> findWordsFA(
    const std::vector<std::vector<char>>& board,
    const std::vector<std::string>&       words)
{
    //! @details leetcode.com/explore/interview/card/google/62/recursion-4/462

    //! Build trie from words
    const auto root_of_trie = build_trie_FA(words);

    const auto nrows    = static_cast<int>(std::ssize(board));
    const auto ncols    = static_cast<int>(std::ssize(board[0]));
    const auto is_valid = [&](int row, int col) {
        return row >= 0 && row < nrows && col >= 0 && col < ncols;
    };

    //! Directions when traversing the board: up, left, right, and down
    const std::vector<std::pair<int, int>> directions {
        {-1, 0}, {0, -1}, {0, 1}, {1, 0}};

    //! Keep track of visited positions
    std::set<std::pair<int, int>> visited_pos;

    //! Output vector storing all words on the board
    std::vector<std::string> words_on_board;

    //! Helper function to get all words at given position
    std::function<void(int, int, TrieNode*)> get_words_at_pos =
        [&](int row, int col, TrieNode* curr_node) {
            //! @todo Do I need to handle base case(s)?

            for (const auto& [drow, dcol] : directions)
            {
                const int next_row {row + drow};
                const int next_col {col + dcol};
                auto      next_pos = std::make_pair(next_row, next_col);

                if (is_valid(next_row, next_col)
                    && !visited_pos.contains(next_pos))
                {
                    visited_pos.insert(std::move(next_pos));

                    const char next_ch {board.at(next_row).at(next_col)};
                    if (curr_node->children.contains(next_ch))
                    {
                        //! @todo How do I know when to add to words_on_board?
                        get_words_at_pos(next_row,
                                         next_col,
                                         &curr_node->children.at(next_ch));
                    }
                }
            }
    };

    //! Traverse over the board and find all words
    for (int row = 0; row < nrows; ++row)
    {
        for (int col = 0; col < ncols; ++col)
        {
            if (!root_of_trie.children.contains(board[row][col]))
            {
                //! No words start with current char so skip it
                continue;
            }

            //! Reset visited positions at each starting position
            visited_pos = {std::make_pair(row, col)};
            get_words_at_pos(row, col, &root_of_trie);
        }
    }

    return words_on_board;

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
