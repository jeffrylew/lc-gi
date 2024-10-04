set (
    cpp_files
    "3sum"
    "container_with_most_water"
    "jump_game"
    "longest_substring_without_repeating_characters"
    "multiply_strings"
    "next_permutation"
    "rotate_image"
)

foreach (cpp_file IN ITEMS ${cpp_files})
    add_executable (${cpp_file}_test ${cpp_file}.cpp)

    target_link_libraries (${cpp_file}_test PRIVATE gtest_main)
endforeach ()

include (GoogleTest)

foreach (cpp_file IN ITEMS ${cpp_files})
    gtest_discover_tests (${cpp_file}_test)
endforeach ()