#include <gtest/gtest.h>

#include <algorithm>
#include <stack>
#include <utility>

//! @class MinStackFA
//! @brief First attempt to design stack that can get min element in constant T
//! @details https://leetcode.com/explore/interview/card/google/65/design-4/3091
class MinStackFA
{
public:
    //! @brief Constructor initializes stack object
    MinStackFA() = default;

    //! @brief Push element val onto the stack in O(1) time complexity
    //! @param[in] val Element to push onto the stack
    void push(int val)
    {
        if (ele_min.empty())
        {
            ele_min.emplace(val, val);
        }
        else
        {
            const int prev_min {ele_min.top().second};
            ele_min.emplace(val, std::min(prev_min, val));
        }
    }

    //! @brief Remove element on the top of the stack in O(1) time complexity
    void pop()
    {
        ele_min.pop();
    }

    //! @brief Get top element of the stack in O(1) time complexity
    //! @return Top element of the stack
    int top()
    {
        return ele_min.top().first;
    }

    //! @brief Retrieve min element in the stack in O(1) time complexity
    //! @return Min element in the stack
    int getMin()
    {
        return ele_min.top().second;
    }

private:
    //! Stack storing an element and the minimum element seen so far
    std::stack<std::pair<int, int>> ele_min;

}; // class MinStackFA

TEST(MinStackTest, SampleTest1)
{
    MinStackFA min_stack_FA;
    min_stack_FA.push(-2);
    min_stack_FA.push(0);
    min_stack_FA.push(-3);
    EXPECT_EQ(-3, min_stack_FA.getMin());

    min_stack_FA.pop();
    EXPECT_EQ(0, min_stack_FA.top());
    EXPECT_EQ(-2, min_stack_FA.getMin());
}
