#include <gtest/gtest.h>

#include <algorithm>
#include <iterator>
#include <numeric>
#include <queue>
#include <set>
#include <tuple>
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

TEST(FindOrderTest, SampleTest1)
{
    constexpr int numCourses {2};

    const std::vector<std::vector<int>> prerequisites {{1, 0}};

    const std::vector<int> expected_output {0, 1};

    EXPECT_EQ(expected_output, findOrderFA(numCourses, prerequisites));
}

TEST(FindOrderTest, SampleTest2)
{
    constexpr int numCourses {4};

    const std::vector<std::vector<int>> prerequisites {
        {1, 0}, {2, 0}, {3, 1}, {3, 2}};

    const std::vector<int> expected_output {0, 2, 1, 3};

    EXPECT_EQ(expected_output, findOrderFA(numCourses, prerequisites));
}

TEST(FindOrderTest, SampleTest3)
{
    constexpr int numCourses {1};

    const std::vector<std::vector<int>> prerequisites {};

    const std::vector<int> expected_output {0};

    EXPECT_EQ(expected_output, findOrderFA(numCourses, prerequisites));
}

TEST(FindOrderTest, SampleTest4)
{
    constexpr int numCourses {3};

    const std::vector<std::vector<int>> prerequisites {{1, 0}, {2, 0}};

    const std::vector<int> expected_output {0, 2, 1};

    EXPECT_EQ(expected_output, findOrderFA(numCourses, prerequisites));
}

TEST(FindOrderTest, SampleTest5)
{
    constexpr int numCourses {3};

    const std::vector<std::vector<int>> prerequisites {{0, 2}, {1, 2}, {2, 0}};

    const std::vector<int> incorrect_output {0, 2, 1};
    const std::vector<int> expected_output {};

    EXPECT_EQ(incorrect_output, findOrderFA(numCourses, prerequisites));
    EXPECT_NE(expected_output, findOrderFA(numCourses, prerequisites));
}
