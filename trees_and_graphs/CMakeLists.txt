set (
    cpp_files
    "binary_tree_maximum_path_sum"
    "count_complete_tree_nodes"
    "course_schedule_ii"
    "cracking_the_safe"
    "decode_string"
    "evaluate_division"
    "flip_equivalent_binary_trees"
    "longest_increasing_path_in_matrix"
    "most_stones_removed_with_same_row_or_column"
    "number_of_islands"
    "robot_room_cleaner"
)

foreach (cpp_file IN ITEMS ${cpp_files})
    add_executable (${cpp_file}_test ${cpp_file}.cpp)

    target_link_libraries (${cpp_file}_test PRIVATE gtest_main)
endforeach ()

include (GoogleTest)

foreach (cpp_file IN ITEMS ${cpp_files})
    gtest_discover_tests (${cpp_file}_test)
endforeach ()