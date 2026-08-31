# Google Test

set(M_GTEST_HASH 03597a01ee50ed33e9dfd640b249b4be3799d395)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/${M_GTEST_HASH}.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
