#include <gtest/gtest.h>

#include <array>
#include <format>
#include <string>

//! @brief First attempt to get the number of bulls and cows in the guess
//! @param[in] secret std::string containing the secret number
//! @param[in] guess  std::string containing the guess of the secret number
//! @return A hint in the format xAyB where x = num bulls and y = num cows
static std::string getHintFA(const std::string& secret,
                             const std::string& guess)
{
    //! @details leetcode.com/explore/interview/card/google/66/sql-2/3100

    int        num_bulls {};
    int        num_cows {};
    const auto num_digits = static_cast<int>(std::ssize(secret));

    //! Count non-bull digits
    std::array<int, 10> secret_digit_count {};
    std::array<int, 10> guess_digit_count {};

    int idx {};
    while (idx < num_digits)
    {
        if (secret[idx] == guess[idx])
        {
            ++num_bulls;
        }
        else
        {
            const auto secret_digit = static_cast<int>(secret[idx] - '0');
            const auto guess_digit  = static_cast<int>(guess[idx] - '0');

            ++secret_digit_count[secret_digit];
            ++guess_digit_count[guess_digit];
        }

        ++idx;
    }

    for (idx = 0; idx < 10; ++idx)
    {
        num_cows += std::min(secret_digit_count[idx], guess_digit_count[idx]);
    }

    return std::format("{}A{}B", num_bulls, num_cows);
}

TEST(BullsAndCowsTest, SampleTest1)
{
    const std::string secret {"1807"};
    const std::string guess {"7810"};
    const std::string expected_output {"1A3B"};

    EXPECT_EQ(expected_output, getHintFA(secret, guess));
}

TEST(BullsAndCowsTest, SampleTest2)
{
    const std::string secret {"1123"};
    const std::string guess {"0111"};
    const std::string expected_output {"1A1B"};

    EXPECT_EQ(expected_output, getHintFA(secret, guess));
}
