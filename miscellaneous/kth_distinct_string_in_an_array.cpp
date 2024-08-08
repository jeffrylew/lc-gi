#include <gtest/gtest.h>

#include <string>
#include <unordered_map>
#include <unordered_set>
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

//! @brief Brute force discussion solution
//! @param[in] arr Vector of strings
//! @param[in] k   Number of distinct string to retrieve
//! @return kth distinct string in arr or empty string if fewer than k distinct
static std::string kthDistinctDS1(const std::vector<std::string>& arr, int k)
{
    //! @details https://leetcode.com/problems/kth-distinct-string-in-an-array
    //!
    //!          Time complexity O(N ^ 2) where N = arr.size()
    //!          Space complexity O(N) for distinct_strings, which can store up
    //!          to N strings in worst case.

    const auto arr_size = static_cast<int>(std::ssize(arr));

    std:vector<std::string> distinct_strings {};

    //! Iterate through each string in vector
    for (int i = 0; i < arr_size; ++i)
    {
        const auto& current_string = arr[i];
        bool        is_distinct {true};

        //! Check if current string is distinct
        for (int j = 0; j < arr_size; ++j)
        {
            if (j == i)
            {
                //! Skip comparing with itself
                continue;
            }

            if (arr[j] == current_string)
            {
                is_distinct = false;
                break;
            }
        }

        //! If string is distinct, add it to vector
        if (is_distinct)
        {
            distinct_strings.push_back(current_string);
        }
    }

    //! Check if there are enough distinct strings
    if (std::ssize(distinct_strings) < k)
    {
        return {};
    }

    return distinct_strings[k - 1];

} // static std::string kthDistinctDS1( ...

//! @brief Hash set discussion solution
//! @param[in] arr Vector of strings
//! @param[in] k   Number of distinct string to retrieve
//! @return kth distinct string in arr or empty string if fewer than k distinct
static std::string kthDistinctDS2(const std::vector<std::string>& arr, int k)
{
    //! @details https://leetcode.com/problems/kth-distinct-string-in-an-array
    //!
    //!          Time complexity O(N) where N = arr.size(). The algorithm makes
    //!          two passes through arr.
    //!          Space complexity O(N). In the worst case, one of the sets could
    //!          store all N strings in arr (e.g. all strings are distinct).

    std::unordered_set<std::string> distinct_strings;
    std::unordered_set<std::string> duplicate_strings;

    //! First pass: Identify distinct and duplicate strings
    for (const auto& str : arr)
    {
        //! If str already in duplicate_strings, skip further processing
        if (duplicate_strings.contains(str))
        {
            continue;
        }

        //! If str in distinct_strings, means we have seen it before so move it
        //! to duplicate_strings
        if (distinct_strings.contains(str))
        {
            distinct_strings.erase(str);
            duplicate_strings.insert(str);
        }
        else
        {
            distinct_strings.insert(str);
        }
    }

    //! Second pass: Find kth distinct string
    for (const auto& str : arr)
    {
        if (!duplicate_strings.contains(str))
        {
            //! Decrement k for each distinct string encountered
            --k;
        }

        //! When k reaches 0, we have found the kth distinct string
        if (k == 0)
        {
            return str;
        }
    }

    return {};

} // static std::string kthDistinctDS2( ...

TEST(KthDistinctTest, SampleTest1)
{
    const std::vector<std::string> arr {"d", "b", "c", "b", "c", "a"};

    EXPECT_EQ("a", kthDistinctFA(arr, 2));
    EXPECT_EQ("a", kthDistinctDS1(arr, 2));
    EXPECT_EQ("a", kthDistinctDS2(arr, 2));
}

TEST(KthDistinctTest, SampleTest2)
{
    const std::vector<std::string> arr {"aaa", "aa", "a"};

    EXPECT_EQ("aaa", kthDistinctFA(arr, 1));
    EXPECT_EQ("aaa", kthDistinctDS1(arr, 1));
    EXPECT_EQ("aaa", kthDistinctDS2(arr, 1));
}

TEST(KthDistinctTest, SampleTest1)
{
    const std::vector<std::string> arr {"a", "b", "a"};

    EXPECT_TRUE(kthDistinctFA(arr, 3).empty());
    EXPECT_TRUE(kthDistinctDS1(arr, 3).empty());
    EXPECT_TRUE(kthDistinctDS2(arr, 3).empty());
}
