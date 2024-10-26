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

class SolutionDS1
{
public:
    //! @brief Discussion solution (Java) to read n chars from file into buf
    //! @param[out] buf Destination buffer
    //! @param[in]  n   Number of characters to read
    //! @return The number of actual characters read
    int read(char* buf, int n)
    {
        //! @details https://leetcode.com/explore/interview/card/google/59
        //!          /array-and-strings/436/discuss/49598/A-simple-Java-code

        //! Index in output buf to write to
        int ptr {};

        while (ptr < n)
        {
            if (buff_ptr == 0)
            {
                buff_cnt = read4(buff);
            }

            if (buff_cnt == 0)
            {
                break;
            }

            while (ptr < n && buff_ptr < buff_cnt)
            {
                buf[ptr++] = buff[buff_ptr++];
            }

            //! If buff_ptr reaches current buff_cnt, set to 0 to read new data
            if (buff_ptr >= buff_cnt)
            {
                buff_ptr = 0;
            }
        }

        return ptr;
    }

private:
    //! Store data received in previous calls. buff_ptr is index in buff
    int buff_ptr {};
    int buff_cnt {};

    char buff[4] {};

}; // class SolutionDS1

class SolutionDS2
{
public:
    //! @brief Discussion solution follow up to speed up copy
    //! @param[out] buf Destination buffer
    //! @param[in]  n   Number of characters to read
    //! @return The number of actual characters read
    int read(char* buf, int n)
    {
        //! @details https://leetcode.com/explore/interview/card/google/59
        //!          /array-and-strings/436/discuss/188293
        //!          /Google-follow-up-question.-Speed-up-the-copy.

        //! Index in output buf to write to
        int ptr {};

        while (ptr < n)
        {
            //! Read from buf4 first if it has something
            if (buff_ptr < buff_cnt)
            {
                buf[ptr++] = buff[buff_ptr++];
                continue;
            }

            //! If there is enough space in buf
            if (n - ptr >= 4)
            {
                const int chars_read {read4(buf + ptr)};
                ptr += chars_read;

                if (chars_read == 0)
                {
                    return ptr;
                }
            }
            else
            {
                //! n - ptr < 4
                buff_cnt = read4(buff);
                buff_ptr = 0;

                if (buff_cnt == 0)
                {
                    return ptr;
                }
            }
        }

        return ptr;
    }

private:
    //! Store data received in previous calls. buff_ptr is index in buff
    int buff_ptr {};
    int buff_cnt {};

    char buff[4] {};

}; // class SolutionDS2

class SolutionDS3
{
public:
    //! @brief Another discussion solution from Java
    //! @param[out] buf Destination buffer
    //! @param[in]  n   Number of characters to read
    //! @return The number of actual characters read
    int read(char* buf, int n)
    {
        //! @details https://leetcode.com/explore/interview/card/google/59
        //!          /array-and-strings/436/discuss/49615/Clean-solution-in-Java

        int counter {};

        while (counter < n)
        {
            if (prev_idx < prev_size)
            {
                buf[counter++] = prev_buf[prev_idx++];
                continue;
            }

            prev_size = read4(prev_buf);
            prev_idx  = 0;

            if (prev_size == 0)
            {
                //! No more data to consume from stream
                break;
            }
        }

        return counter;
    }

private:
    //! Track actual size of prev_buf
    int prev_size {};

    //! Index of next char to read from prev_buf
    int prev_idx {};

    //! Stores chars from read4
    char prev_buf[4] {};

}; // class SolutionDS3

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

    SolutionDS1 sol_ds1;
    char        buf_ds1[3] {};
    EXPECT_EQ(1, sol_ds1.read(buf_ds1, 1));
    EXPECT_EQ('a', buf_ds1[0]);

    EXPECT_EQ(2, sol_ds1.read(buf_ds1, 2));
    EXPECT_EQ('b', buf_ds1[0]);
    EXPECT_EQ('c', buf_ds1[1]);

    EXPECT_EQ(0, sol_ds1.read(buf_ds1, 1));

    SolutionDS2 sol_ds2;
    char        buf_ds2[3] {};
    EXPECT_EQ(1, sol_ds2.read(buf_ds2, 1));
    EXPECT_EQ('a', buf_ds2[0]);

    EXPECT_EQ(2, sol_ds2.read(buf_ds2, 2));
    EXPECT_EQ('b', buf_ds2[0]);
    EXPECT_EQ('c', buf_ds2[1]);

    EXPECT_EQ(0, sol_ds2.read(buf_ds2, 1));
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

    SolutionDS1 sol_ds1;
    char        buf_ds1[3] {};
    EXPECT_EQ(3, sol_ds1.read(buf_ds1, 4));
    EXPECT_EQ('a', buf_ds1[0]);
    EXPECT_EQ('b', buf_ds1[1]);
    EXPECT_EQ('c', buf_ds1[2]);

    SolutionDS2 sol_ds2;
    char        buf_ds2[3] {};
    EXPECT_EQ(3, sol_ds2.read(buf_ds2, 4));
    EXPECT_EQ('a', buf_ds2[0]);
    EXPECT_EQ('b', buf_ds2[1]);
    EXPECT_EQ('c', buf_ds2[2]);
}
