#include <gtest/gtest.h>

#include <random>
#include <string>
#include <string_view>
#include <utility>
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

    //! Obtain a random seed from the hardward
    std::random_device rd;

    //! Initialize the Mersenne Twister engine with the seed
    std::mt19937 gen(rd());

    std::vector<std::string> candidate_words;

    auto word_list = words;
    int  num_matching_chars {};

    for (int attempt = 0; attempt < 10 && num_matching_chars != 6; ++attempt)
    {
        const auto num_words = static_cast<int>(std::ssize(word_list));

        //! Define the inclusive range
        std::uniform_int_distribution<int> distrib(0, num_words - 1);

        auto      guess_word = word_list[distrib(gen)];
        const int num_matches {master.guess(guess_word)};

        for (auto& word : word_list)
        {
            if (num_matches == get_num_matches(guess_word, word))
            {
                candidate_words.push_back(std::move(word));
            }
        }

        word_list = std::move(candidate_words);
        candidate_words.clear();
    }
}
