#include <gtest/gtest.h>

#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

//! @brief Recursive helper to find strobogrammatic numbers of length curr_len
//! @param[in, out] snum_map Reference to map to memoize nums at varying lengths
//! @param[in]      curr_len Current length of strobogrammatic numbers to find
//! @param[in]      last_len Final desired length of strobogrammatic number
//! @return Reference to vector of strobogrammatic numbers with length curr_len
[[nodiscard]] static const std::vector<std::string>& find_strobogrammatic_nums(
    std::unordered_map<int, std::vector<std::string>>& snum_map,
    int                                                curr_len,
    int                                                last_len)
{
    if (snum_map.contains(curr_len))
    {
        return snum_map[curr_len];
    }

    //! Get vector of strobogrammatic numbers with length curr_len - 2
    const auto& prev_snums =
        find_strobogrammatic_nums(snum_map, curr_len - 2, last_len);

    //! Reserve space for new strobogrammatic numbers. Multiply by 4 to account
    //! for inserting 1/6/8/9. ssize(prev_snums) = ssize(snum_map[curr_len - 2])
    std::vector<std::string> new_snums;
    new_snums.reserve(4U * prev_snums.size());

    //! New strobogrammatic numbers can begin with zero if curr_len != last_len
    for (const char num_to_insert : {'0', '1', '6', '8', '9'})
    {
        if (curr_len == last_len && num_to_insert == '0')
        {
            continue;
        }

        for (const auto& prev_snum : prev_snums)
        {
            auto& new_snum = new_snums.emplace_back(1U, num_to_insert);
            new_snum += prev_snum;

            switch (num_to_insert)
            {
                case '6':
                    new_snum += '9';
                    break;
                case '9':
                    new_snum += '6';
                    break;
                default:
                    new_snum += num_to_insert;
                    break;
            }
        }
    }

    return snum_map[curr_len] = std::move(new_snums);

} // static const std::vector<std::string>& find_strobogrammatic_nums( ...

//! @brief First attempt to get all strobogrammatic numbers of length n
//! @param[in] n Length of strobogrammatic number
//! @return Vector of strobogrammatic numbers that are of length n
static std::vector<std::string> findStrobogrammaticFA(int n)
{
    //! @details leetcode.com/explore/interview/card/google/62/recursion-4/399
    //!
    //!          Time complexity O(n ^ 3) (?) where n is the input length of the
    //!          strobogrammatic number. find_strobogrammatic_nums is called
    //!          roughly n times due to memoization. During each call of
    //!          find_strobogrammatic_nums, we create 5 * n strings of length n
    //!          in the worst case for a time complexity of roughly O(n ^ 2).
    //!          Space complexity O(n ^ 3) (?). snum_map needs to store all the
    //!          states for n strobogrammatic numbers.

    if (n == 1)
    {
        return {"0", "1", "8"};
    }

    if (n == 2)
    {
        return {"11", "69", "88", "96"};
    }

    //! Map of <strobogrammatic number length, vector of all numbers>
    //! Map memoizes subproblem solutions for n > 2
    std::unordered_map<int, std::vector<std::string>> snum_map {
        {1, {"0", "1", "8"}}, {2, {"00", "11", "69", "88", "96"}}};

    return find_strobogrammatic_nums(snum_map, n, n);

} // static std::vector<std::string> findStrobogrammaticFA( ...

//! @brief Generate strobogrammatic numbers of length curr_len
//! @param[in] curr_len  Current length of strobogrammatic numbers
//! @param[in] final_len Final length of strobogrammatic numbers = n
//! @return Vector of strings containing strobogrammatic numbers with final_len
[[nodiscard]] static std::vector<std::string>
    generate_strobogrammatic_nums(int curr_len, int final_len)
{
    //! @details Recurrence relation is generate_strobogrammatic_nums(N, N) =
    //!          List("first_digit" + "number" + "second_digit"
    //!          for each number in generate_strobogrammatic_nums(N - 2, N)
    //!          for each (first_digit, second_digit) in reversible_pairs)

    if (curr_len == 0)
    {
        //! 0-digit strobogrammatic number is an empty string
        return {""};
    }

    if (curr_len == 1)
    {
        //! 1-digit strobogrammatic numbers
        return {"0", "1", "8"};
    }

    static const std::vector<std::pair<char>> reversible_pairs {
        {'0', '0'}, {'1', '1'}, {'6', '9'}, {'8', '8'}, {'9', '6'}};

    const auto prev_strobo_nums =
        generate_strobogrammatic_nums(curr_len - 2, final_len);

    std::vector<std::string> curr_strobo_nums;

    for (const auto& prev_strobo_num : prev_strobo_nums)
    {
        for (const auto& [first_digit, last_digit] : reversible_pairs)
        {
            //! Only append 0 if it is not the first digit
            if (first_digit != '0' || curr_len != final_len)
            {
                curr_strobo_nums.push_back(
                    first_digit + prev_strobo_num + last_digit);
            }
        }
    }

    return curr_strobo_nums;

} // static std::vector<std::string> generate_strobogrammatic_nums(

