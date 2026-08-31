add_subdirectory(front_end)
add_subdirectory(test_util)

enable_testing()

add_executable(dart_front_end_test)

target_link_libraries(dart_front_end_test
    PRIVATE
        front_end_scanner_test
)

include(GoogleTest)
gtest_discover_tests(dart_front_end_test)