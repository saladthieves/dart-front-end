add_library(front_end_scanner_token)

target_sources(front_end_scanner_token
    PRIVATE
        error_token.cpp
        keyword_state.cpp
        token.cpp

    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

target_link_libraries(front_end_scanner_token
    PRIVATE
        front_end_base
        front_end_messages
        common
)