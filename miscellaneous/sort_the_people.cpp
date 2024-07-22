#include <gtest/gtest.h>

#include <map>
#include <string>
#include <vector>

//! @brief First attempt to return names sorted in descending order by heights
//! @param[in] names   Vector of names containing lower and upper case letters
//! @param[in] heights Vector of heights consisting of positive distinct ints
//! @return Names sorted in descending order by people's heights
static std::vector<std::string> sortPeopleFA(
    const std::vector<std::string>& names,
    const std::vector<int>&         heights)
{
    //! @details https://leetcode.com/problems/sort-the-people/
    //!
    //!          Time complexity O(N * log N) where N = number of people.
    //!          Space complexity O(N) for height_idx map

    std::map<int, int> height_idx {};

    for (int idx = 0; idx < static_cast<int>(std::ssize(heights)); ++idx)
    {
        height_idx.emplace(heights[idx], idx);
    }

    std::vector<std::string> descending {};
    descending.reserve(names.size());

    for (auto rit = height_idx.rbegin(); rit != height_idx.rend(); ++rit)
    {
        descending.push_back(names[rit->second]);
    }

    return descending;
}

TEST(SortPeopleTest, SampleTest1)
{
    const std::vector<std::string> names {"Mary", "John", "Emma"};
    const std::vector<int>         heights {180, 165, 170};
    const std::vector<std::string> expected_output {"Mary", "Emma", "John"};

    EXPECT_EQ(expected_output, sortPeopleFA(names, heights));
}

TEST(SortPeopleTest, SampleTest2)
{
    const std::vector<std::string> names {"Alice", "Bob", "Bob"};
    const std::vector<int>         heights {155, 185, 150};
    const std::vector<std::string> expected_output {"Bob", "Alice", "Bob"};

    EXPECT_EQ(expected_output, sortPeopleFA(names, heights));
}
