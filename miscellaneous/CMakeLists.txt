set (
    cpp_files
    "add_binary"
    "crawler_log_folder"
    "create_binary_tree_from_descriptions"
    "encode_n_ary_tree_to_binary_tree"
    "find_the_index_of_the_first_occurrence_in_a_string"
    "find_the_minimum_and_maximum_number_of_nodes_between_critical_points"
    "fruits_into_baskets"
    "intersection_of_two_arrays_ii"
    "kth_distinct_string_in_an_array"
    "lemonade_change"
    "length_of_last_word"
    "longest_common_prefix"
    "longest_substring_with_at_most_k_distinct_characters"
    "lucky_numbers_in_a_matrix"
    "maximum_depth_of_binary_tree"
    "maximum_depth_of_n_ary_tree"
    "merge_nodes_in_between_zeros"
    "minimum_deletions_to_make_string_balanced"
    "minimum_difference_between_largest_and_smallest_value_in_three_moves"
    "minimum_operations_to_make_array_values_equal_to_k"
    "n_ary_tree_level_order_traversal"
    "n_ary_tree_preorder_traversal"
    "n_ary_tree_postorder_traversal"
    "palindrome_number"
    "pass_the_pillow"
    "remove_element"
    "reverse_substrings_between_each_pair_of_parentheses"
    "sort_the_people"
    "sum_of_digits_of_string_after_convert"
    "symmetric_tree"
    "three_consecutive_odds"
    "valid_parentheses"
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