add_library(front_end_messages)

target_sources(front_end_messages
    PRIVATE
        codes.cpp

    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

target_link_libraries(front_end_messages
    PRIVATE
        common
)
