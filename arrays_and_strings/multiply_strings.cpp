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
    //!
    //!          Time complexity O(M ^ 2 + M * N) where N = num1.size() and
    //!          M = num2.size(). During multiplication, we perform N operations
    //!          for each of the M digits of the second number in O(M * N). Then
    //!          we add each of the M multiplication results of length O(N + M)
    //!          to the answer string, requiring O(M * (M + N)) time. When
    //!          multiplying a number with one digit, the max length can be one
    //!          more than the number's length. There can be at most M - 1 zeros
    //!          appended to the result. Hence, each result is O(N + M).
    //!          Summing the results requires iterating over the length of the
    //!          current answer for each result. The length of two numbers
    //!          multiplied together cannot be longer than sum of the lengths of
    //!          two numbers so iterating over each digit takes O(M + N) time.
    //!          Do this M - 1 times so this step takes O(M * (M + N)). Finally,
    //!          reversing the answer takes O(M + N).
    //!          Space complexity O(M ^ 2 + M * N). Store each multiplication
    //!          result in the results vector. Each result can have N + M length
    //!          max and there are M results.

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

//! @brief Elementary math using less space discussion solution
//! @param[in] num1 std::string containing first non-negative integer
//! @param[in] num2 std::string containing second non-negative integer
//! @return Product of num1 and num2 as a std::string
static std::string multiplyDS2(std::string num1, std::string num2)
{
    //! @details https://leetcode.com/problems/multiply-strings/editorial/
    //!
    //!          Time complexity O(M ^ 2 + M * N) where N = num1.size() and
    //!          M = num2.size(). Multiplication performs N operations for each
    //!          of the M digits of num2 and requires O(M * N). We add the
    //!          multiplication result to the ans vector that has length N + M.
    //!          There are M additions for each digit in num2 for O(M * (N + M))
    //!          Space complexity O(N + M). The answer string and multiplication
    //!          results will have at most N + M length.

    if (num1 == "0" || num2 == "0")
    {
        return "0";
    }

    //! Reverse both numbers
    std::reverse(num1.begin(), num1.end());
    std::reverse(num2.begin(), num2.end());

    const auto num1_size = static_cast<int>(std::ssize(num1));
    const auto num2_size = static_cast<int>(std::ssize(num2));

    //! Vector stores multiplication result of each digit of num2 with num1
    std::vector<int> ans(num1_size + num2_size, 0);

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

    //! Add num to ans vector
    const auto add_to_ans = [&](std::vector<int> num) {
        int carry {};

        for (int ans_idx = 0; ans_idx < std::ssize(ans); ++ans_idx)
        {
            //! If num is shorter than ans, use 0 as the current digit
            const int num_digit {ans_idx < std::ssize(num) ? num[ans_idx] : 0};

            //! Add digits of ans and num
            const int sum {ans[ans_idx] + num_digit + carry};

            //! Set carry equal to the tens place digit of sum
            carry = sum / 10;

            //! Set digit in ans to ones place digit of sum
            ans[ans_idx] = sum % 10;
        }
    };

    //! For each digit in num2, multiply the digit by num1
    //! and add multiplication result to ans
    for (int num2_idx = 0; num2_idx < num2_size; ++num2_idx)
    {
        //! Add current result to final ans
        add_to_ans(multiply_one_digit(num1, num2[num2_idx], num2_idx));
    }

    //! Pop the excess 0 from the back of ans
    if (ans.back() == 0)
    {
        ans.pop_back();
    }

    //! ans is in the reversed order
    //! Copy it in the reverse order
    std::string answer;
    for (int idx = std::ssize(ans) - 1; idx > -1; --idx)
    {
        answer.push_back(ans[idx] + '0');
    }

    return answer;

} // static std::string multiplyDS2( ...

//! @brief Sum products from all pairs of digits discussion solution
//! @param[in] num1 std::string containing first non-negative integer
//! @param[in] num2 std::string containing second non-negative integer
//! @return Product of num1 and num2 as a std::string
static std::string multiplyDS3(std::string num1, std::string num2)
{
    //! @details https://leetcode.com/problems/multiply-strings/editorial/
    //!
    //!          Time complexity O(M * N) where N = num1.size() and
    //!          M = num2.size(). During multiplication we perform N operations
    //!          for each of the M digits of num2.
    //!          Space complexity O(1), space used to store the output is not
    //!          included. Note strings are immutable in Python, Java, and
    //!          JavaScript so a temporary data structure using O(M + N) space
    //!          is required to store the answer while it is updated.

    if (num1 == "0" || num2 == "0")
    {
        return "0";
    }

    //! Reverse num1 and num2
    std::reverse(num1.begin(), num1.end());
    std::reverse(num2.begin(), num2.end());

    //! Initialize answer as a string of zeros of length ans_size
    const auto  num1_size = static_cast<int>(std::ssize(num1));
    const auto  num2_size = static_cast<int>(std::ssize(num2));
    const auto  ans_size  = num1_size + num2_size;
    std::string answer(ans_size, '0');

    for (int place2 = 0; place2 < num2_size; ++place2)
    {
        const auto digit2 = static_cast<int>(num2[place2] - '0');

        //! For each digit in num2, multiply it by all digits in num1
        for (int place1 = 0; place1 < num1_size; ++place1)
        {
            const auto digit1 = static_cast<int>(num1[place1] - '0');

            //! The number of zeros depends on the place of digit2
            //! in num2 and the place of digit1 in num1
            const int num_zeros {place1 + place2};

            //! The digit currently at position num_zeros in the answer
            //! string is carried over and summed with the current result
            const auto carry = static_cast<int>(answer[num_zeros] - '0');
            const int  multiplication {digit1 * digit2 + carry};

            //! Set the ones place of the multiplication result
            answer[num_zeros] = (multiplication % 10) + '0';

            //! Carry the tens place of the multiplication result by
            //! adding it to the next position in the answer string
            answer[num_zeros + 1] += (multiplication / 10);

        } // for (int place1 = 0; ...

    } // for (int place2 = 0; ...

    if (answer.back() == '0')
    {
        answer.pop_back();
    }

    std::reverse(answer.begin(), answer.end());
    return answer;

} // static std::string multiplyDS3( ...

TEST(MultiplyTest, SampleTest1)
{
    EXPECT_EQ("6", multiplyFA("2", "3"));
    EXPECT_EQ("6", multiplyDS1("2", "3"));
    EXPECT_EQ("6", multiplyDS2("2", "3"));
    EXPECT_EQ("6", multiplyDS3("2", "3"));
}

TEST(MultiplyTest, SampleTest2)
{
    EXPECT_EQ("56088", multiplyFA("123", "456"));
    EXPECT_EQ("56088", multiplyDS1("123", "456"));
    EXPECT_EQ("56088", multiplyDS2("123", "456"));
    EXPECT_EQ("56088", multiplyDS3("123", "456"));
}
