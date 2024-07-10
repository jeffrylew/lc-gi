#include <gtest/gtest.h>

#include <algorithm>
#include <stack>
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

//! @brief Counter discussion solution
//! @param[in] logs Vector of change folder operation strings
//! @return Min number of operations needed to get back to the main folder
static int minOperationsDS1(std::vector<std::string> logs)
{
    //! @details https://leetcode.com/problems/crawler-log-folder/
    //!
    //!          Time complexity O(N) where N = logs.size(). The algorithm
    //!          iterates through each operation once. String matching ops take
    //!          linear time wrt string length but the length of strings is
    //!          limited to 10 so this does not significantly impact the overall
    //!          time complexity
    //!          Space complexity O(1) for folder_depth

    int folder_depth {};

    for (const auto& current_operation : logs)
    {
        if (current_operation == "../")
        {
            //! Go up one directory if "../" is encountered,
            //! but don't go above the root
            folder_depth = std::max(0, folder_depth - 1);
        }
        else if (current_operation != "./")
        {
            //! Increase depth if operation moves out of current directory
            ++folder_depth;
        }

        //! Ignore "./" operations as they don't change the current folder
    }

    return folder_depth;
}

//! @brief Stack discussion solution
//! @param[in] logs Vector of change folder operation strings
//! @return Min number of operations needed to get back to the main folder
static int minOperationsDS2(std::vector<std::string> logs)
{
    //! @details https://leetcode.com/problems/crawler-log-folder/
    //!
    //!          Time complexity O(N) where N = logs.size(). The algorithm
    //!          iterates through each operation once and each push/pop is O(1).
    //!          Space complexity O(N), folder_stack can store up to N entries.

    std::stack<std::string> folder_stack {};

    for (const auto& current_operation : logs)
    {
        if (current_operation == "../")
        {
            //! Move up to parent directory if not already at main folder
            if (!folder_stack.empty())
            {
                folder_stack.pop();
            }
        }
        else if (current_operation != "./")
        {
            //! Enter a new folder
            folder_stack.push(current_operation);
        }

        //! Ignore "./" operations as they don't change the current folder
    }

    return static_cast<int>(folder_stack.size());
}

TEST(MinOperationsTest, SampleTest1)
{
    const std::vector<int> logs {"d1/", "d2/", "../", "d21/", "./"};

    EXPECT_EQ(2, minOperationsFA(logs));
    EXPECT_EQ(2, minOperationsDS1(logs));
    EXPECT_EQ(2, minOperationsDS2(logs));
}

TEST(MinOperationsTest, SampleTest2)
{
    const std::vector<int> logs {"d1/", "d2/", "./", "d3/", "../", "d31/"};

    EXPECT_EQ(3, minOperationsFA(logs));
    EXPECT_EQ(3, minOperationsDS1(logs));
    EXPECT_EQ(3, minOperationsDS2(logs));
}

TEST(MinOperationsTest, SampleTest3)
{
    const std::vector<int> logs {"d1/", "../", "../", "../"};

    EXPECT_EQ(0, minOperationsFA(logs));
    EXPECT_EQ(0, minOperationsDS1(logs));
    EXPECT_EQ(0, minOperationsDS2(logs));
}
