#include <gtest/gtest.h>

#include <string>

//! @brief First attempt to write s in a zigzag pattern for a given num rows
//! @param[in] s       String to write in a zigzag pattern
//! @param[in] numRows The number of rows to write s across
//! @return The line by line converted string
static std::string convertFA(std::string s, int numRows)
{
    //! @details https://leetcode.com/problems/zigzag-conversion/description/

    //! @todo
}

TEST(ConvertTest, SampleTest1)
{
    EXPECT_EQ("PAHNAPLSIIGYIR", convertFA("PAYPALISHIRING", 3));
}
