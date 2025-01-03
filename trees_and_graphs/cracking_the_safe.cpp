#include <gtest/gtest.h>

#include <cmath>
#include <functional>
#include <string>
#include <unordered_set>

//! @brief DFS discussion soln to get min length string that unlocks the safe
//! @param[in] n Number of digits in password, 1 <= n <= 4
//! @param[in] k Range of each digit in password, 1 <= k <= 10
//! @return Minimum length string that will unlock the safe at some point
static std::string crackSafeDS(int n, int k)
{
    //! @details https://leetcode.com/problems/cracking-the-safe/description/

    std::string password(n, '0');

    std::unordered_set<std::string> visited_combos {password};

    const auto target_num_visited = static_cast<int>(std::pow(k, n));

    const std::function<bool()> crack_safe_backtrack = [&] {
        //! Base case: All n-length combinations from digits 0 ... k - 1
        //! have been visited
        if (std::ssize(visited_combos) == target_num_visited)
        {
            return true;
        }

        for (int offset = 0; offset < k; ++offset)
        {
            password.push_back('0' + offset);
            const std::string new_combo {password.substr(password.size() - n)};

            if (!visited_combos.contains(new_combo))
            {
                visited_combos.insert(new_combo);
                if (crack_safe_backtrack())
                {
                    return true;
                }
                visited_combos.erase(new_combo);
            }

            password.pop_back();
        }

        return false;
    };

    crack_safe_backtrack();

    return password;

} // static std::string crackSafeDS( ...

TEST(CrackSafeTest, SampleTest1)
{
    EXPECT_EQ("10", crackSafeDS(1, 2));
}

TEST(CrackSafeTest, SampleTest2)
{
    EXPECT_EQ("01100", crackSafeDS(2, 2));
}
