#include <algorithm>
#include <cctype>
#include <ranges>
#include <string>

//! @brief First attempt to get reformatted license key
//! @param[in] s License key as std::string containing alphanumeric chars+dashes
//! @param[in] k The number of chars in each group, except for the first group
//! @return License key containing N + 1 groups of k characters with N dashes
static std::string licenseKeyFormattingFA(const std::string& s, int k)
{
    //! @details https://leetcode.com/explore/interview/card/google/67/sql-2/472

    const auto to_upper_case = [](char ch) -> char {
        return static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    };

    if (std::ssize(s) == 1L)
    {
        if (s.front() == '-')
        {
            return {};
        }

        s.front() = to_upper_case(s.front());
        return s;
    }

    std::string reformatted_license_key;

    auto reversed_key_chunks =
        s | std::views::reverse
          | std::views::filter([](char c) { return c != '-'; })
          | std::views::transform(to_upper_case)
          | std::views::chunk(k);

    std::ranges::for_each(
        reversed_key_chunks,
        [&](std::ranges::viewable_range auto&& r) {
            for (const char ch : r)
            {
                reformatted_license_key += ch;
            }

            reformatted_license_key += '-';
        });

    if (!reformatted_license_key.empty()
        && reformatted_license_key.back() == '-')
    {
        reformatted_license_key.pop_back();
    }

    std::ranges::reverse(reformatted_license_key);

    return reformatted_license_key;
}
