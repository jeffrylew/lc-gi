#include <gtest/gtest.h>

#include <functional>
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

//! @brief First discussion solution to get answers to all queries
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

static double backtrack_evaluate_DS2(
    const Graph_t&                        graph,
    std::string_view                      curr_node,
    std::string_view                      target_node,
    double                                acc_product,
    std::unordered_set<std::string_view>& visited)
{
    //! Mark the visit
    visited.insert(curr_node);

    double answer {-1.0};

    const auto& neighors = graph[curr_node];
    if (neighors.contains(target_node))
    {
        answer = acc_product * neighbors[target_node];
    }
    else
    {
        for (const auto& [next_node, next_val] : neighbors)
        {
            if (visited.contains(next_node))
            {
                continue;
            }

            answer = backtrack_evaluate_DS2(graph,
                                            next_node,
                                            target_node,
                                            acc_product * next_val,
                                            visited);

            if (answer != -1.0)
            {
                break;
            }
        }
    }

    //! Unmark the visit for the next backtracking
    visited.erase(curr_node);
    return answer;

} // static double backtrack_evaluate_DS2( ...

//! @brief Path search in graph discussion soln to get answers to all queries
//! @param[in] equations Reference to vector of variable pairs [A_i, B_i]
//! @param[in] values    Reference to vector of values A_i / B_i = values[i]
//! @param[in] queries   Reference to vector of queries [C_j, D_j]
//! @return Answers to all queries. If cannot be determined, return -1.0
static std::vector<double> calcEquationDS2(
    const std::vector<std::vector<std::string>>& equations,
    const std::vector<double>&                   values,
    const std::vector<std::vector<std::string>>& queries)
{
    //! @details https://leetcode.com/problems/evaluate-division/editorial/
    //!
    //!          Time complexity O(M * N) where M = queries.size() and
    //!          N = equations.size(). We iterate through the equations to build
    //!          a graph in O(N). For each query, need to traverse the graph. In
    //!          the worst case, need to traverse the entire graph in O(N). In
    //!          total, the evaluation of queries could take M * O(N) = O(M * N)
    //!          Space complexity O(N). We build a graph from the equations. In
    //!          the worst case of no overlap between equations, we have N edges
    //!          and 2 * N nodes in the graph. Therefore, the space complexity
    //!          is O(N + 2N) = O(N). Recursion in backtracking uses O(N) for
    //!          the function call stack. The visited set uses O(N).

    Graph_t graph {};

    //! Step 1) Build graph from the equations
    for (int idx = 0; idx < std::ssize(equations); ++idx)
    {
        const auto&  dividend = equations[idx].front();
        const auto&  divisor  = equations[idx].back();
        const double quotient {values[idx]};

        graph[dividend][divisor] = quotient;
        graph[divisor][dividend] = 1 / quotient;
    }

    //! Step 2) Evaluate each query via backtracking (DFS) by verifying
    //! whether there exists a path from dividend to divisor
    std::vector<double> answers {};
    answers.reserve(queries.size());

    for (int idx = 0; idx < std::ssize(queries); ++idx)
    {
        const auto& dividend = queries[idx].front();
        const auto& divisor  = queries[idx].back();

        if (!graph.contains(dividend) || !graph.contains(divisor))
        {
            answers.push_back(-1.0);
        }
        else if (dividend == divisor)
        {
            answers.push_back(-1.0);
        }
        else
        {
            std::unordered_set<std::string_view> visited {};
            answers.push_back(backtrack_evaluate_DS2(graph,
                                                     dividend,
                                                     divisor,
                                                     1.0,
                                                     visited));
        }
    }

    return results;

} // static std::vector<double> calcEquationDS2( ...

