#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <iterator>
#include <string>
#include <vector>

//! @brief First attempt to check if start can be transformed into result
//! @param[in] start  The starting string
//! @param[in] result The ending string
//! @return True if a sequence of moves exists to transform start into result
static bool canTransformFA(std::string start, std::string result)
{
    //! @details leetcode.com/explore/interview/card/google/66/others-4/3103
    //!
    //!          First attempt solution passes 67 / 99 test cases.
    //!          It has Time Limit Exceeded for SampleTest4.

    const auto start_size  = static_cast<int>(std::ssize(start));
    const auto result_size = static_cast<int>(std::ssize(result));

    if (start_size < 2 || start_size != result_size)
    {
        return false;
    }

    const std::function<bool(std::string, int)> can_transform =
        [&](std::string curr_string, int curr_end_index) {
            if (curr_string == result)
            {
                return true;
            }

            bool result_reachable {};

            for (int end_idx = curr_end_index; end_idx < start_size; ++end_idx)
            {
                if (result_reachable)
                {
                    break;
                }

                auto& begin_char = curr_string[end_idx - 1];
                auto& end_char   = curr_string[end_idx];

                if ((begin_char == 'X' && end_char == 'L')
                    || (begin_char == 'R' && end_char == 'X'))
                {
                    std::swap(begin_char, end_char);
                    result_reachable =
                        result_reachable
                        || can_transform(curr_string, std::max(1, end_idx - 1));
                    std::swap(begin_char, end_char);
                }
            }

            return result_reachable;
        };

    bool sequence_exists {};
    for (int end_index = 1; end_index < start_size; ++end_index)
    {
        if (sequence_exists)
        {
            break;
        }

        sequence_exists = sequence_exists || can_transform(start, end_index);
    }

    return sequence_exists;
}

//! @brief Invariant discussion solution
//! @param[in] start  The starting string
//! @param[in] result The ending string
//! @return True if a sequence of moves exists to transform start into result
static bool canTransformDS1(std::string start, std::string result)
{
    //! @details leetcode.com/problems/swap-adjacent-in-lr-string/editorial
    //!
    //!          Time complexity O(N) where N = the length of start and result.
    //!          Space complexity O(N). std::ranges::remove_copy uses O(N) for
    //!          the two vectors, while the remaining operations use O(1) space.

    std::vector<char> start_without_x;
    std::vector<char> result_without_x;
    std::ranges::remove_copy(start, std::back_inserter(start_without_x), 'X');
    std::ranges::remove_copy(result, std::back_inserter(result_without_x), 'X');

    if (start_without_x != result_without_x)
    {
        return false;
    }

    const auto start_size = static_cast<int>(std::ssize(start));
    int        result_idx {};

    for (int start_idx = 0; start_idx < start_size; ++start_idx)
    {
        if (start[start_idx] == 'L')
        {
            //! Move result_idx to the right. Due to "accessbility", the nth 'L'
            //! cannot be to the right of its original position since we can
            //! replace "XL" with "LX"
            while (result[result_idx] != 'L')
            {
                ++result_idx;
            }

            //! result_idx is now at a position in result that has an 'L'
            //! It cannot be to the right of start_idx
            if (start_idx < result_idx)
            {
                return false;
            }

            //! result_idx is either to the left of or at start_idx
            //! Increment it to start the search for the next 'L'
            ++result_idx;
        }
    }

    result_idx = 0;
    for (int start_idx = 0; start_idx < start_size; ++start_idx)
    {
        //! Move result_idx to the right. Due to "accessibility", the nth 'R'
        //! cannot be to the left of its original position since we can replace
        //! "RX" with "XR"
        if (start[start_idx] == 'R')
        {
            while (result[result_idx] != 'R')
            {
                ++result_idx;
            }

            //! result_idx is now at a position in result that has an 'R'
            //! It cannot be to the left of start_idx
            if (start_idx > result_idx)
            {
                return false;
            }

            //! result_idx is either to the right of or at start_idx
            //! Increment it to start the search for the next 'R'
            ++result_idx;
        }
    }

    return true;
}

