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
        //! Total number of chars read during this function call
        int total_chars_read {};

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
        while (!char_queue.empty() && total_chars_read < missing_chars)
        {
            *(buf + buf_offset) = char_queue.front();
            char_queue.pop();
            ++buf_offset;
            ++total_chars_read;
        }

        return total_chars_read;
    }

private:
    //! Position in buf to start writing at
    int buf_offset {};

    //! Intermediate buffer to store result of read4
    char buf4[4] {};

    //! For storing chars across read4 calls
    std::queue<char> char_queue {};

}; // class SolutionFA
