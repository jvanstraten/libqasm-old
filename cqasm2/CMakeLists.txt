cmake_minimum_required(VERSION 2.8)

project(libqasm2)

enable_testing()

find_package(BISON 3.0)
find_package(FLEX 2.6)
find_package(PythonInterp 3 REQUIRED)

if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
    endif ()
else ()
    set (CMAKE_CXX_STANDARD 11)
endif ()

BISON_TARGET(
    cQASM2Parser
    flex-bison/parser.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
)
#COMPILE_FLAGS "-Wall -d -t -g -r all"
FLEX_TARGET(
    cQASM2Lexer
    flex-bison/lexer.l
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
)
ADD_FLEX_BISON_DEPENDENCY(cQASM2Lexer cQASM2Parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

file(GLOB_RECURSE LIBCQASM2_SRCS
  "ast/node.cpp"
  "ast/node.hpp"
#  "ast/*.cpp"
#  "ast/*.hpp"
#  "semantic/*.cpp"
#  "semantic/*.hpp"
)

add_library(
    cqasm2 SHARED
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
    ${LIBCQASM2_SRCS}
)

file(GLOB_RECURSE CQASM2CHECK_SRCS
  "check/*.cpp"
  "check/*.hpp"
)

add_executable(cqasm2check ${CQASM2CHECK_SRCS})
target_link_libraries(cqasm2check cqasm2)

# 
# install(TARGETS libqasm2 RUNTIME DESTINATION bin)
