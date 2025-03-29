#include <gtest/gtest.h>

//! @class MinStackFA
//! @brief First attempt to design stack that can get min element in constant T
class MinStackFA
{
public:
    MinStackFA()
    {
    }

    void push(int val)
    {
    }

    void pop()
    {
    }

    int top()
    {
    }

    int getMin()
    {
    }

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
