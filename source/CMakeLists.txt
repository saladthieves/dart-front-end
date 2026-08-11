add_subdirectory(common)

set(TARGET dart-front-end)

add_executable(${TARGET})

target_sources(${TARGET}
    PRIVATE
        main.cpp
)

target_link_libraries(${TARGET}
    PRIVATE
        common
)

