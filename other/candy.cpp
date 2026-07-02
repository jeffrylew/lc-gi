#include <gtest/gtest.h>

#include <numeric>
#include <vector>

//! @brief Get min number of candies needed to distribute them to the children
//! @param[in] ratings Reference to vector of rating of each child
//! @return Min number of candies required to distribute them to the children
static int candyFA(const std::vector<int>& ratings)
{
    //! @details leetcode.com/explore/featured/card/google/66/others-4/3097

    const auto ratings_size = static_cast<int>(std::ssize(ratings));
    if (ratings_size == 1)
    {
        return 1;
    }

    int              min_rating {20001};
    std::vector<int> min_rating_idxs;

    for (int idx = 0; idx < ratings_size; ++idx)
    {
        const int curr_rating {ratings[idx]};

        if (curr_rating < min_rating)
        {
            min_rating = curr_rating;
            min_rating_idxs.clear();
            min_rating_idxs.push_back(idx);
        }
        else if (curr_rating == min_rating)
        {
            min_rating_idxs.push_back(idx);
        }
    }
    const auto min_rating_idxs_size =
        static_cast<int>(std::ssize(min_rating_idxs));

    int min_candies {std::accumulate(min_rating_idxs.begin(),
                                     min_rating_idxs.end(),
                                     0)};

    int curr_candies {1};
    for (int ridx = min_rating_idxs[0] - 1; ridx > 0; --ridx)
    {
        if (ratings[ridx] > ratings[ridx + 1])
        {
            ++curr_candies;
            min_candies += curr_candies;
        }
        else if (ratings[ridx] == ratings[ridx + 1])
        {
            if (ratings[ridx] > ratings[ridx - 1])
            {
                ++curr_candies;
            }
            else if (ratings[ridx] == ratings[ridx - 1])
            {
                curr_candies = 1;
            }
            min_candies += curr_candies;
        }
        else
        {
            //! ratings[ridx] < ratings[ridx + 1]

            if (ratings[ridx] > ratings[ridx - 1])
            {
                //! @todo
            }
            else if (ratings[ridx] == ratings[ridx - 1])
            {
                //! @todo
            }
            min_candies += curr_candies;
        }
    }

    if (min_rating_idxs_size > 1)
    {
        for (int lidx = min_rating_idxs.back();
             lidx < min_rating_idxs_size;
             ++lidx)
        {
            //! @todo
        }
    }

    //! @todo

    return min_candies;
}

//! @brief Brute force discussion solution to get min number of candies
//! @param[in] ratings Reference to vector of rating of each child
//! @return Min number of candies required to distribute them to the children
static int candyDS1(const std::vector<int>& ratings)
{
    //! @details https://leetcode.com/problems/candy/editorial/

    std::vector<int> candies(ratings.size(), 1);
    const auto       num_ratings = static_cast<int>(std::ssize(ratings));

    bool has_changed {true};

    while (has_changed)
    {
        has_changed = false;

        for (int child = 0; child < num_ratings; ++child)
        {
            if (child != num_ratings - 1
                && ratings[child] > ratings[child + 1]
                && candies[child] <= candies[child + 1])
            {
                candies[child] = candies[child + 1] + 1;
                has_changed = true;
            }

            if (child > 0
                && ratings[child] > ratings[child - 1]
                && candies[child] <= candies[child - 1])
            {
                candies[child] = candies[child - 1] + 1;
                has_changed = true;
            }
        }
    }

    return std::accumulate(candies.begin(), candies.end(), 0);
}

TEST(CandyTest, SampleTest1)
{
    const std::vector<int> ratings {1, 0, 2};

    EXPECT_EQ(5, candyFA(ratings));
    EXPECT_EQ(5, candyDS1(ratings));
}

TEST(CandyTest, SampleTest2)
{
    const std::vector<int> ratings {1, 2, 2};

    EXPECT_EQ(4, candyFA(ratings));
    EXPECT_EQ(4, candyDS1(ratings));
}
