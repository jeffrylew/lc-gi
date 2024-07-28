#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <queue>
#include <set>
#include <tuple>
#include <unordered_map>
#include <vector>

//! @brief First attempt to get ordering of courses to finish all courses
//! @param[in] numCourses    Total num of courses to take in [0, numCourses - 1]
//! @param[in] prerequisites Vector where [a_i, b_i] means must take b_i b4 a_i
//! @return Ordering of courses to finish all or empty vector if impossible
static std::vector<int> findOrderFA(
    int                                  numCourses,
    const std::vector<std::vector<int>>& prerequisites)
{
    //! @details First attempt doesn't pass SampleTest5 but passes 29 / 45 cases

    //! Order of courses initialized to -1
    std::vector<int> course_order(numCourses, -1);

    if (prerequisites.empty())
    {
        std::iota(course_order.begin(), course_order.end(), 0);
        return course_order;
    }

    //! Vector of <course, prerequisites for course>
    std::vector<std::vector<int>> course_dependencies(numCourses);

    //! Set of all courses in prerequisites
    std::set<int> all_in_prereqs {};

    //! Set of courses that are prerequisites (ordered for std::set_difference)
    std::set<int> course_prereqs {};

    for (const auto& prereq : prerequisites)
    {
        const int course {prereq[0]};
        const int pre_course {prereq[1]};

        //! Build graph of dependencies
        course_dependencies[course].push_back(pre_course);

        std::ignore = all_in_prereqs.insert(course);
        std::ignore = all_in_prereqs.insert(pre_course);
        std::ignore = course_prereqs.insert(pre_course);
    }

    std::vector<int> courses_not_prereqs {};
    std::set_difference(all_in_prereqs.begin(),
                        all_in_prereqs.end(),
                        course_prereqs.begin(),
                        course_prereqs.end(),
                        std::inserter(courses_not_prereqs,
                                        courses_not_prereqs.begin()));

    //! There are cyclic dependencies
    if (courses_not_prereqs.empty())
    {
        return {};
    }

    //! Current index in course_order output 
    int order_idx {numCourses - 1};

    //! Queue for processing prerequisite courses
    std::queue<int> prereq_queue {};

    //! Keep track of which courses have been taken
    std::vector<bool> courses_taken(numCourses);

    //! Start traversing from courses that aren't prerequisites for others
    for (const int start : courses_not_prereqs)
    {
        course_order[order_idx--] = start;
        courses_taken[start]      = true;
    }

    for (const int start : courses_not_prereqs)
    {
        for (const auto& prereq : course_dependencies[start])
        {
            prereq_queue.push(prereq);
        }

        while (!prereq_queue.empty())
        {
            const int course {prereq_queue.front()};
            prereq_queue.pop();

            if (courses_taken[course])
            {
                continue;
            }

            if (order_idx >= 0)
            {
                course_order[order_idx--] = course;
            }
            else
            {
                return {};
            }

            courses_taken[course] = true;
        }

        /*
        prereq_queue.push(start);

        while (!prereq_queue.empty())
        {
            const int course {prereq_queue.front()};
            prereq_queue.pop();

            if (courses_taken[course])
            {
                continue;
            }

            if (order_idx >= 0)
            {
                course_order[order_idx--] = course;
            }
            else
            {
                return {};
            }

            courses_taken[course] = true;

            for (const auto& prereq : course_dependencies[course])
            {
                prereq_queue.push(prereq);
            }
        }
        */
    }

    for (int taken_idx = 0; taken_idx < numCourses; ++taken_idx)
    {
        if (!courses_taken[taken_idx])
        {
            if (order_idx >= 0)
            {
                course_order[order_idx--] = taken_idx;
            }
            else
            {
                return {};
            }
        }
    }

    return course_order;

} // static std::vector<int> findOrderFA( ...

