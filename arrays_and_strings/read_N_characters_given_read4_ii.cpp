#include <gtest/gtest.h>

#include <algorithm>
#include <queue>

class SolutionFA
{
public:
    //! @brief First attempt to read n chars from file into buf
    //! @param[out] buf Destination buffer
    //! @param[in]  n   Number of characters to read
    //! @return The number of actual characters read
    int read(char* buf, int n)
    {
        //! @details https://leetcode.com/explore/interview/card/google/59
        //!          /array-and-strings/436

        //! Total number of chars read during this function call
        int total_chars_read {};

        //! Position in buf to start writing at. Resets every call to read.
        int buf_offset {};

        //! If there are chars available in char_queue, write them to buf
        while (!char_queue.empty() && total_chars_read < n)
        {
            *(buf + buf_offset) = char_queue.front();
            char_queue.pop();
            ++buf_offset;
            ++total_chars_read;
        }

        //! Read the requested number of chars
        if (total_chars_read == n)
        {
            return total_chars_read;
        }

        //! Wrote all available chars in char_queue, need to read more
        const int missing_chars {n - total_chars_read};

        //! Read and copy chars to char_queue
        int remaining_chars {missing_chars};
        while (remaining_chars > 0)
        {
            const int curr_chars_read {read4(buf4)};
            if (curr_chars_read == 0)
            {
                //! No more chars available
                break;
            }

            //! Copy chars from buf4 to char_queue
            for (int buf4_idx = 0; buf4_idx < curr_chars_read; ++buf4_idx)
            {
                char_queue.push(buf4[buf4_idx]);
            }

            remaining_chars -= curr_chars_read;

        } // while (remaining_chars > 0)

        //! Write chars from char_queue
        while (!char_queue.empty() && total_chars_read < n)
        {
            *(buf + buf_offset) = char_queue.front();
            char_queue.pop();
            ++buf_offset;
            ++total_chars_read;
        }

        return total_chars_read;
    }

private:
    //! Intermediate buffer to store result of read4
    char buf4[4] {};

    //! For storing chars across read4 calls
    std::queue<char> char_queue {};

}; // class SolutionFA

TEST(ReadTest, SampleTest1)
{
    //! file = "abc"
    SolutionFA sol;
    char       buf[3] {};
    EXPECT_EQ(1, sol.read(buf, 1));
    EXPECT_EQ('a', buf[0]);

    EXPECT_EQ(2, sol.read(buf, 2));
    EXPECT_EQ('b', buf[0]);
    EXPECT_EQ('c', buf[1]);

    EXPECT_EQ(0, sol.read(buf, 1));
}

TEST(ReadTest, SampleTest2)
{
    //! file = "abc"
    SolutionFA sol;
    char       buf[3] {};
    EXPECT_EQ(3, sol.read(buf, 4));
    EXPECT_EQ('a', buf[0]);
    EXPECT_EQ('b', buf[1]);
    EXPECT_EQ('c', buf[2]);

    EXPECT_EQ(0, sol.read(buf, 1));
}
