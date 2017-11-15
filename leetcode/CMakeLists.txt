cmake_minimum_required(VERSION 3.7)
project(leetcode)

set(CMAKE_CXX_COMPILER_ID "Clang")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -Wall")
set(CMAKE_CXX_STANDARD 11)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)


include_directories(include)
file(GLOB SOURCE_FILES
        "./array/*.cpp")

add_executable(leetcode main.cpp ${SOURCE_FILES})