add_library(util)

target_sources(util
    PRIVATE
        link.cpp
        
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/.."
)