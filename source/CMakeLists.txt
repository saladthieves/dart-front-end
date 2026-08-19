add_subdirectory(common)
add_subdirectory(front_end)
add_subdirectory(util)

add_executable(dart_front_end)

target_sources(dart_front_end
    PRIVATE
        main.cpp
)

target_link_libraries(dart_front_end
    PRIVATE
        common
        front_end_base
        front_end_messages
        front_end_scanner_token
        front_end_scanner
        util
)