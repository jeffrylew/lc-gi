#include <gtest/gtest.h>

#include <cmath>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

//! @brief First attempt to write s in a zigzag pattern for a given num rows
//! @param[in] s       String to write in a zigzag pattern
//! @param[in] numRows The number of rows to write s across
//! @return The line by line converted string
static std::string convertFA(std::string s, int numRows)
{
    //! @details https://leetcode.com/problems/zigzag-conversion/description/
    //!
    //!          Time complexity O(max(S, numRows)) where S = s.size(). We
    //!          iterate over s in O(S) and we iterate over the row_strings
    //!          vector in O(numRows).
    //!          Space complexity O(S) to store all chars from s in row_strings.

    if (numRows == 1)
    {
        return s;
    }

    std::string converted_string;
    converted_string.reserve(s.size());

    std::vector<std::string> row_strings(numRows);

    int  row_index {};
    bool should_decrement_index {};

    for (const char ch : s)
    {
        row_strings[row_index] += ch;

        if (row_index == 0)
        {
            should_decrement_index = false;
        }
        else if (row_index == numRows - 1)
        {
            should_decrement_index = true;
        }

        if (should_decrement_index)
        {
            --row_index;
        }
        else
        {
            ++row_index;
        }
    }

    for (auto& row_string : row_strings)
    {
        converted_string.insert(converted_string.end(),
                                std::make_move_iterator(row_string.begin()),
                                std::make_move_iterator(row_string.end()));
    }

    return converted_string;
}

//! @brief Simulate zig-zag movement discussion solution
//! @param[in] s       String to write in a zigzag pattern
//! @param[in] numRows The number of rows to write s across
//! @return The line by line converted string
static std::string convertDS1(std::string s, int numRows)
{
    //! @details https://leetcode.com/problems/zigzag-conversion/editorial/
    //!
    //!          Time complexity O(numRows * S) where S = s.size(). The empty 2D
    //!          matrix of size numRows * numCols takes O(numRows * numCols) to
    //!          initialize, where O(numCols) = O(S). We iterate over the input
    //!          s in O(S) and traverse the matrix to convert the string in
    //!          O(numRows * S).
    //!          Space complexity O(numRows * S) for the 2D matrix.

    if (numRows == 1)
    {
        return s;
    }

    const auto num_chars = static_cast<int>(std::ssize(s));

    //! Each section has 2 * numRows - 2 chars. numRows chars in the column and
    //! numRows - 2 chars in the diagonal
    const auto num_sections =
        static_cast<int>(std::ceil(num_chars / (2 * numRows - 2.0)));

    //! Each section has numRows - 1 columns
    const int num_cols {num_sections * (numRows - 1)};

    std::vector<std::vector<char>> matrix(
        numRows, std::vector<char>(num_cols, ' '));

    int curr_row {};
    int curr_col {};
    int curr_string_idx {};

    //! Iterate in zig-zag pattern over matrix and fill with characters
    while (curr_string_idx < num_chars)
    {
        //! Move down
        while (curr_row < numRows && curr_string_idx < num_chars)
        {
            matrix[curr_row][curr_col] = s[curr_string_idx];
            ++curr_row;
            ++curr_string_idx;
        }

        curr_row -= 2;
        ++curr_col;

        //! Move up and to the right
        while (curr_row > 0
               && curr_col < num_cols
               && curr_string_idx < num_chars)
        {
            matrix[curr_row][curr_col] = s[curr_string_idx];
            --curr_row;
            ++curr_col;
            ++curr_string_idx;
        }
    }

    std::string converted_string;
    converted_string.reserve(s.size());

    for (const auto& row : matrix)
    {
        for (const char ch : row)
        {
            if (ch != ' ')
            {
                converted_string += ch;
            }
        }
    }

    return converted_string;
}

//! @brief String traversal discussion solution
//! @param[in] s       String to write in a zigzag pattern
//! @param[in] numRows The number of rows to write s across
//! @return The line by line converted string
static std::string convertDS2(std::string s, int numRows)
{
    //! @details https://leetcode.com/problems/zigzag-conversion/editorial/

    if (numRows == 1)
    {
        return s;
    }

    std::string converted_string;
    converted_string.reserve(s.size());

    const auto num_chars = static_cast<int>(std::ssize(s));
    const int  chars_in_section {2 * (numRows - 1)};

    for (int curr_row = 0; curr_row < numRows; ++curr_row)
    {
        int curr_idx {curr_row};

        while (curr_idx < num_chars)
        {
            converted_string += s[curr_idx];

            //! If curr_row is not the first or last row then there is another
            //! char in the same row of the current section that we need to add
            if (curr_row != 0 && curr_row != numRows - 1)
            {
                const int chars_to_next_idx {chars_in_section - 2 * curr_row};
                const int next_idx {curr_idx + chars_to_next_idx};

                if (next_idx < num_chars)
                {
                    converted_string += s[next_idx];
                }
            }

            //! Jump to the first char in the same row of the next section
            curr_idx += chars_in_section;
        }
    }

    return converted_string;
}

TEST(ConvertTest, SampleTest1)
{
    // 01234567890123
    // 01210121012101
    // PAYPALISHIRING
    // Row 0: {0, P}, {4, A}, {8, H}, {12, N}
    // Row 1: {1, A}, {3, P}, {5, L}, {7, S}, {9, I}, {11, I}, {13, G}
    // Row 2: {2, Y}, {6, I}, {10, R}
    EXPECT_EQ("PAHNAPLSIIGYIR", convertFA("PAYPALISHIRING", 3));
    EXPECT_EQ("PAHNAPLSIIGYIR", convertDS1("PAYPALISHIRING", 3));
    EXPECT_EQ("PAHNAPLSIIGYIR", convertDS2("PAYPALISHIRING", 3));
}

TEST(ConvertTest, SampleTest2)
{
    // 01234567890123
    // 01232101232101
    // PAYPALISHIRING
    // Row 0: {0, P}, {6, I}, {12, N}
    // Row 1: {1, A}, {5, L}, {7, S}, {11, I}, {13, G}
    // Row 2: {2, Y}, {4, A}, {8, H}, {10, R}
    // Row 3: {3, P}, {9, I}
    EXPECT_EQ("PINALSIGYAHRPI", convertFA("PAYPALISHIRING", 4));
    EXPECT_EQ("PINALSIGYAHRPI", convertDS1("PAYPALISHIRING", 4));
    EXPECT_EQ("PINALSIGYAHRPI", convertDS2("PAYPALISHIRING", 4));
}

TEST(ConvertTest, SampleTest3)
{
    EXPECT_EQ("A", convertFA("A", 1));
    EXPECT_EQ("A", convertDS1("A", 1));
    EXPECT_EQ("A", convertDS2("A", 1));
}
