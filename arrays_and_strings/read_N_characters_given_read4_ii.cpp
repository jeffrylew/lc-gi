#include <gtest/gtest.h>

#include <algorithm>

class SolutionFA
{
public:
    //! First attempt to read n chars from file into buf
    //! @param buf Destination buffer
    //! @param n   Number of characters to read
    //! @return The number of actual characters read
    int read(char* buf, int n)
    {
        //! Total number of chars read during this function call
        int total_chars_read {};

        const int full_reads {n / 4};

        for (int num_reads = 0; num_reads < full_reads; ++num_reads)
        {
            const int curr_chars_read {read4(buf4)};
            if (curr_chars_read == 0)
            {
                return total_chars_read;
            }

            std::copy(buf4, buf4 + curr_chars_read, buf + buf_offset);
            total_chars_read += curr_chars_read;
            buf_offset += curr_chars_read;
        }

        //! @todo Handle read for remaining chars
        const int num_remaining_chars {n % 4};
        const int num_read {read4(buf4)};

        return total_chars_read;
    }

private:
    //! Position in buf to start writing at
    int  buf_offset {};

    //! Intermediate buffer to store result of read4
    char buf4[4] {};
};