//! @brief DFS discussion solution
//! @param[in] numCourses    Total num of courses to take in [0, numCourses - 1]
//! @param[in] prerequisites Vector where [a_i, b_i] means must take b_i b4 a_i
//! @return Ordering of courses to finish all or empty vector if impossible
static std::vector<int> findOrderDS1(
    int                                  numCourses,
    const std::vector<std::vector<int>>& prerequisites)
{
    //! @details https://leetcode.com/problems/course-schedule-ii
    //!
    //!          Time complexity O(V + E) where V = number of vertices and
    //!          E = number of edges. Iterate through each node and vertex once.
    //!          Space complexity O(V + E). Use adjacency list to represent
    //!          graph initially. The space occupied is defined by number of
    //!          edges, O(E). We apply recursion in our algorithm which incurs
    //!          O(E) extra space in the call stack in the worst case. Thus, the
    //!          overall space complexity is O(V + E).

    enum State
    {
        WHITE, // Initial state of vertices
        GRAY,  // Ongoing recursion
        BLACK  // Recursion finished
    };

    //! Becomes false if found a cycle
    bool is_possible {true};

    //! By default all vertices are WHITE
    std::vector<int> color(numCourses, State::WHITE);

    std::vector<int> topological_order {};

    std::unordered_map<int, std::vector<int>> adj_list {};

    //! Create adjacency list representation of the graph
    for (const auto& relation : prerequisites)
    {
        const int dest {relation[0]};
        const int src {relation[1]};

        adj_list[src].push_back(dest);
    }

    std::function<void(int)> dfs = [&](int node) {
        //! Don't recurse further if we found a cycle already
        if (!is_possible)
        {
            return;
        }

        //! Start the recursion
        color[node] = State::GRAY;

        //! Traverse on neighboring vertices
        for (const int neighbor : adj_list[node])
        {
            if (color[neighbor] == State::WHITE)
            {
                dfs(neighbor);
            }
            else if (color[neighbor] == State::GRAY)
            {
                //! An edge to a GRAY vertex represents a cycle
                is_possible = false;
            }
        }

        //! Recursion ends, make node as black
        color[node] = State::BLACK;
        topological_order.push_back(node);

    }; // std::function<void(int)> dfs = ...

    //! If the node is unprocessed then call dfs on it
    for (int node = 0; node < numCourses; ++node)
    {
        if (color[node] == State::WHITE && is_possible)
        {
            dfs(node);
        }
    }

    std::vector<int> order {};
    if (is_possible)
    {
        order.resize(numCourses);

        for (int node = 0; node < numCourses; ++node)
        {
            order[node] = topological_order[numCourses - node - 1];
        }

        /*
         * Alternatively:
         *
        std::reverse(topological_order.begin(), topological_order.end());
        return topological_order;
         */
    }

    return order;

} // static std::vector<int> findOrderDS1( ...

//! @brief Node indegree discussion solution (Kahn's algorithm in comments)
//! @param[in] numCourses    Total num of courses to take in [0, numCourses - 1]
//! @param[in] prerequisites Vector where [a_i, b_i] means must take b_i b4 a_i
//! @return Ordering of courses to finish all or empty vector if impossible
static std::vector<int> findOrderDS2(
    int                                  numCourses,
    const std::vector<std::vector<int>>& prerequisites)
{
    //! @details https://leetcode.com/problems/course-schedule-ii
    //!
    //!          Time complexity O(V + E) where V = number of vertices and E =
    //!          number of edges. Pop each node once from zero_indeg_vertices,
    //!          which takes O(V). For each vertex, iterate over its adjacency
    //!          list. In total, iterate over all edges in the graph which gives
    //!          O(E). Hence, O(V + E).
    //!          Space complexity O(V + E). The indegree vector takes O(V) space
    //!          and use an intermediate queue data structure to keep all nodes
    //!          with 0 in-degree. In the worst case when there aren't any
    //!          prerequisites, the queue will initially contain all vertices
    //!          since all will have 0 in-degree for O(V) space. The adjacency
    //!          list contains all edges for each node and uses O(E).

    std::unordered_map<int, std::vector<int>> adj_list {};

    std::vector<int> indegree(numCourses);
    std::vector<int> topological_order {};

    //! Create adjacency list representation of the graph
    for (const auto& relation : prerequisites)
    {
        const int dest {relation[0]};
        const int src {relation[1]};

        adj_list[src].push_back(dest);

        //! Record in-degree of each vertex
        ++indegree[dest];
    }

    //! Add all vertices with 0 in-degree to the queue
    std::queue<int> zero_indeg_vertices {};
    for (int node = 0; node < numCourses; ++node)
    {
        if (indegree[node] == 0)
        {
            zero_indeg_vertices.push(node);
        }
    }

    //! Process until the queue becomes empty
    while (!zero_indeg_vertices.empty())
    {
        const int node {zero_indeg_vertices.front()};
        zero_indeg_vertices.pop();

        topological_order.push_back(node);

        //! Reduce the in-degree of each neighbor by 1
        if (adj_list.contains(node))
        {
            for (const int neighbor : adj_list[node])
            {
                //! If in-degree of neighbor becomes 0, add it to the queue
                if (--indegree[neighbor] == 0)
                {
                    zero_indeg_vertices.push(neighbor);
                }
            }
        }
    }

    //! Check to see if topological sort is possible or not
    if (static_cast<int>(std::ssize(topological_order)) == numCourses)
    {
        return topological_order;
    }

    return {};

} // static std::vector<int> findOrderDS2( ...