//! @brief Two pointers discussion solution
//! @param[in] start  The starting string
//! @param[in] result The ending string
//! @return True if a sequence of moves exists to transform start into result
static bool canTransformDS2(std::string start, std::string result)
{
    //! @details leetcode.com/problems/swap-adjacent-in-lr-string/editorial
    //!
    //!          Time complexity O(N) where N = the length of start and result.
    //!          Space complexity O(1)

    const auto string_size = static_cast<int>(std::ssize(start));

    //! Count of 'X' in start and result should be the same
    int num_x {};
    for (int idx = 0; idx < string_size; ++idx)
    {
        if (start[idx] == 'X')
        {
            ++num_x;
        }

        if (result[idx] == 'X')
        {
            --num_x;
        }
    }

    if (num_x != 0)
    {
        return false;
    }

    int start_idx {};
    int result_idx {};

    while (start_idx < string_size && result_idx < string_size)
    {
        while (start_idx < string_size && start[start_idx] == 'X')
        {
            ++start_idx;
        }

        while (result_idx < string_size && result[result_idx] == 'X')
        {
            ++result_idx;
        }

        //! start_idx and result_idx are the indices of the next non-'X' chars
        if (start_idx == string_size || result_idx == string_size)
        {
            return start_idx == string_size && result_idx == string_size;
        }

        if (start[start_idx] != result[result_idx])
        {
            return false;
        }

        //! The nth 'L' in result cannot be right of its orig position in start
        //! when replacing "XL" with "LX"
        if (start[start_idx] == 'L' && start_idx < result_idx)
        {
            return false;
        }

        //! The nth 'R' in result cannot be left of its orig position in start
        //! when replacing "RX" with "XR"
        if (start[start_idx] == 'R' && start_idx > result_idx)
        {
            return false;
        }

        ++start_idx;
        ++result_idx;
    }

    return true;
}

TEST(CanTransformTest, SampleTest1)
{
    //! RX XLRXRXL  -> XR XLRXRXL
    //! XR XL RXRXL -> XR LX RXRXL
    //! XRLX RX RXL -> XRLX XR RXL
    //! XRLXXRR XL  -> XRLXXRR LX == XRLXXRRLX
    EXPECT_TRUE(canTransformFA("RXXLRXRXL", "XRLXXRRLX"));
    EXPECT_TRUE(canTransformDS1("RXXLRXRXL", "XRLXXRRLX"));
    EXPECT_TRUE(canTransformDS2("RXXLRXRXL", "XRLXXRRLX"));
}

TEST(CanTransformTest, SampleTest2)
{
    EXPECT_FALSE(canTransformFA("X", "L"));
    EXPECT_FALSE(canTransformDS1("X", "L"));
    EXPECT_FALSE(canTransformDS2("X", "L"));
}

TEST(CanTransformTest, SampleTest3)
{
    EXPECT_TRUE(canTransformFA("XXXXXLXXXX", "LXXXXXXXXX"));
    EXPECT_TRUE(canTransformDS1("XXXXXLXXXX", "LXXXXXXXXX"));
    EXPECT_TRUE(canTransformDS2("XXXXXLXXXX", "LXXXXXXXXX"));
}

TEST(CanTransformTest, SampleTest4)
{
    EXPECT_TRUE(
        canTransformFA("XXXXXXRXXLXRXXXXXRXXXXXRXXXXXLXXXLXLXXRXXXXXLXXXXX",
                       "XXRXXXXLXXRXXXRXXXXRXXXXXLXXLXXXXXXLXXXXRXXXXLXXXX"));
    EXPECT_TRUE(
        canTransformDS1("XXXXXXRXXLXRXXXXXRXXXXXRXXXXXLXXXLXLXXRXXXXXLXXXXX",
                        "XXRXXXXLXXRXXXRXXXXRXXXXXLXXLXXXXXXLXXXXRXXXXLXXXX"));
    EXPECT_TRUE(
        canTransformDS2("XXXXXXRXXLXRXXXXXRXXXXXRXXXXXLXXXLXLXXRXXXXXLXXXXX",
                        "XXRXXXXLXXRXXXRXXXXRXXXXXLXXLXXXXXXLXXXXRXXXXLXXXX"));
}
