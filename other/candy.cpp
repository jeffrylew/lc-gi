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
    //!
    //!          Time complexity O(N ^ 2) where N = ratings.size(). We need to
    //!          traverse the vector at most N times. This is because a child
    //!          will get N candies at most, and their candy count will be
    //!          updated once on each traversal.
    //!          Space complexity O(N) for one vector, candies, of size N.

    std::vector<int> candies(ratings.size(), 1);
    const auto       num_children = static_cast<int>(std::ssize(ratings));

    //! If in any traversal, no update of the candies vector occurs, it means
    //! that the vector now contains the final distribution of candies so we
    //! should stop. We use bool has_changed to keep track of whether or not we
    //! made any changes in the last traversal.
    bool has_changed {true};

    while (has_changed)
    {
        has_changed = false;

        for (int child = 0; child < num_children; ++child)
        {
            if (child < num_children - 1
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

//! @brief Using two vectors discussion solution to get min number of candies
//! @param[in] ratings Reference to vector of rating of each child
//! @return Min number of candies required to distribute them to the children
static int candyDS2(const std::vector<int>& ratings)
{
    //! @details https://leetcode.com/problems/candy/editorial/
    //!
    //!          Time complexity O(N) where N = ratings.size().
    //!          The candies_rel_to_left_child and candies_rel_to_right_child
    //!          vectors are traversed thrice.
    //!          Space complexity O(N). The two vectors have size N.

    const auto num_children = static_cast<int>(std::ssize(ratings));

    //! The student with a higher rating than their left neighbor
    //! should always get more candies than their left neighbor
    std::vector<int> candies_rel_to_left_child(ratings.size(), 1);

    //! The student with a higher rating than their right neighbor
    //! should always get more candies than their right neighbor
    std::vector<int> candies_rel_to_right_child(ratings.size(), 1);

    for (int child = 1; child < num_children; ++child)
    {
        if (ratings[child] > ratings[child - 1])
        {
            candies_rel_to_left_child[child] =
                candies_rel_to_left_child[child - 1] + 1;
        }
    }

    for (int child = num_children - 2; child >= 0; --child)
    {
        if (ratings[child] > ratings[child + 1])
        {
            candies_rel_to_right_child[child] =
                candies_rel_to_right_child[child + 1] + 1;
        }
    }

    int min_candies {};
    for (int child = 0; child < num_children; ++child)
    {
        min_candies += std::max(candies_rel_to_left_child[child],
                                candies_rel_to_right_child[child]);
    }

    return min_candies;
}

//! @brief Using one vector discussion solution to get min number of candies
//! @param[in] ratings Reference to vector of rating of each child
//! @return Min number of candies required to distribute them to the children
static int candyDS3(const std::vector<int>& ratings)
{
    //! @details https://leetcode.com/problems/candy/editorial/
    //!
    //!          Time complexity O(N) where N = ratings.size().
    //!          The candies vector of size N is traversed thrice.
    //!          Space complexity O(N) for the candies vector of size N.

    const auto num_children = static_cast<int>(std::ssize(ratings));

    std::vector<int> candies(ratings.size(), 1);

    for (int child = 1; child < num_children; ++child)
    {
        //! Don't need to check if candies[child] <= candies[child - 1]
        //! since this will always be the case
        if (ratings[child] > ratings[child - 1])
        {
            candies[child] = candies[child - 1] + 1;
        }
    }

    int min_candies {candies.back()};
    for (int child = num_children - 2; child >= 0; --child)
    {
        //! When traversing from right to left, we need to update candies[child]
        //! only if candies[child] <= candies[child + 1] since we already
        //! altered the candies vector during the forward traversal.
        if (ratings[child] > ratings[child + 1])
        {
            //! If candies[child] <= candies[child + 1] then
            //! candies[child + 1] + 1 will be bigger than candies[child].
            //! If candies[child] > candies[child + 1] then
            //! we don't need to update candies[child].
            /*
            if (candies[child] <= candies[child + 1])
            {
                candies[child] = candies[child + 1] + 1;
            }
             */

            //! Using std::max captures the above logic
            candies[child] = std::max(candies[child], candies[child + 1] + 1);
        }
        min_candies += candies[child];
    }

    return min_candies;
}

//! @brief Calculate sum of first n natural numbers
//! @param[in] n int representing the number of natural numbers to sum
//! @return Sum of first n natural numbers
[[nodiscard]] constexpr int sum_of_natural_numbers(int n)
{
    return (n * (n + 1)) / 2;
}

//! @brief Single pass with constant space discussion solution
//! @param[in] ratings Reference to vector of rating of each child
//! @return Min number of candies required to distribute them to the children
static int candyDS4(const std::vector<int>& ratings)
{
    //! @details https://leetcode.com/problems/candy/editorial/

    const auto num_children = static_cast<int>(std::ssize(ratings));
    if (num_children <= 1)
    {
        return num_children;
    }

    int total_candies {};
    int uphill_candies {};
    int downhill_candies {};
    int old_slope {};

    for (int child = 1; child < num_children; ++child)
    {
        const int new_slope {
            ratings[child] > ratings[child - 1]
                ? 1
                : (ratings[child] < ratings[child - 1] ? -1 : 0)};

        //! Slope is changing from uphill to flat or downhill
        //! or from downhill to flat or uphill
        if ((old_slope > 0 && new_slope == 0)
            || (old_slope < 0 && new_slope >= 0))
        {
            total_candies += sum_of_natural_numbers(uphill_candies)
                + sum_of_natural_numbers(downhill_candies)
                + std::max(uphill_candies, downhill_candies);

            uphill_candies   = 0;
            downhill_candies = 0;
        }

        //! Slope is uphill
        if (new_slope > 0)
        {
            ++uphill_candies;
        }
        //! Slope is downhill
        else if (new_slope < 0)
        {
            ++downhill_candies;
        }
        //! Slope is flat
        else
        {
            ++total_candies;
        }

        old_slope = new_slope;
    }

    total_candies += 1
        + sum_of_natural_numbers(uphill_candies)
        + sum_of_natural_numbers(downhill_candies)
        + std::max(uphill_candies, downhill_candies);

    return total_candies;
}

TEST(CandyTest, SampleTest1)
{
    const std::vector<int> ratings {1, 0, 2};

    EXPECT_EQ(5, candyFA(ratings));
    EXPECT_EQ(5, candyDS1(ratings));
    EXPECT_EQ(5, candyDS2(ratings));
    EXPECT_EQ(5, candyDS3(ratings));
    EXPECT_EQ(5, candyDS4(ratings));
}

TEST(CandyTest, SampleTest2)
{
    const std::vector<int> ratings {1, 2, 2};

    EXPECT_EQ(4, candyFA(ratings));
    EXPECT_EQ(4, candyDS1(ratings));
    EXPECT_EQ(4, candyDS2(ratings));
    EXPECT_EQ(4, candyDS3(ratings));
    EXPECT_EQ(4, candyDS4(ratings));
}

TEST(CandyTest, SampleTest3)
{
    const std::vector<int> ratings {12, 4, 3, 11, 34, 34, 1, 67};

    EXPECT_EQ(16, candyFA(ratings));
    EXPECT_EQ(16, candyDS1(ratings));

    //!                    ratings: 12  4  3 11 34 34  1 67
    //!  candies_rel_to_left_child:  1  1  1  2  3  1  1  2
    //! candies_rel_to_right_child:  3  2  1  1  1  2  1  1
    //!  max of latter two vectors:  3  2  1  2  3  2  1  2 -> sum = 16
    EXPECT_EQ(16, candyDS2(ratings));

    EXPECT_EQ(16, candyDS3(ratings));
    EXPECT_EQ(16, candyDS4(ratings));
}
