#include <gtest/gtest.h>

#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using Graph_t =
    std::unordered_map<std::string_view,
                       std::unordered_map<std::string_view, double>>;

static double answerQueryDS1(std::string_view start,
                             std::string_view end,
                             const Graph_t&   graph)
{
    //! No info on this node/numerator/denominator
    if (!graph.contains(start))
    {
        return -1.0;
    }

    std::unordered_set<std::string_view> seen({start});

    //! <node, current ratio/multiplier>
    std::stack<std::pair<std::string_view, double>> node_ratio(
        {std::make_pair(start, 1.0)});

    while (!node_ratio.empty())
    {
        //! Each node is a possible denominator
        const auto [node, ratio] = node_ratio.top();
        node_ratio.pop();

        if (node == end)
        {
            return ratio;
        }

        for (const auto& [neighbor, val] : graph[node])
        {
            if (!seen.contains(neighbor))
            {
                seen.insert(neighbor);
                node_ratio.emplace(neighbor, ratio * val);
            }
        }
    }

    return -1;

} // static double answerQueryDS1( ...

//! @brief Path search in graph discussion soln to get answers to all queries
//! @param[in] equations Reference to vector of variable pairs [A_i, B_i]
//! @param[in] values    Reference to vector of values A_i / B_i = values[i]
//! @param[in] queries   Reference to vector of queries [C_j, D_j]
//! @return Answers to all queries. If cannot be determined, return -1.0
static std::vector<double> calcEquationDS1(
    const std::vector<std::vector<std::string>>& equations,
    const std::vector<double>&                   values,
    const std::vector<std::vector<std::string>>& queries)
{
    //! @details https://leetcode.com/explore/interview/card/google/61
    //!          /trees-and-graphs/331/
    //!          https://leetcode.com/explore/featured/card/
    //!          leetcodes-interview-crash-course-data-structures-and-algorithms
    //!          /707/traversals-trees-graphs/4635/
    //!
    //!          Time complexity O(Q * (N + E)) where Q = queries.size(),
    //!          N = number of variables given in equations (linear with
    //!          equations.size() in the worst case), and E = number of edges.
    //!          Each call to answerQueryDS1 is a graph traversal, which costs
    //!          the number of nodes plus the number of edges N + E. We perform
    //!          Q traversals.
    //!          Space complexity O(N + E) for building graph, seen, and the
    //!          node_ratio stack.

    Graph_t    graph {};
    const auto num_values = static_cast<int>(std::ssize(values));

    for (int idx = 0; idx < num_values; ++idx)
    {
        const std::string_view numerator {equations[idx].front()};
        const std::string_view denominator {equations[idx].back()};

        //! numerator / denominator
        const double val {values[idx]};

        graph[numerator][denominator] = val;
        graph[denominator][numerator] = 1.0 / val;
    }

    std::vector<double> answers {};
    answers.reserve(queries.size());

    for (const auto& query : queries)
    {
        answers.push_back(answerQueryDS1(query.front(), query.back(), graph));
    }

    return answers;

} // static std::vector<double> calcEquationFA( ...

TEST(CalcEquationTest, SampleTest1)
{
    const std::vector<std::vector<std::string>> equations {
        {"a", "b"}, {"b", "c"}};
    const std::vector<double> values {2.0, 3.0};
    const std::vector<std::vector<std::string>> queries {
        {"a", "c"}, {"b", "a"}, {"a", "e"}, {"a", "a"}, {"x", "x"}};
    const std::vector<double> expected_output {6.0, 0.5, -1.0, 1.0, -1.0};

    EXPECT_EQ(expected_output, calcEquationDS1(equations, values, queries));
}

TEST(CalcEquationTest, SampleTest2)
{
    const std::vector<std::vector<std::string>> equations {
        {"a", "b"}, {"b", "c"}, {"bc", "cd"}};
    const std::vector<double> values {1.5, 2.5, 5.0};
    const std::vector<std::vector<std::string>> queries {
        {"a", "c"}, {"c", "b"}, {"bc", "cd"}, {"cd", "bc"}};
    const std::vector<double> expected_output {3.75, 0.4, 5.0, 0.2};

    EXPECT_EQ(expected_output, calcEquationDS1(equations, values, queries));
}

TEST(CalcEquationTest, SampleTest3)
{
    const std::vector<std::vector<std::string>> equations {{"a", "b"}};
    const std::vector<double>                   values {0.5};
    const std::vector<std::vector<std::string>> queries {
        {"a", "b"}, {"b", "a"}, {"a", "c"}, {"x", "y"}};
    const std::vector<double> expected_output {0.5, 2.0, -1.0, -1.0};

    EXPECT_EQ(expected_output, calcEquationDS1(equations, values, queries));
}
