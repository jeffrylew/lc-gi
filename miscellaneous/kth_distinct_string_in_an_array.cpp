#include <gtest/gtest.h>

#include <string>
#include <unordered_map>
#include <vector>

//! @brief First attempt to get kth distinct string in arr
//! @param[in] arr Vector of strings
//! @param[in] k   Number of distinct string to retrieve
//! @return kth distinct string in arr or empty string if fewer than k distinct
static std::string kthDistinctFA(const std::vector<std::string>& arr, int k)
{
    //! @details https://leetcode.com/problems/kth-distinct-string-in-an-array
    //!
    //!          Time complexity O(N) where N = arr.size()
    //!          Space complexity O(N)

    std::unordered_map<std::string, int> str_count;

    for (const auto& str : arr)
    {
        ++str_count[str];
    }

    for (const auto& str : arr)
    {
        if (str_count[str] == 1 && --k == 0)
        {
            return str;
        }
    }

    return {};
}

TEST(KthDistinctTest, SampleTest1)
{
    const std::vector<std::string> arr {"d", "b", "c", "b", "c", "a"};

    EXPECT_EQ("a", kthDistinctFA(arr, 2));
}

TEST(KthDistinctTest, SampleTest2)
{
    const std::vector<std::string> arr {"aaa", "aa", "a"};

    EXPECT_EQ("aaa", kthDistinctFA(arr, 1));
}

TEST(KthDistinctTest, SampleTest1)
{
    const std::vector<std::string> arr {"a", "b", "a"};

    EXPECT_TRUE(kthDistinctFA(arr, 3).empty());
}
