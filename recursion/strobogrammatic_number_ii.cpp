#include <gtest/gtest.h>

#include <string>
#include <vector>

static void find_strobogrammatic_nums(std::vector<std::string>& s_nums,
                                      std::string&              s_num,
                                      int                       s_num_len)
{
    if (std::ssize(s_num) == s_num_len)
    {
        s_nums.push_back(s_num);
        return;
    }

    //! @todo Strobogrammatic number like bracket matching or anagram. Maybe use
    //!       stack while backtracking to get correct order.
}

//! @brief First attempt to get all strobogrammatic numbers of length n
//! @param[in] n Length of strobogrammatic number
//! @return Vector of strobogrammatic numbers that are of length n
static std::vector<std::string> findStrobogrammaticFA(int n)
{
    //! @details leetcode.com/explore/interview/card/google/62/recursion-4/399

    if (n == 1)
    {
        return {"0", "1", "8"};
    }
    
    std::vector<std::string> s_nums;

    for (const int num : {0, 1, 6, 8, 9})
    {
        auto s_num = std::to_string(num);
        find_strobogrammatic_nums(s_nums, s_num, n);
    }

    return s_nums;

} // static std::vector<std::string> findStrobogrammaticFA( ...

TEST(FindStrobogrammaticTest, SampleTest1)
{
    const std::vector<std::string> expected_output {"11", "69", "88", "96"};

    EXPECT_EQ(expected_output, findStrobogrammaticFA(2));
}

TEST(FindStrobogrammaticTest, SampleTest2)
{
    const std::vector<std::string> expected_output {"0", "1", "8"};

    EXPECT_EQ(expected_output, findStrobogrammaticFA(1));
}
