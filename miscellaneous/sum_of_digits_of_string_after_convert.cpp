#include <gtest/gtest.h>

#include <string>

//! @brief First attempt to convert s to an int and then transforming it
//! @param[in] s std::string of letters to convert to an int
//! @param[in] k Number of times to perform summation transformation
//! @return Resulting integer after performing operations
static int getLuckyFA(std::string s, int k)
{
    //! @details leetcode.com/problems/sum-of-digits-of-string-after-convert
    //!
    //!          Time complexity O(N) where N = s.size()
    //!          Space complexity O(1)
    //!
    //!          First attempt solution does not pass SampleTest4

    std::uint64_t result {};

    for (const char letter : s)
    {
        const auto next_digits = 1ULL + static_cast<std::uint64_t>(letter - 'a');
        result = result * (next_digits < 10 ? 10ULL : 100ULL) + next_digits;
    }

    for (int num_ops = 0; num_ops < k; ++num_ops)
    {
        std::uint64_t transform {};

        while (result > 0ULL)
        {
            transform += (result % 10ULL);
            result /= 10ULL;
        }

        result = transform;
    }

    return static_cast<int>(result);

} // static int getLuckyFA( ...

TEST(GetLuckyTest, SampleTest1)
{
    EXPECT_EQ(36, getLuckyFA("iiii", 1));
}

TEST(GetLuckyTest, SampleTest2)
{
    EXPECT_EQ(6, getLuckyFA("leetcode", 2));
}

TEST(GetLuckyTest, SampleTest3)
{
    EXPECT_EQ(8, getLuckyFA("zbax", 2));
}

TEST(GetLuckyTest, SampleTest4)
{
    EXPECT_NE(8, getLuckyFA("fleyctuuajsr", 5));
    EXPECT_EQ(5, getLuckyFA("fleyctuuajsr", 5));
}
