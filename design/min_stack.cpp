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

//! @class MinStackDS1
//! @brief Stack of value and min pairs discussion solution
//! @details https://leetcode.com/problems/min-stack/editorial
//!
//!          Time complexity O(1) for all operations.
//!          Space complexity O(N). Worst case is that all operations are push
//!          so there will be O(2 * N) = O(N) space used, where N = the total
//!          number of operations performed.
class MinStackDS1
{
public:
    MinStackDS1() = default;

    //! @brief Push element val onto the stack in O(1) time complexity
    //! @param[in] x Element to push onto the stack
    void push(int x)
    {
        //! If the stack is empty then the min value is the first value we add
        if (stack.empty())
        {
            stack.emplace(x, x);
            return;
        }

        const int curr_min {stack.top().second};
        stack.emplace(x, std::min(x, curr_min));
    }

    //! @brief Remove element on the top of the stack in O(1) time complexity
    void pop()
    {
        stack.pop();
    }

    //! @brief Get top element of the stack in O(1) time complexity
    //! @return Top element of the stack
    int top()
    {
        return stack.top().first;
    }

    //! @brief Retrieve min element in the stack in O(1) time complexity
    //! @return Min element in the stack
    int getMin()
    {
        return stack.top().second;
    }

private:
    //! Stack storing an element and the minimum element seen so far
    std::stack<std::pair<int, int>> stack;

}; // class MinStackDS1

//! @class MinStackDS2
//! @brief Two stacks discussion solution
//! @details https://leetcode.com/problems/min-stack/editorial
//!
//!          Time complexity O(1) for all operations.
//!          Space complexity O(N) where N = total number of operations.
class MinStackDS2
{
public:
    MinStackDS2() = default;

    void push(int x)
    {
        //! Push element to stack
        stack.push(x);

        //! Push min element so far to min_stack
        if (min_stack.empty() || x <= min_stack.top())
        {
            min_stack.push(x);
        }
    }

    void pop()
    {
        //! Remove top element of min_stack if it is equal to the top of stack
        if (stack.top() == min_stack.top())
        {
            min_stack.pop();
        }

        //! Remove top element of stack
        stack.pop();
    }

    int top()
    {
        return stack.top();
    }

    int getMin()
    {
        return min_stack.top();
    }

    private:
    //! Stack storing elements
    std::stack<int> stack;

    //! Stack storing minimum elements
    std::stack<int> min_stack;

}; // class MinStackDS2

//! @class MinStackDS3
//! @brief Improved two stacks discussion solution
//! @details https://leetcode.com/problems/min-stack/editorial
//!
//!          Time complexity O(1) for all operations.
//!          Space complexity O(N) where N = total number of operations.
class MinStackDS3
{
public:
    MinStackDS3() = default;

    void push(int x)
    {
        //! Always put the number onto the main stack
        stack.push(x);

        if (min_stack.empty() || x < min_stack.top().first)
        {
            //! If min stack is empty or x is smaller than the top of the min
            //! stack then push x with a count of 1
            min_stack.emplace(x, 1);
        }
        else if (x == min_stack.top().first)
        {
            //! If x is equal to the top of the min stack then increment count
            ++min_stack.top().second;
        }
    }

    void pop()
    {
        if (stack.top() == min_stack.top().first)
        {
            //! If top of stack is same as top of min stack then decrement count
            --min_stack.top().second;
        }

        if (min_stack.top().second == 0)
        {
            //! If count at top of min stack is 0 then pop
            min_stack.pop();
        }

        //! Like before, pop element from stack
        stack.pop();
    }

    int top()
    {
        return stack.top();
    }

    int getMin()
    {
        return min_stack.top().first;
    }

private:
    std::stack<int> stack;

    //! Stack of <min value, num instances of min value>
    std::stack<std::pair<int, int>> min_stack;

}; // class MinStackDS3

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

    MinStackDS1 min_stack_DS1;
    min_stack_DS1.push(-2);
    min_stack_DS1.push(0);
    min_stack_DS1.push(-3);
    EXPECT_EQ(-3, min_stack_DS1.getMin());

    min_stack_DS1.pop();
    EXPECT_EQ(0, min_stack_DS1.top());
    EXPECT_EQ(-2, min_stack_DS1.getMin());

    MinStackDS2 min_stack_DS2;
    min_stack_DS2.push(-2);
    min_stack_DS2.push(0);
    min_stack_DS2.push(-3);
    EXPECT_EQ(-3, min_stack_DS2.getMin());

    min_stack_DS2.pop();
    EXPECT_EQ(0, min_stack_DS2.top());
    EXPECT_EQ(-2, min_stack_DS2.getMin());

    MinStackDS3 min_stack_DS3;
    min_stack_DS3.push(-2);
    min_stack_DS3.push(0);
    min_stack_DS3.push(-3);
    EXPECT_EQ(-3, min_stack_DS3.getMin());

    min_stack_DS3.pop();
    EXPECT_EQ(0, min_stack_DS3.top());
    EXPECT_EQ(-2, min_stack_DS3.getMin());
}
