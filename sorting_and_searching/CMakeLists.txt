set (
    cpp_files
    "count_of_smaller_numbers_after_self"
    "find_first_and_last_position_of_element_in_sorted_array"
    "median_of_two_sorted_arrays"
    "merge_intervals"
    "peak_index_in_a_mountain_array"
    "valid_anagram"
)

foreach (cpp_file IN ITEMS ${cpp_files})
    add_executable (${cpp_file}_test ${cpp_file}.cpp)

    target_link_libraries (${cpp_file}_test PRIVATE gtest_main)
endforeach ()

include (GoogleTest)

foreach (cpp_file IN ITEMS ${cpp_files})
    gtest_discover_tests (${cpp_file}_test)
endforeach ()