TEST(FindOrderTest, SampleTest1)
{
    constexpr int numCourses {2};

    const std::vector<std::vector<int>> prerequisites {{1, 0}};

    const std::vector<int> expected_output {0, 1};

    EXPECT_EQ(expected_output, findOrderFA(numCourses, prerequisites));
    EXPECT_EQ(expected_output, findOrderDS1(numCourses, prerequisites));
    EXPECT_EQ(expected_output, findOrderDS2(numCourses, prerequisites));
}

TEST(FindOrderTest, SampleTest2)
{
    constexpr int numCourses {4};

    const std::vector<std::vector<int>> prerequisites {
        {1, 0}, {2, 0}, {3, 1}, {3, 2}};

    const std::vector<int> expected_output {0, 2, 1, 3};

    EXPECT_EQ(expected_output, findOrderFA(numCourses, prerequisites));
    EXPECT_EQ(expected_output, findOrderDS1(numCourses, prerequisites));
    EXPECT_EQ(expected_output, findOrderDS2(numCourses, prerequisites));
}

TEST(FindOrderTest, SampleTest3)
{
    constexpr int numCourses {1};

    const std::vector<std::vector<int>> prerequisites {};

    const std::vector<int> expected_output {0};

    EXPECT_EQ(expected_output, findOrderFA(numCourses, prerequisites));
    EXPECT_EQ(expected_output, findOrderDS1(numCourses, prerequisites));
    EXPECT_EQ(expected_output, findOrderDS2(numCourses, prerequisites));
}

TEST(FindOrderTest, SampleTest4)
{
    constexpr int numCourses {3};

    const std::vector<std::vector<int>> prerequisites {{1, 0}, {2, 0}};

    const std::vector<int> expected_output {0, 2, 1};

    EXPECT_EQ(expected_output, findOrderFA(numCourses, prerequisites));
    EXPECT_EQ(expected_output, findOrderDS1(numCourses, prerequisites));
    EXPECT_EQ(expected_output, findOrderDS2(numCourses, prerequisites));
}

TEST(FindOrderTest, SampleTest5)
{
    constexpr int numCourses {3};

    const std::vector<std::vector<int>> prerequisites {{0, 2}, {1, 2}, {2, 0}};

    const std::vector<int> incorrect_output {0, 2, 1};
    const std::vector<int> expected_output {};

    EXPECT_EQ(incorrect_output, findOrderFA(numCourses, prerequisites));
    EXPECT_NE(expected_output, findOrderFA(numCourses, prerequisites));
    EXPECT_EQ(expected_output, findOrderDS1(numCourses, prerequisites));
    EXPECT_EQ(expected_output, findOrderDS2(numCourses, prerequisites));
}
