#include <gtest/gtest.h>

#include <string>
#include <vector>

//! @brief First attempt to get min number of operations to get to main folder
//! @param[in] logs Vector of change folder operation strings
//! @return Min number of operations needed to get back to the main folder
static int minOperationsFA(std::vector<std::string> logs)
{
    //! @details https://leetcode.com/problems/crawler-log-folder/
    //!
    //!          Time complexity O(N) where N = logs.size()
    //!          Space complexity O(1)

    int num_ops {};

    for (const auto& instr : logs)
    {
        if (instr == "../")
        {
            if (num_ops > 0)
            {
                --num_ops;
            }
        }
        else if (instr == "./")
        {
            continue;
        }
        else
        {
            ++num_ops;
        }
    }

    return num_ops;
}

TEST(MinOperationsTest, SampleTest1)
{
    EXPECT_EQ(2, minOperationsFA({"d1/", "d2/", "../", "d21/", "./"}));
}

TEST(MinOperationsTest, SampleTest2)
{
    EXPECT_EQ(3, minOperationsFA({"d1/", "d2/", "./", "d3/", "../", "d31/"}));
}

TEST(MinOperationsTest, SampleTest3)
{
    EXPECT_EQ(0, minOperationsFA({"d1/", "../", "../", "../"}));
}
