#include <gtest/gtest.h>

#include <functional>
#include <set>
#include <utility>
#include <vector>

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

//! @brief Spiral backtracking discussion solution
//! @param[in, out] robot Reference to robot starting at unknown cell
void cleanRoomDS(Robot& robot)
{
    //! @details https://leetcode.com/problems/robot-room-cleaner/editorial/

    //! Going clockwise:                             col -1   0   1
    //! - 0 : up    (delta row = -1, delta col = 0) row  _ _ _ _ _ _
    //! - 1 : right (delta row = 0, delta col = 1)   -1 |_ _|_ _|_ _|
    //! - 2 : down  (delta row = 1, delta col = 0)    0 |_ _|_x_|_ _|
    //! - 3 : left  (delta row = 0, delta col = -1)   1 |_ _|_ _|_ _|
    //!
    //! directions vector stores <delta row, delta col>
    const std::vector<std::pair<int, int>> directions {
        {-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    //! std::unordered_set requires defining a hash for std::pair
    std::set<std::pair<int, int>> visited_cells {};

    const auto go_back = [&robot] {
        robot.turnRight();
        robot.turnRight();
        robot.move();
        robot.turnRight();
        robot.turnRight();
    };

    const std::function<void(int, int, int)> backtrack =
        [&](int curr_row, int curr_col, int curr_direction) {
            visited_cells.emplace(curr_row, curr_col);
            robot.clean();

            //! Going clockwise:
            //! - 0 : up
            //! - 1 : right
            //! - 2 : down
            //! - 3 : left
            for (int num_right_turns = 0;
                 num_right_turns < 4;
                 ++num_right_turns)
            {
                //! Ensure direction remains in [0, 3]
                const int next_direction {
                    (curr_direction + num_right_turns) % 4};
                const auto& [delta_row, delta_col] =
                    directions.at(next_direction);
                const int next_row {curr_row + delta_row};
                const int next_col {curr_col + delta_col};

                if (!visited_cells.contains({next_row, next_col})
                    && robot.move())
                {
                    backtrack(next_row, next_col, next_direction);
                    go_back();
                }

                //! Turn the robot clockwise following the chosen direction
                robot.turnRight();
            }
        };

    backtrack(0, 0, 0);

} // void cleanRoomDS( ...

TEST(CleanRoomTest, SampleTest1)
{
    // room = {{1, 1, 1, 1, 1, 0, 1, 1},
    //         {1, 1, 1, 1, 1, 0, 1, 1},
    //         {1, 0, 1, 1, 1, 1, 1, 1},
    //         {0, 0, 0, 1, 0, 0, 0, 0},
    //         {1, 1, 1, 1, 1, 1, 1, 1}}
    // row = 1, col = 3

    Robot robot;
    cleanRoomDS(robot);
}

TEST(CleanRoomTest, SampleTest2)
{
    // room = {{1}}
    // row = 0, col = 0

    Robot robot;
    cleanRoomDS(robot);
}
