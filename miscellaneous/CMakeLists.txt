set (
    cpp_files
    "crawler_log_folder"
    "create_binary_tree_from_descriptions"
    "find_the_minimum_and_maximum_number_of_nodes_between_critical_points"
    "intersection_of_two_arrays_ii"
    "kth_distinct_string_in_an_array"
    "lemonade_change"
    "lucky_numbers_in_a_matrix"
    "merge_nodes_in_between_zeros"
    "minimum_deletions_to_make_string_balanced"
    "minimum_difference_between_largest_and_smallest_value_in_three_moves"
    "palindrome_number"
    "pass_the_pillow"
    "reverse_substrings_between_each_pair_of_parentheses"
    "sort_the_people"
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