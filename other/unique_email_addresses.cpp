#include <gtest/gtest.h>

#include <algorithm>
#include <iterator>
#include <ranges>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

//! @brief First attempt to get number of different addresses that receive email
//! @param[in] emails Vector of emails containing a local name and domain name
//! @return Number of different addresses that actually receive email
static int numUniqueEmailsFA(const std::vector<std::string>& emails)
{
    //! @details leetcode.com/explore/interview/card/google/67/sql-2/3044
    //!
    //!          Time complexity O(N * C) where N = emails.size() and C is the
    //!          average number of characters per email. We iterate through the
    //!          characters in each email.
    //!          Space complexity O(N * C) for the unique_emails set in the
    //!          worst case where all emails are unique.

    std::unordered_set<std::string> unique_emails;

    for (const auto& email : emails)
    {
        const auto  email_size = static_cast<int>(std::ssize(email));
        int         email_idx {};
        std::string transformed_email;

        while (email_idx < email_size)
        {
            if (email[email_idx] == '@')
            {
                transformed_email.append(email, email_idx);
                unique_emails.insert(std::move(transformed_email));
                break;
            }

            if (email[email_idx] == '+')
            {
                while (email_idx < email_size && email[email_idx] != '@')
                {
                    ++email_idx;
                }
                continue;
            }

            if (email[email_idx] == '.')
            {
                ++email_idx;
                continue;
            }

            transformed_email += email[email_idx];
            ++email_idx;
        }
    }

    return static_cast<int>(std::ssize(unique_emails));
}

//! @brief Linear iteration discussion solution
//! @param[in] emails Vector of emails containing a local name and domain name
//! @return Number of different addresses that actually receive email
static int numUniqueEmailsDS1(const std::vector<std::string>& emails)
{
    //! @details https://leetcode.com/problems/unique-email-addresses/editorial/
    //!
    //!          Time complexity O(N * C) where N = emails.size() and C is the
    //!          average length of an email. In the worst case, we iterate over
    //!          all the characters of each of the emails.
    //!          Space complexity O(N * C). In the worst case when all emails
    //!          are unique, we will store every email in the hash set.

    std::unordered_set<std::string> unique_emails;

    for (const auto& email : emails)
    {
        std::string clean_email;

        for (const char ch : email)
        {
            if (ch == '+' || ch == '@')
            {
                //! Stop adding chars to clean_email
                break;
            }

            if (ch == '.')
            {
                //! Skip this char
                continue;
            }

            clean_email += ch;
        }

        std::string domain_name;
        for (const char ch : email | std::views::reverse)
        {
            domain_name += ch;

            if (ch == '@')
            {
                break;
            }
        }

        std::ranges::reverse(domain_name);

#ifdef __cpp_lib_containers_ranges
        clean_email.append_range(std::move(domain_name));
#else
        clean_email.append(std::make_move_iterator(domain_name.begin()),
                           std::make_move_iterator(domain_name.end()));
#endif

        unique_emails.insert(std::move(clean_email));
    }

    return static_cast<int>(std::ssize(unique_emails));
}

TEST(NumUniqueEmailsTest, Example1)
{
    const std::vector<std::string> emails {
        "test.email+alex@leetcode.com",
        "test.e.mail+bob.cathy@leetcode.com",
        "testemail+david@lee.tcode.com"};

    //! Explanation: testemail@leetcode.com and testemail@lee.tcode.com
    //!              actually receive mail
    EXPECT_EQ(2, numUniqueEmailsFA(emails));
    EXPECT_EQ(2, numUniqueEmailsDS1(emails));
}

TEST(NumUniqueEmailsTest, Example2)
{
    const std::vector<std::string> emails {
        "a@leetcode.com", "b@leetcode.com", "c@leetcode.com"};

    EXPECT_EQ(3, numUniqueEmailsFA(emails));
    EXPECT_EQ(3, numUniqueEmailsDS1(emails));
}
