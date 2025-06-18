set (
    cpp_files
    "3sum"
    "backspace_string_compare"
    "container_with_most_water"
    "expressive_words"
    "find_and_replace_in_string"
    "jump_game"
    "longest_substring_without_repeating_characters"
    "longest_substring_with_at_most_two_distinct_characters"
    "maximum_distance_to_closest_person"
    "meeting_rooms_ii"
    "merge_k_sorted_lists"
    "minimum_window_substring"
    "missing_ranges"
    "multiply_strings"
    "next_closest_time"
    "next_permutation"
    "plus_one"
    "read_N_characters_given_read4_ii"
    "rotate_image"
    "trapping_rain_water"
    "valid_parentheses"
)

foreach (cpp_file IN ITEMS ${cpp_files})
    add_executable (${cpp_file}_test ${cpp_file}.cpp)

    target_link_libraries (${cpp_file}_test PRIVATE gtest_main)
endforeach ()

include (GoogleTest)

foreach (cpp_file IN ITEMS ${cpp_files})
    gtest_discover_tests (${cpp_file}_test)
endforeach ()