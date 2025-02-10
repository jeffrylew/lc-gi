#include <gtest/gtest.h>

#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

using Start_char_words_t = std::unordered_map<char, std::vector<std::string>>;

//! @brief Check if word can be inserted into word_square at curr_row_idx
//! @param[in] word_square      Reference to current [partial] word_square
//! @param[in] word_to_validate Word to check if it can be inserted
//! @param[in] curr_row_idx     Index of row that word_to_validate will go into
//! @return True if word_to_validate can be inserted at curr_row_idx, else false
[[nodiscard]] static bool
    is_word_valid_at_row(const std::vector<std::string>& word_square,
                         std::string_view                word_to_validate,
                         int                             curr_row_idx)
{
    //! @details Row 0 doesn't have any restrictions on chars
    //!          Row 1 must have row1_word[0] == row0_word[1]
    //!
    //!          Row 2 must have row2_word[0] == row0_word[2]
    //!                          row2_word[1] == row1_word[2]
    //!
    //!          Row 3 must have row3_word[0] == row0_word[3]
    //!                          row3_word[1] == row1_word[3]
    //!                          row3_word[2] == row2_word[3]

    for (int idx = 0; idx < curr_row_idx; ++idx)
    {
        //! row0_word, row1_word, or row2_word
        const auto& word_at_row_idx = word_square.at(idx);

        if (word_to_validate[idx] != word_at_row_idx.at(curr_row_idx))
        {
            return false;
        }
    }

    return true;

} // [[nodiscard]] static bool is_word_valid_at_row( ...

//! @brief Find all word squares from curr_word_square and add to word_squares
//! @param[out]     word_squares          Reference to output word_squares
//! @param[in, out] curr_word_square      Reference to current word_square
//! @param[in]      words_with_start_char Reference to start chars and words
//! @param[in]      fill_row_idx          Index of row to fill in word square
static void find_word_squares_FA(
    std::vector<std::vector<std::string>>& word_squares,
    std::vector<std::string>&              curr_word_square,
    const Start_char_words_t&              words_with_start_char,
    int                                    fill_row_idx)
{
    const auto& row0_word        = curr_word_square[0];
    const auto  word_square_size = static_cast<int>(std::ssize(row0_word));

    if (fill_row_idx == word_square_size)
    {
        if (std::ssize(curr_word_square) == word_square_size)
        {
            word_squares.push_back(curr_word_square);
        }
        return;
    }

    const char start_char {row0_word[fill_row_idx]};

    if (!words_with_start_char.contains(start_char))
    {
        //! No words start with start_char at fill_row_idx
        return;
    }

    for (const auto& next_word : words_with_start_char.at(start_char))
    {
        if (!is_word_valid_at_row(curr_word_square, next_word, fill_row_idx))
        {
            continue;
        }

        curr_word_square.push_back(next_word);

        find_word_squares_FA(word_squares,
                             curr_word_square,
                             words_with_start_char,
                             fill_row_idx + 1);

        curr_word_square.pop_back();
    }

} // static void find_word_squares( ...

//! @brief First attempt to get all word squares from vector of unique strings
//! @param[in] words Vector of unique strings to build word squares from
//! @return All word squares from words. Same word can be used multiple times.
static std::vector<std::vector<std::string>>
    wordSquaresFA(const std::vector<std::string>& words)
{
    //! @details leetcode.com/explore/interview/card/google/62/recursion-4/370
    //!
    //!          Time complexity O(N ^ 4) where N = words.size(). For each row
    //!          in a word square, there are O(N) options that could be inserted
    //!          Space complexity O(N) for words_with_start_char map. Each word
    //!          has a fixed size of four chars and word_square will have four
    //!          strings. The space for the recursion stack has a depth of up to
    //!          four calls.

    std::vector<std::vector<std::string>> word_squares {};

    //! Map of <start char, vector of words that begin with start char>
    Start_char_words_t words_with_start_char {};

    for (const auto& word : words)
    {
        words_with_start_char[word[0]].push_back(word);
    }

    for (const auto& starting_word : words)
    {
        std::vector<std::string> word_square {starting_word};

        find_word_squares_FA(word_squares,
                             word_square,
                             words_with_start_char,
                             1);
    }

    return word_squares;

} // static std::vector<std::vector<std::string>> wordSquaresFA( ...

using Prefix_map_t = std::unordered_map<std::string, std::vector<std::string>>;

[[nodiscard]] static Prefix_map_t build_prefix_hash_table_DS1(
    const std::vector<std::string>& words)
{
    Prefix_map_t prefix_map {};

    const auto word_square_size = static_cast<int>(std::ssize(words[0]));

    for (const auto& word : words)
    {
        for (int prefix_len = 1; prefix_len < word_square_size; ++prefix_len)
        {
            auto prefix = word.substr(0, prefix_len);
            prefix_map[std::move(prefix)].push_back(word);
        }
    }

    return prefix_map;
}

static void find_word_squares(
    int                                    step,
    const Prefix_map_t&                    prefix_map,
    std::vector<std::string>&              word_square,
    std::vector<std::vector<std::string>>& word_squares)
{
    if (step == std::ssize(word_squares[0]))
    {
        word_squares.push_back(word_square);
        return;
    }

    std::string prefix {};
    for (const auto& word : word_square)
    {
        prefix += word[step];
    }

    if (!prefix_map.contains(prefix))
    {
        return;
    }

    for (const auto& candidate : prefix_map.at(prefix))
    {
        word_square.push_back(candidate);
        find_word_squares(step + 1, prefix_map, word_square, word_squares);
        word_square.pop_back();
    }
}

//! @brief Backtracking with hash table discussion solution
//! @param[in] words Vector of unique strings to build word squares from
//! @return All word squares from words. Same word can be used multiple times.
static std::vector<std::vector<std::string>>
    wordSquaresDS1(const std::vector<std::string>& words)
{
    //! @details https://leetcode.com/problems/word-squares/editorial/

    std::vector<std::vector<std::string>> word_squares {};

    const auto prefix_map = build_prefix_hash_table_DS1(words);

    for (const auto& word : words)
    {
        std::vector<std::string> word_square {word};
        find_word_squares(1, prefix_map, word_square, word_squares);
    }

    return word_squares;

} // static std::vector<std::vector<std::string>> wordSquaresDS1( ...

TEST(WordSquaresTest, SampleTest1)
{
    //! Order of output does not matter, just order of words in each word square
    //!     b a l l    w a l l
    //!     a r e a    a r e a
    //!     l e a d    l e a d
    //!     l a d y    l a d y
    const std::vector<std::vector<std::string>> expected_output {
        {"ball", "area", "lead", "lady"}, {"wall", "area", "lead", "lady"}};

    const std::vector<std::string> words {
        "area", "lead", "wall", "lady", "ball"};

    EXPECT_EQ(expected_output, wordSquaresFA(words));
    EXPECT_EQ(expected_output, wordSquaresDS1(words));
}

TEST(WordSquaresTest, SampleTest2)
{
    //! Order of output does not matter, just order of words in each word square
    //!     b a b a    b a b a
    //!     a b a t    a b a t
    //!     b a b a    b a b a
    //!     a t a l    a t a n
    const std::vector<std::vector<std::string>> expected_output {
        {"baba", "abat", "baba", "atal"}, {"baba", "abat", "baba", "atan"}};

    const std::vector<std::string> words {"abat", "baba", "atan", "atal"};

    EXPECT_EQ(expected_output, wordSquaresFA(words));
    EXPECT_EQ(expected_output, wordSquaresDS1(words));
}
