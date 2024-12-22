#include <gtest/gtest.h>

#include <string>
#include <unordered_map>
#include <vector>

//! @brief First attempt to get answers to all queries
//! @param[in] equations Reference to vector of variable pairs [A_i, B_i]
//! @param[in] values    Reference to vector of values A_i / B_i = values[i]
//! @param[in] queries   Reference to vector of queries [C_j, D_j]
//! @return Answers to all queries. If cannot be determined, return -1.0
static std::vector<double> calcEquationFA(
    const std::vector<std::vector<std::string>>& equations,
    const std::vector<double>&                   values,
    const std::vector<std::vector<std::string>>& queries)
{
    //! @details https://leetcode.com/explore/interview/card/google/61
    //!          /trees-and-graphs/331/

    std::vector<double> answers {};
    answers.reserve(queries.size());

    //! @todo

} // static std::vector<double> calcEquationFA( ...

TEST(CalcEquationTest, SampleTest1)
{
    const std::vector<std::vector<std::string>> equations {
        {"a", "b"}, {"b", "c"}};
    const std::vector<double> values {2.0, 3.0};
    const std::vector<std::vector<std::string>> queries {
        {"a", "c"}, {"b", "a"}, {"a", "e"}, {"a", "a"}, {"x", "x"}};
    const std::vector<double> expected_output {6.0, 0.5, -1.0, 1.0, -1.0};

    EXPECT_EQ(expected_output, calcEquationFA(equations, values, queries));
}

TEST(CalcEquationTest, SampleTest2)
{
    const std::vector<std::vector<std::string>> equations {
        {"a", "b"}, {"b", "c"}, {"bc", "cd"}};
    const std::vector<double> values {1.5, 2.5, 5.0};
    const std::vector<std::vector<std::string>> queries {
        {"a", "c"}, {"c", "b"}, {"bc", "cd"}, {"cd", "bc"}};
    const std::vector<double> expected_output {3.75, 0.4, 5.0, 0.2};

    EXPECT_EQ(expected_output, calcEquationFA(equations, values, queries));
}

TEST(CalcEquationTest, SampleTest3)
{
    const std::vector<std::vector<std::string>> equations {{"a", "b"}};
    const std::vector<double>                   values {0.5};
    const std::vector<std::vector<std::string>> queries {
        {"a", "b"}, {"b", "a"}, {"a", "c"}, {"x", "y"}};
    const std::vector<double> expected_output {0.5, 2.0, -1.0, -1.0};

    EXPECT_EQ(expected_output, calcEquationFA(equations, values, queries));
}
