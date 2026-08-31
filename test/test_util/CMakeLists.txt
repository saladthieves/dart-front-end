add_library(test_util)

target_sources(test_util
    PRIVATE
        utils.cpp

    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

target_link_libraries(test_util
    PRIVATE
        common
)