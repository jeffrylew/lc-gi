set (
    cpp_files
    "crawler_log_folder"
    "find_the_minimum_and_maximum_number_of_nodes_between_critical_points"
    "intersection_of_two_arrays_ii"
    "merge_nodes_in_between_zeros"
    "minimum_difference_between_largest_and_smallest_value_in_three_moves"
    "pass_the_pillow"
    "three_consecutive_odds"
    "water_bottles"
)

foreach (cpp_file IN ITEMS ${cpp_files})
    add_executable (${cpp_file}_test ${cpp_file}.cpp)

    target_link_libraries (${cpp_file}_test PRIVATE gtest_main)
endforeach ()

include (GoogleTest)

foreach (cpp_file IN ITEMS ${cpp_files})
    gtest_discover_tests (${cpp_file}_test)
endforeach ()