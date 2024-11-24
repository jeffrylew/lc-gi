cmake_minimum_required (VERSION 3.30)
project (lc-gi)

# GoogleTest requires at least C++14
set (CXX_STANDARD 23)

include (FetchContent)
FetchContent_Declare (
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.14.0.zip
)

FetchContent_MakeAvailable (googletest)

enable_testing ()

add_subdirectory (arrays_and_strings)
add_subdirectory (linked_lists)
add_subdirectory (trees_and_graphs)
# add_subdirectory (recursion)
add_subdirectory (sorting_and_searching)
# add_subdirectory (dynamic_programming)
# add_subdirectory (design)
# add_subdirectory (other)
add_subdirectory (miscellaneous)