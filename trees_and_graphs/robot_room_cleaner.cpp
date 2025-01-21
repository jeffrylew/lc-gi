#include <gtest/gtest.h>

#include <unordered_map>
#include <utility>

/*
 * This is the robot's control interface
 * You should not implement it or speculate about its implementation
 *
class Robot
{
public:
    // Returns true if the cell in front is open and robot moves into the cell.
    // Returns false if front cell is blocked and robot stays in current cell
    bool move();

    // Robot will stay in the same cell after calling turnLeft/turnRight
    // Each turn will be 90 degrees
    void turnLeft();
    void turnRight();

    // Clean the current cell
    void clean();
};
 */

//! @brief First attempt to clean an m x n room with walls (0) and cells (1)
//! @param[in, out] robot Reference to robot starting at unknown cell
void cleanRoomFA(Robot& robot)
{
    //! Map of <<row, col>, number of visits>
    std::unordered_map<std::pair<int, int>, int> cell_visits {};

    //! @todo
}

TEST(CleanRoomTest, SampleTest1)
{
    // room = {{1, 1, 1, 1, 1, 0, 1, 1},
    //         {1, 1, 1, 1, 1, 0, 1, 1},
    //         {1, 0, 1, 1, 1, 1, 1, 1},
    //         {0, 0, 0, 1, 0, 0, 0, 0},
    //         {1, 1, 1, 1, 1, 1, 1, 1}}
    // row = 1, col = 3

    Robot robot;
    cleanRoomFA(robot);
}

TEST(CleanRoomTest, SampleTest2)
{
    // room = {{1}}
    // row = 0, col = 0

    Robot robot;
    cleanRoomFA(robot);
}
