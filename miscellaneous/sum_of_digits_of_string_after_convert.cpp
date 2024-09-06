#include <gtest/gtest.h>

#include <cstdint>
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

    std::uint64_t result {};

    for (const char letter : s)
    {
        const auto next_digits = 1ULL + static_cast<std::uint64_t>(letter - 'a');

        result += next_digits % 10ULL;
        result += next_digits < 10 ? 0ULL : next_digits / 10ULL;
    }

    for (int num_ops = 1; num_ops < k; ++num_ops)
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

//! @brief String concatenation to summation
//! @param[in] s std::string of letters to convert to an int
//! @param[in] k Number of times to perform summation transformation
//! @return Resulting integer after performing operations
static int getLuckyDS1(std::string s, int k)
{
    //! @details leetcode.com/problems/sum-of-digits-of-string-after-convert
    //!
    //!          Time complexity O(N) where N = s.size(). For each character in
    //!          s, we compute its numeric value and append it to numeric_string
    //!          Perform this transformation k times. In each transformation, we
    //!          iterate over the digits of numeric_string. In the worst case,
    //!          each character contributes 2 digits so the length could be 2N.
    //!          The time complexity for applying transformations k times is
    //!          O(k * N) = O(N) since k is a constant.
    //!          Space complexity O(N). Use space proportional to length of
    //!          numeric_string.

    //! Convert each character to its numerical value and build a string
    std::string numeric_string {};
    for (const char ch : s)
    {
        numeric_string += std::to_string(static_cast<int>(ch - 'a') + 1);
    }

    //! Apply digit sum transformations k times
    while (k-- > 0)
    {
        int digit_sum {};

        for (const char digit : numeric_string)
        {
            digit_sum += static_cast<int>(digit - '0');
        }

        numeric_string = std::to_string(digit_sum);
    }

    //! Convert final string to int and return
    return std::stoi(numeric_string);

} // static int getLuckyDS1( ...

TEST(GetLuckyTest, SampleTest1)
{
    EXPECT_EQ(36, getLuckyFA("iiii", 1));
    EXPECT_EQ(36, getLuckyDS1("iiii", 1));
}

TEST(GetLuckyTest, SampleTest2)
{
    EXPECT_EQ(6, getLuckyFA("leetcode", 2));
    EXPECT_EQ(6, getLuckyDS1("leetcode", 2));
}

TEST(GetLuckyTest, SampleTest3)
{
    EXPECT_EQ(8, getLuckyFA("zbax", 2));
    EXPECT_EQ(8, getLuckyDS1("zbax", 2));
}

TEST(GetLuckyTest, SampleTest4)
{
    EXPECT_EQ(8, getLuckyFA("fleyctuuajsr", 5));
    EXPECT_EQ(8, getLuckyDS1("fleyctuuajsr", 5));
}
