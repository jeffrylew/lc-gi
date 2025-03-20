#include <gtest/gtest.h>

#include <functional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

//! @struct TrieNode
//! @brief Defines a node in a trie/prefix tree
struct TrieNode
{
    //! If TrieNode is at the end of a complete word, stores the word
    std::string_view complete_word;

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
                curr_node->children.try_emplace(ch, TrieNode {});

            curr_node = &(ch_it->second);
        }

        //! At this point, have a complete word
        curr_node->complete_word = word;
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
    //!
    //!          First attempt solution does not pass SampleTest5. Need to use
    //!          backtracking to "unvisit" a position because DFS searches the
    //!          incorrect direction first and prevents the corresponding cell
    //!          from being traversed in another path.

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

    //! Output set storing all words on the board
    std::unordered_set<std::string> words_on_board;

    //! Helper function to get all words at given position
    std::function<void(int, int, const TrieNode*)> get_words_at_pos =
        [&](int row, int col, const TrieNode* curr_node) {
            if (!curr_node->complete_word.empty())
            {
                words_on_board.emplace(curr_node->complete_word);
            }

            for (const auto& [drow, dcol] : directions)
            {
                const int next_row {row + drow};
                const int next_col {col + dcol};
                auto      next_pos = std::make_pair(next_row, next_col);

                if (is_valid(next_row, next_col)
                    && !visited_pos.contains(next_pos))
                {
                    visited_pos.insert(next_pos);

                    const char next_ch {board.at(next_row).at(next_col)};
                    if (curr_node->children.contains(next_ch))
                    {
                        get_words_at_pos(next_row,
                                         next_col,
                                         &curr_node->children.at(next_ch));
                        visited_pos.erase(std::move(next_pos));
                    }
                }
            }
    };

    //! Traverse over the board and find all words
    for (int row = 0; row < nrows; ++row)
    {
        for (int col = 0; col < ncols; ++col)
        {
            const char init_ch {board[row][col]};

            if (!root_of_trie.children.contains(init_ch))
            {
                //! No words start with current char so skip it
                continue;
            }

            //! Reset visited positions at each starting position
            visited_pos = {std::make_pair(row, col)};
            get_words_at_pos(row, col, &root_of_trie.children.at(init_ch));
        }
    }

    return std::vector<std::string> {words_on_board.begin(),
                                     words_on_board.end()};

} // static std::vector<std::string> findWordsFA( ...

struct Trie_node
{
    std::string_view word;

    std::unordered_map<char, Trie_node> children;
};

static void get_words_at_pos_DS(int                             row,
                                int                             col,
                                Trie_node&                      parent,
                                std::vector<std::vector<char>>& board_in,
                                std::vector<std::string>&       words_on_board)
{
    const char orig_letter {board_in[row][col]};
    auto&      curr_node = parent.children[orig_letter];

    //! Check if there is a matching word
    if (!curr_node.word.empty())
    {
        words_on_board.emplace_back(curr_node.word);

        //! Prevent duplicate entries
        curr_node.word = {};
    }

    //! Mark the current letter as visited before checking neighbors
    board_in[row][col] = '#';

    //! Explore neighbor cells clockwise: up, right, down, left
    const std::vector<std::pair<int, int>> directions {
        {-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    for (const auto& [row_offset, col_offset] : directions)
    {
        const int next_row {row + row_offset};
        const int next_col {col + col_offset};
        if (next_row < 0
            || next_row >= std::ssize(board_in)
            || next_col < 0
            || next_col >= std::ssize(board_in[0]))
        {
            continue;
        }

        if (curr_node.children.contains(board_in[next_row][next_col]))
        {
            get_words_at_pos_DS(next_row,
                                next_col,
                                curr_node,
                                board_in,
                                words_on_board);
        }
    }

    //! End of checking neighbors, restore the original letter
    board_in[row][col] = orig_letter;

    //! Optimization: Incrementally remove the leaf nodes
    if (curr_node.children.empty())
    {
        parent.children.erase(orig_letter);
    }

} // static void get_words_at_pos_DS( ...

//! @brief Backtracking with Trie discussion solution
//! @param[in] board Reference to m x n board of characters
//! @param[in] words Reference to vector of strings to search for
//! @return Vector of words from list that were found in board
static std::vector<std::string> findWordsDS(
    const std::vector<std::vector<char>>& board,
    const std::vector<std::string>&       words)
{
    //! @details https://leetcode.com/problems/word-search-ii/

    //! Step 1) Construct the Trie
    Trie_node root;
    for (const auto& word : words)
    {
        auto* node = &root;
        for (const char letter : word)
        {
            node = &node->children[letter];
        }

        //! Store word in Trie
        node->word = word;
    }

    auto                     board_cpy = board;
    std::vector<std::string> words_on_board;

    //! Step 2) Start backtracking from each cell in the board
    for (int row = 0; row < std::ssize(board); ++row)
    {
        for (int col = 0; col < std::ssize(board[0]); ++col)
        {
            if (root.children.contains(board[row][col]))
            {
                get_words_at_pos_DS(row, col, root, board_cpy, words_on_board);
            }
        }
    }

    return words_on_board;

} // static std::vector<std::string> findWordsDS( ...

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
    EXPECT_EQ(expected_output, findWordsDS(board, words));
}

TEST(FindWordsTest, SampleTest2)
{
    const std::vector<std::vector<char>> board {{'a', 'b'}, {'c', 'd'}};
    const std::vector<std::string>       words {"abcb"};

    EXPECT_TRUE(findWordsFA(board, words).empty());
    EXPECT_TRUE(findWordsDS(board, words).empty());
}

TEST(FindWordsTest, SampleTest3)
{
    const std::vector<std::vector<char>> board {{'a'}};
    const std::vector<std::string>       words {"a"};
    const std::vector<std::string>       expected_output {"a"};

    EXPECT_EQ(expected_output, findWordsFA(board, words));
    EXPECT_EQ(expected_output, findWordsDS(board, words));
}

TEST(FindWordsTest, SampleTest4)
{
    const std::vector<std::vector<char>> board {
        {'o', 'a', 'b', 'n'},
        {'o', 't', 'a', 'e'},
        {'a', 'h', 'k', 'r'},
        {'a', 'f', 'l', 'v'}};

    const std::vector<std::string> words {"oa", "oaa"};
    const std::vector<std::string> expected_output {"oa", "oaa"};

    EXPECT_EQ(expected_output, findWordsFA(board, words));
    EXPECT_EQ(expected_output, findWordsDS(board, words));
}

TEST(FindWordsTest, SampleTest5)
{
    const std::vector<std::vector<char>> board {
        {'a', 'b', 'c', 'e'},
        {'x', 'x', 'c', 'd'},
        {'x', 'x', 'b', 'a'}};

    const std::vector<std::string> words {"abc", "abcd"};
    const std::vector<std::string> expected_output {"abc", "abcd"};
    const std::vector<std::string> FA_output {"abc"};

    EXPECT_NE(expected_output, findWordsFA(board, words));
    EXPECT_EQ(FA_output, findWordsFA(board, words));
    EXPECT_EQ(expected_output, findWordsDS(board, words));
}
