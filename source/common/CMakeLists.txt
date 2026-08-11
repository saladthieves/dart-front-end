set(TARGET common)

add_library(${TARGET})

target_sources(${TARGET}
    PRIVATE
        log.cpp
    
    PUBLIC
        FILE_SET HEADERS
)