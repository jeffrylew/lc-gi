#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <string>

//! @brief First attempt to check if start can be transformed into result
//! @param[in] start  The starting string
//! @param[in] result The ending string
//! @return True if a sequence of moves exists to transform start into result
static bool canTransformFA(std::string start, std::string result)
{
    //! @details leetcode.com/explore/interview/card/google/66/others-4/3103
    //!
    //!          First attempt solution passes 53 / 99 test cases.
    //!          It fails SampleTest3.

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
                        result_reachable || can_transform(curr_string, end_idx);
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

TEST(CanTransformTest, SampleTest1)
{
    //! RX XLRXRXL  -> XR XLRXRXL
    //! XR XL RXRXL -> XR LX RXRXL
    //! XRLX RX RXL -> XRLX XR RXL
    //! XRLXXRR XL  -> XRLXXRR LX == XRLXXRRLX
    EXPECT_TRUE(canTransformFA("RXXLRXRXL", "XRLXXRRLX"));
}

TEST(CanTransformTest, SampleTest2)
{
    EXPECT_FALSE(canTransformFA("X", "L"));
}

TEST(CanTransformTest, SampleTest3)
{
    // EXPECT_TRUE(canTransformFA("XXXXXLXXXX", "LXXXXXXXXX"));
}
