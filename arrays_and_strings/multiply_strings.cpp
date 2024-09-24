#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <string_view>

//! @brief Return product of num1 and num2 as a string
//! @param[in] num1 std::string containing first non-negative integer
//! @param[in] num2 std::string containing second non-negative integer
//! @return Product of num1 and num2 as a std::string
static std::string multiplyFA(std::string num1, std::string num2)
{
    if (num1 == "0" || num2 == "0")
    {
        return "0";
    }

    //! @brief Multiply num with digit and append num_zeros zeros
    const auto multiply = [](std::string_view num, char digit, int num_zeros) {
        std::string out(num.size() + 1U + num_zeros, '0');

        const auto num_size  = static_cast<int>(std::ssize(num));
        const auto int_digit = static_cast<int>(digit - '0');

        int out_idx {num_size};
        int carryover {};

        for (int num_idx = num_size - 1; num_idx > -1; --num_idx)
        {
            int prod {static_cast<int>(num[num_idx] - '0') * int_digit};
            prod += carryover;

            out[out_idx--] = static_cast<char>(prod % 10);
            carryover      = prod / 10;
        }

        if (carryover > 0)
        {
            out.front() = static_cast<char>(carryover);
        }

        return out.front() == '0' ? out.erase(0, 1) : out;
    };

    //! @brief Add two products
    const auto add = [](const std::string& prod1, const std::string& prod2) {
        std::string out(std::max(prod1.size(), prod2.size()) + 1U, '\0');

        const auto prod1_size = static_cast<int>(std::ssize(prod1));
        const auto prod2_size = static_cast<int>(std::ssize(prod2));
        int        idx1 {prod1_size - 1};
        int        idx2 {prod2_size - 1};

        auto out_idx {static_cast<int>(std::ssize(out)) - 1};
        int  carryover {};

        while (idx1 > -1 && idx2 > -1)
        {
            const auto digit1 = static_cast<int>(prod1[idx1--] - '0');
            const auto digit2 = static_cast<int>(prod2[idx2--] - '0');
            const int  sum {digit1 + digit2 + carryover};

            out[out_idx--] = static_cast<char>(sum % 10);
            carryover      = sum / 10;
        }

        while (idx1 > -1)
        {
            const auto digit1 = static_cast<int>(prod1[idx1--] - '0');
            const int  sum {digit1 + carryover};

            out[out_idx--] = static_cast<char>(sum % 10);
            carryover      = sum / 10;
        }

        while (idx2 > -1)
        {
            const auto digit2 = static_cast<int>(prod2[idx2--] - '0');
            const int  sum {digit2 + carryover};

            out[out_idx--] = static_cast<char>(sum % 10);
            carryover      = sum / 10;
        }

        if (carryover > 0)
        {
            out.front() = static_cast<char>(carryover);
        }

        return out.front() == '0' ? out.erase(0, 1) : out;
    };

    std::string product {};
    const auto  num1_size = static_cast<int>(std::ssize(num1));
    const auto  num2_size = static_cast<int>(std::ssize(num2));

    if (num1_size <= num2_size)
    {
        for (int idx1 = num1_size - 1; idx1 > -1; --idx1)
        {
            product =
                add(product, multiply(num2, num1[idx1], num1_size - idx1 - 1));
        }
    }
    else
    {
        for (int idx2 = num2_size - 1; idx2 > -1; --idx2)
        {
            product =
                add(product, multiply(num1, num2[idx2], num2_size - idx2 - 1));
        }
    }

    return product;

} // static std::string multiplyFA( ...

TEST(MultiplyTest, SampleTest1)
{
    EXPECT_EQ("6", multiplyFA("2", "3"));
}

TEST(MultiplyTest, SampleTest2)
{
    EXPECT_EQ("56088", multiplyFA("123", "456"));
}
