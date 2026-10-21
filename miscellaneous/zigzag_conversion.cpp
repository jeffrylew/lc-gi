#include <gtest/gtest.h>

#include <string>

//! @brief First attempt to write s in a zigzag pattern for a given num rows
//! @param[in] s       String to write in a zigzag pattern
//! @param[in] numRows The number of rows to write s across
//! @return The line by line converted string
static std::string convertFA(std::string s, int numRows)
{
    //! @details https://leetcode.com/problems/zigzag-conversion/description/

    
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
