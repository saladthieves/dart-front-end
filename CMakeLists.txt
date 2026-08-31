cmake_minimum_required(VERSION 4.0.0)

include(cmake/init.cmake)

project(
    dart_front_end
    VERSION 0.0.1
    LANGUAGES CXX
)

add_subdirectory(source)

if(DFE_ENABLE_TESTING)
    include(cmake/modules.cmake)
    add_subdirectory(test)
endif()