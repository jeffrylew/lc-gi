set (
    cpp_files
    "bulls_and_cows"
    "candy"
    "fruit_into_baskets"
    "isomorphic_strings"
    "license_key_formatting"
    "my_calendar_ii"
    "odd_even_jump"
    "strobogrammatic_number"
    "unique_email_addresses"
)

foreach (cpp_file IN ITEMS ${cpp_files})
    add_executable (${cpp_file}_test ${cpp_file}.cpp)

    target_link_libraries (${cpp_file}_test PRIVATE gtest_main)
endforeach ()

include (GoogleTest)

foreach (cpp_file IN ITEMS ${cpp_files})
    gtest_discover_tests (${cpp_file}_test)
endforeach ()