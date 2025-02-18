#include <gtest/gtest.h>

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

[[nodiscard]] static std::vector<std::string>
    generate_strobogrammatic_nums(int curr_len, int final_len)
{
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

    return generate_strobogrammatic_nums(n, n);
}

TEST(FindStrobogrammaticTest, SampleTest1)
{
    const std::vector<std::string> expected_output {"11", "69", "88", "96"};

    EXPECT_EQ(expected_output, findStrobogrammaticFA(2));
    EXPECT_EQ(expected_output, findStrobogrammaticDS1(2));
}

TEST(FindStrobogrammaticTest, SampleTest2)
{
    const std::vector<std::string> expected_output {"0", "1", "8"};

    EXPECT_EQ(expected_output, findStrobogrammaticFA(1));
    EXPECT_EQ(expected_output, findStrobogrammaticDS1(1));
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
