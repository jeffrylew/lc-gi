#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <string_view>
#include <vector>

//! @brief Return product of num1 and num2 as a string
//! @param[in] num1 std::string containing first non-negative integer
//! @param[in] num2 std::string containing second non-negative integer
//! @return Product of num1 and num2 as a std::string
static std::string multiplyFA(std::string num1, std::string num2)
{
    //! @details https://leetcode.com/problems/multiply-strings/description/
    //!
    //!          Time complexity O(M * N) where M = num1.size(), N = num2.size()
    //!          Space complexity O(max(M, N))

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

            out[out_idx--] = static_cast<char>('0' + prod % 10);
            carryover      = prod / 10;
        }

        if (carryover > 0)
        {
            out.front() = static_cast<char>('0' + carryover);
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

            out[out_idx--] = static_cast<char>('0' + sum % 10);
            carryover      = sum / 10;
        }

        while (idx1 > -1)
        {
            const auto digit1 = static_cast<int>(prod1[idx1--] - '0');
            const int  sum {digit1 + carryover};

            out[out_idx--] = static_cast<char>('0' + sum % 10);
            carryover      = sum / 10;
        }

        while (idx2 > -1)
        {
            const auto digit2 = static_cast<int>(prod2[idx2--] - '0');
            const int  sum {digit2 + carryover};

            out[out_idx--] = static_cast<char>('0' + sum % 10);
            carryover      = sum / 10;
        }

        if (carryover > 0)
        {
            out.front() = static_cast<char>('0' + carryover);
        }

        return out.front() == '\0' ? out.erase(0, 1) : out;
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

//! @brief Elementary math discussion solution
//! @param[in] num1 std::string containing first non-negative integer
//! @param[in] num2 std::string containing second non-negative integer
//! @return Product of num1 and num2 as a std::string
static std::string multiplyDS1(std::string num1, std::string num2)
{
    //! @details https://leetcode.com/problems/multiply-strings/editorial/

    if (num1 == "0" || num2 == "0")
    {
        return "0";
    }

    //! Reverse both numbers
    std::reverse(num1.begin(), num1.end());
    std::reverse(num2.begin(), num2.end());

    //! Multiply current digit of num2 with num1
    const auto multiply_one_digit =
        [](std::string_view first_num, char second_num_digit, int num_zeros) {
            //! Insert zeros at the beginning based on current digit's place
            std::vector<int> current_result(num_zeros, 0);

            int carry {};

            //! Multiply num1 with the current digit of num2
            for (const char first_num_digit : first_num)
            {
                const int multiplication {
                    (second_num_digit - '0') * (first_num_digit - '0') + carry};

                //! Set carry equal to the tens place digit of multiplication
                carry = multiplication / 10;

                //! Append last digit to the current result
                current_result.push_back(multiplication % 10);
            }

            if (carry > 0)
            {
                current_result.push_back(carry);
            }

            return current_result;
        };

    //! For each digit in num2, multiply the digit by num1 and
    //! store the multiplication result (reversed) in results
    std::vector<std::vector<int>> results {};
    for (int idx = 0; idx < std::ssize(num2); ++idx)
    {
        results.push_back(multiply_one_digit(num1, num2[idx], idx));
    }

    //! Calculate sum of all results from multiply_one_digit
    const auto sum_results = [&results]() {
        //! Initialize answer as a number from results
        auto             answer = results.back();
        std::vector<int> new_answer {};
        results.pop_back();

        //! Sum each digit from answer and result
        for (const auto& result : results)
        {
            new_answer.clear();
            int carry = 0;

            for (int idx = 0;
                 idx < std::ssize(answer) || idx < std::ssize(result);
                 ++idx)
            {
                //! If answer is shorter than result or vice versa,
                //! use 0 as the current digit
                const int digit1 {idx < std::ssize(result) ? result[idx] : 0};
                const int digit2 {idx < std::ssize(answer) ? answer[idx] : 0};

                //! Add current digits of both numbers
                const int sum {digit1 + digit2 + carry};

                //! Set carry equal to the tens place digit of sum
                carry = sum / 10;

                //! Append the ones place digit of sum to answer
                new_answer.push_back(sum % 10);
            }

            if (carry > 0)
            {
                new_answer.push_back(carry);
            }
            answer = new_answer;
        }

        //! Convert answer to a string
        std::string final_answer {};
        for (const int digit : answer)
        {
            final_answer.push_back(digit + '0');
        }

        return final_answer;
    };

    //! Add all results in results vector and store sum in answer string
    auto answer = sum_results();

    //! Reverse answer to get final answer
    std::reverse(answer.begin(), answer.end());
    return answer;

} // static std::string multiplyDS1( ...

TEST(MultiplyTest, SampleTest1)
{
    EXPECT_EQ("6", multiplyFA("2", "3"));
    EXPECT_EQ("6", multiplyDS1("2", "3"));
}

TEST(MultiplyTest, SampleTest2)
{
    EXPECT_EQ("56088", multiplyFA("123", "456"));
    EXPECT_EQ("56088", multiplyDS1("123", "456"));
}
