#include <gtest/gtest.h>

#include <string>
#include <string_view>
#include <vector>

/*
 * This is the Master's API, do not implement it
 *
class Master
{
public:
    int guess(std::string word);
};
 */

[[nodiscard]] constexpr int
    get_num_matches(std::string_view word1, std::string_view word2)
{
    int num_matches {};

    for (int idx = 0; idx < std::ssize(word1); ++idx)
    {
        if (word1[idx] == word2[idx])
        {
            ++num_matches;
        }
    }

    return num_matches;
}

 //! @brief Guess the word discussion solution
 //! @param[in]      words  Reference to a vector of unique six letter words
 //! @param[in, out] master Reference to helper object Master
static void findSecretWordDS1(const std::vector<std::string>& words,
                              Master&                         master)
{
    //! @details leetcode.com/explore/interview/card/google/66/others-4/3104
}
