add_library(front_end_base)

target_sources(front_end_base
    PRIVATE
        source_range.cpp
        
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/.."
)