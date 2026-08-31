add_library(front_end_scanner_test)

target_sources(front_end_scanner_test
    PUBLIC
        comments_test.cpp
        scanner_test.cpp
        string_test.cpp
    
    PRIVATE
        FILE_SET HEADERS
)

target_link_libraries(front_end_scanner_test
    PUBLIC
        common
        front_end_scanner
        front_end_scanner_token
        util
        test_util
        GTest::gtest_main
)