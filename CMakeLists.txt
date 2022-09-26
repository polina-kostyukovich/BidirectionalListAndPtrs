cmake_minimum_required(VERSION 3.20)
project(BidirectionalListAndPtrs)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,leak,undefined")

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(BidirectionalListAndPtrs bidirectional_list_tests.cpp bidirectional_list.h shared_ptr.h shared_ptr_tests.cpp weak_ptr.h weak_ptr_tests.cpp)

target_link_libraries(BidirectionalListAndPtrs gtest_main)
