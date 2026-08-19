add_subdirectory(token)

add_library(front_end_scanner)

target_sources(front_end_scanner
    PRIVATE
        abstract_scanner.cpp
        scanner.cpp

    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

target_link_libraries(front_end_scanner
    PRIVATE
        common
        front_end_base
        front_end_scanner_token
        util
)
