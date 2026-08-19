add_library(common)

target_sources(common
    PRIVATE
        errors.cpp
        
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/.."
)