//! @brief Recursive discussion solution
//! @param[in] n Length of strobogrammatic number
//! @return Vector of strobogrammatic numbers that are of length n
static std::vector<std::string> findStrobogrammaticDS1(int n)
{
    //! @details leetcode.com/problems/strobogrammatic-number-ii/editorial
    //!
    //!          Time complexity O(N * 5 ^ {[N / 2] + 1}) where N is the length
    //!          of the strobogrammatic numbers. Each recursive call decreases N
    //!          by two so there are N/2 elements in [N, N-2, N-4, N-6, ..., 0].
    //!          In each recursive level, iterate over all prev_strobo_nums and
    //!          append 5 pairs of characters to each so we increase the vector
    //!          curr_strobo_nums by a factor of five with each level. Except
    //!          for the last level, where it increases by a factor of four
    //!          since zero is omitted. When N is even, the total iterations is
    //!          5 + 5^2 + 5^3 + ... + 5^(N/2 - 1) + 4 = 5 ^ (N / 2). When N is
    //!          odd, start with 3 strings and append 5 pairs of chars to each
    //!          string in each level except the last. So the total iterations
    //!          is 3 * 5 + 3 * 5^2 + 3 * 5^3 + ... + 3 * 5^((N - 3)/2) +
    //!          3 * 4 * 5 ^ ((N - 3) / 2) = 5 ^ ((N - 1) / 2 + 1) = 5^(N/2 + 1)
    //!          Space complexity O(N * 5 ^ [N / 2]). We make at most N / 2
    //!          recursive calls so the call stack uses O(N / 2).
    //!          curr_strobo_nums is the output vector so in the last recursive
    //!          call it is not not considered as auxiliary space. However, for
    //!          earlier recursive calls it is. We store at most 5 ^ (N/2 - 1)
    //!          strings when N is even and otherwise 3 * 5 ^ ((N - 1)/2 - 1)
    //!          = 5 ^ [N / 2] when N is odd in curr_strobo_nums and each string
    //!          is of length N - 2. Thus, the overall space used will be
    //!          (N - 2) * 5 ^ [N / 2] + N / 2 = N * 5 ^ [N / 2].

    return generate_strobogrammatic_nums(n, n);
}

//! @brief Iterative (Level Order Traversal) discussion solution
//! @param[in] n Length of strobogrammatic number
//! @return Vector of strobogrammatic numbers that are of length n
static std::vector<std::string> findStrobogrammaticDS2(int n)
{
    //! @details leetcode.com/problems/strobogrammatic-number-ii/editorial
    //!
    //!          Time complexity O(N * 5 ^ ([N / 2] + 1)) where N = length of
    //!          desired strobogrammatic numbers. Start with strings of length 0
    //!          or 1 and process until they are N digits long. At each level,
    //!          increment the string length by 2 so we traverse N / 2 levels.
    //!          In each level, we iterate over all queue elements and append 5
    //!          pairs of chars to each to increase the queue size by around 5x
    //!          with each level. When N is even, start with 1 empty string and
    //!          the total elements traversed using the queue is 5^(N/2 + 1) =
    //!          1 + 5 + 5^2 + 5^3 + ... + 5^(N/2 - 1) + 4*5^(N/2 - 1). When N
    //!          is odd, start with 3 strings so the total elements traversed is
    //!          5 ^ ([N / 2] + 1) = 5 ^ ((N - 1) / 2 + 1) =
    //!          3 + 3*5 + 3*5^2 + 3*5^3 + ... + 5^(N/2 - 1) + 4*5^(N/2 - 1).
    //!          Appending a char at the end of a string takes O(1) but at the
    //!          beginning it costs O(N). Thus, each iteration requires O(N).
    //!          Space complexity O(N * 5^[N/2]). In the last level, we store at
    //!          most 4 * 5 ^ (N / 2 - 1) = 5 ^ (N / 2) string when N is even,
    //!          otherwise 3 * 4 * 5 ^ ((N - 3) / 2) = 5 ^ ((N - 1) / 2) =
    //!          5 ^ [N / 2] strings when N is odd and each has length N.

    const std::vector<std::pair<char, char>> reversible_pairs {
        {'0', '0'}, {'1', '1'}, {'6', '9'}, {'8', '8'}, {'9', '6'}};

    //! Queue for level order traversal, holds strobogrammatic numbers
    std::queue<std::string> strobo_nums {};

    //! Length of strobogrammatic numbers in the current level
    int curr_strobo_nums_len {};

    //! When n is even, decreasing by two results in zero length
    if (n % 2 == 0)
    {
        //! Start with 0-digit strobogrammatic numbers
        curr_strobo_nums_len = 0;
        strobo_nums.emplace();
    }
    else
    {
        //! Start with 1-digit strobogrammatic numbers
        curr_strobo_nums_len = 1;
        strobo_nums.emplace("0");
        strobo_nums.emplace("1");
        strobo_nums.emplace("8");
    }

    while (curr_strobo_nums_len < n)
    {
        curr_strobo_nums_len += 2;
        auto level_size = static_cast<int>(std::ssize(strobo_nums));

        while (level_size > 0)
        {
            const auto strobogrammatic_num = strobo_nums.front();
            strobo_nums.pop();

            for (const auto& [first_digit, last_digit] : reversible_pairs)
            {
                if (curr_strobo_nums_len != n || first_digit != '0')
                {
                    strobo_nums.push(
                        first_digit + strobogrammatic_num + last_digit);
                }
            }

            --level_size;
        }
    }

    std::vector<std::string> strobogrammatic_nums {};
    while (!strobo_nums.empty())
    {
        strobogrammatic_nums.push_back(strobo_nums.front());
        strobo_nums.pop();
    }

    return strobogrammatic_nums;

} // static std::vector<std::string> findStrobogrammaticDS2( ...

