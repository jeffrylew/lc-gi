#include <gtest/gtest.h>

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

TEST(ConvertTest, SampleTest1)
{
    // 01234567890123
    // 01210121012101
    // PAYPALISHIRING
    // Row 0: {0, P}, {4, A}, {8, H}, {12, N}
    // Row 1: {1, A}, {3, P}, {5, L}, {7, S}, {9, I}, {11, I}, {13, G}
    // Row 2: {2, Y}, {6, I}, {10, R}
    EXPECT_EQ("PAHNAPLSIIGYIR", convertFA("PAYPALISHIRING", 3));
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
}

TEST(ConvertTest, SampleTest3)
{
    EXPECT_EQ("A", convertFA("A", 1));
}
