#include <gtest/gtest.h>

#include <vector>

//! @brief Get min number of candies needed to distribute them to the children
//! @param[in] ratings Reference to vector of rating of each child
//! @return Min number of candies required to distribute them to the children
static int candyFA(const std::vector<int>& ratings)
{
    //! @details leetcode.com/explore/featured/card/google/66/others-4/3097
}

TEST(CandyTest, SampleTest1)
{
    const std::vector<int> ratings {1, 0, 2};

    EXPECT_EQ(5, candyFA(ratings));
}

TEST(CandyTest, SampleTest2)
{
    const std::vector<int> ratings {1, 2, 2};

    EXPECT_EQ(4, candyFA(ratings));
}