TEST(FindStrobogrammaticTest, SampleTest1)
{
    const std::vector<std::string> expected_output {"11", "69", "88", "96"};

    EXPECT_EQ(expected_output, findStrobogrammaticFA(2));
    EXPECT_EQ(expected_output, findStrobogrammaticDS1(2));
    EXPECT_EQ(expected_output, findStrobogrammaticDS2(2));
}

TEST(FindStrobogrammaticTest, SampleTest2)
{
    const std::vector<std::string> expected_output {"0", "1", "8"};

    EXPECT_EQ(expected_output, findStrobogrammaticFA(1));
    EXPECT_EQ(expected_output, findStrobogrammaticDS1(1));
    EXPECT_EQ(expected_output, findStrobogrammaticDS2(1));
}

TEST(FindStrobogrammaticTest, SampleTest3)
{
    const std::vector<std::string> expected_output {
        "100001", "101101", "106901", "108801", "109601", "110011", "111111",
        "116911", "118811", "119611", "160091", "161191", "166991", "168891",
        "169691", "180081", "181181", "186981", "188881", "189681", "190061",
        "191161", "196961", "198861", "199661", "600009", "601109", "606909",
        "608809", "609609", "610019", "611119", "616919", "618819", "619619",
        "660099", "661199", "666999", "668899", "669699", "680089", "681189",
        "686989", "688889", "689689", "690069", "691169", "696969", "698869",
        "699669", "800008", "801108", "806908", "808808", "809608", "810018",
        "811118", "816918", "818818", "819618", "860098", "861198", "866998",
        "868898", "869698", "880088", "881188", "886988", "888888", "889688",
        "890068", "891168", "896968", "898868", "899668", "900006", "901106",
        "906906", "908806", "909606", "910016", "911116", "916916", "918816",
        "919616", "960096", "961196", "966996", "968896", "969696", "980086",
        "981186", "986986", "988886", "989686", "990066", "991166", "996966",
        "998866", "999666"};

    EXPECT_EQ(expected_output, findStrobogrammaticFA(6));
    EXPECT_EQ(expected_output, findStrobogrammaticDS1(6));
    EXPECT_EQ(expected_output, findStrobogrammaticDS2(6));

    /*
    const std::vector<std::string> first_attempt_output {
        "110011", "111111", "116911", "118811", "119611", "160091", "161191",
        "166991", "168891", "169691", "180081", "181181", "186981", "188881",
        "189681", "190061", "191161", "196961", "198861", "199661", "610019",
        "611119", "616919", "618819", "619619", "660099", "661199", "666999",
        "668899", "669699", "680089", "681189", "686989", "688889", "689689",
        "690069", "691169", "696969", "698869", "699669", "810018", "811118",
        "816918", "818818", "819618", "860098", "861198", "866998", "868898",
        "869698", "880088", "881188", "886988", "888888", "889688", "890068",
        "891168", "896968", "898868", "899668", "910016", "911116", "916916",
        "918816", "919616", "960096", "961196", "966996", "968896", "969696",
        "980086", "981186", "986986", "988886", "989686", "990066", "991166",
        "996966", "998866", "999666"};

    EXPECT_EQ(first_attempt_output, findStrobogrammaticFA(6));
     */
}
