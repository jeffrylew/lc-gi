set (
    cpp_files
    "insert_delete_getrandom_O1"
    "logger_rate_limiter"
    "lru_cache"
    "min_stack"
    "serialize_and_deserialize_binary_tree"
)

foreach (cpp_file IN ITEMS ${cpp_files})
    add_executable (${cpp_file}_test ${cpp_file}.cpp)

    target_link_libraries (${cpp_file}_test PRIVATE gtest_main)
endforeach ()

include (GoogleTest)

foreach (cpp_file IN ITEMS ${cpp_files})
    gtest_discover_tests (${cpp_file}_test)
endforeach ()