//! @brief Union-Find with weights discussion soln to get answers to all queries
//! @param[in] equations Reference to vector of variable pairs [A_i, B_i]
//! @param[in] values    Reference to vector of values A_i / B_i = values[i]
//! @param[in] queries   Reference to vector of queries [C_j, D_j]
//! @return Answers to all queries. If cannot be determined, return -1.0
static std::vector<double> calcEquationDS3(
    const std::vector<std::vector<std::string>>& equations,
    const std::vector<double>&                   values,
    const std::vector<std::vector<std::string>>& queries)
{
    //! @details https://leetcode.com/problems/evaluate-division/editorial/
    //!
    //!          Time complexity O((M + N) * log N) where M = queries.size() and
    //!          N = equations.size(). If M operations, either Union or Find,
    //!          are applied to N elements then the total run time is
    //!          O(M * log N) where we perform unions arbitrarily instead of by
    //!          size or rank. To iterate through each input equation and invoke
    //!          unionize() on it takes O(N * log N). Evaluating each query can
    //!          involve invoking the find() function at most twice, taking
    //!          O(M * log N). The Union-Find data structure is more efficient
    //!          than the DFS/BFS search approach for the repetitive/redundant
    //!          scenario. Once we evaluate a query with Union-Find, subsequent
    //!          repetitive or overlapping queries can be evaluated in constant
    //!          time.
    //!          Space complexity O(N) for the Union-Find data structure. The
    //!          call stack of find() uses O(N).

    std::unordered_map<std::string_view, std::pair<std::string_view, double>>
        gid_weight {};

    std::function<std::pair<std::string_view, double>(std::string_view)> find =
        [&](std::string_view node_id) {
            if (!gid_weight.contains(node_id))
            {
                gid_weight[node_id] = {node_id, 1.0};
            }

            const auto& [group_id, weight] = gid_weight[node_id];

            //! Found inconsistency, trigger chain update
            if (group_id != node_id)
            {
                const auto& [new_group_id, new_weight] = find(group_id);

                gid_weight[node_id] = {new_group_id, weight * new_weight};
            }

            return gid_weight[node_id];
        };

    const auto unionize = [&](std::string_view dividend,
                              std::string_view divisor,
                              double           value) {
        const auto& [dividend_gid, dividend_weight] = find(dividend);
        const auto& [divisor_gid, divisor_weight]   = find(divisor);

        //! Merge the two groups together by attaching the dividend group
        //! to that of the divisor
        if (dividend_gid != divisor_gid)
        {
            gid_weight[dividend_gid] = {
                divisor_gid, divisor_weight * value / dividend_weight};
        }
    };

    //! Step 1) Build the union groups
    for (int idx = 0; idx < std::ssize(equations); ++idx)
    {
        const auto   dividend = equations[idx].front();
        const auto   divisor  = equations[idx].back();
        const double quotient {values[idx]};

        unionize(dividend, divisor, quotient);
    }

    //! Step 2) Run the evaluation with "lazy" updates in find() function
    std::vector<double> answers(queries.size());

    for (int idx = 0; idx < std::ssize(queries); ++idx)
    {
        const auto dividend = queries[idx].front();
        const auto divisor  = queries[idx].back();

        if (!gid_weight.contains(dividend) || !gid_weight.contains(divisor))
        {
            //! Case 1) At least one variable did not appear before
            answers[idx] = -1.0;
            continue;
        }

        const auto& [dividend_gid, dividend_weight] = find(dividend);
        const auto& [divisor_gid, divisor_weight]   = find(divisor);

        if (dividend_gid != divisor_gid)
        {
            //! Case 2) Variables do not belong to the same chain/group
            answers[idx] = -1.0;
        }
        else
        {
            //! Case 3) There is a chain/path between the variables
            answers[idx] = dividend_weight / divisor_weight;
        }
    }

    return answers;

} // static std::vector<double> calcEquationDS3( ...

TEST(CalcEquationTest, SampleTest1)
{
    const std::vector<std::vector<std::string>> equations {
        {"a", "b"}, {"b", "c"}};
    const std::vector<double> values {2.0, 3.0};
    const std::vector<std::vector<std::string>> queries {
        {"a", "c"}, {"b", "a"}, {"a", "e"}, {"a", "a"}, {"x", "x"}};
    const std::vector<double> expected_output {6.0, 0.5, -1.0, 1.0, -1.0};

    EXPECT_EQ(expected_output, calcEquationDS1(equations, values, queries));
    EXPECT_EQ(expected_output, calcEquationDS2(equations, values, queries));
    EXPECT_EQ(expected_output, calcEquationDS3(equations, values, queries));
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
    EXPECT_EQ(expected_output, calcEquationDS2(equations, values, queries));
    EXPECT_EQ(expected_output, calcEquationDS3(equations, values, queries));
}

TEST(CalcEquationTest, SampleTest3)
{
    const std::vector<std::vector<std::string>> equations {{"a", "b"}};
    const std::vector<double>                   values {0.5};
    const std::vector<std::vector<std::string>> queries {
        {"a", "b"}, {"b", "a"}, {"a", "c"}, {"x", "y"}};
    const std::vector<double> expected_output {0.5, 2.0, -1.0, -1.0};

    EXPECT_EQ(expected_output, calcEquationDS1(equations, values, queries));
    EXPECT_EQ(expected_output, calcEquationDS2(equations, values, queries));
    EXPECT_EQ(expected_output, calcEquationDS3(equations, values, queries));
}
