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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

# -----------------------------------------------------------------------------
# Lexer/parser generation targets
# -----------------------------------------------------------------------------

BISON_TARGET(
    cQASM2Parser
    parser/parser.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
)
FLEX_TARGET(
    cQASM2Lexer
    parser/lexer.l
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
)
ADD_FLEX_BISON_DEPENDENCY(cQASM2Lexer cQASM2Parser)


# -----------------------------------------------------------------------------
# AST class generation targets
# -----------------------------------------------------------------------------

set (AST_GEN ${CMAKE_CURRENT_BINARY_DIR}/ast.gen.cpp ${CMAKE_CURRENT_BINARY_DIR}/ast.gen.hpp)

add_custom_command(
    OUTPUT ${AST_GEN}
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/tools/tree.py ${CMAKE_CURRENT_SOURCE_DIR}/ast/ast.tree
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/tools/tree.py ${CMAKE_CURRENT_SOURCE_DIR}/ast/ast.tree
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)


# -----------------------------------------------------------------------------
# Include directories
# -----------------------------------------------------------------------------

# Since we're generating files, we need to add the build directory to our
# includes.
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# We also add one directory up as include directory. This allows "absolute"
# include paths like "cqasm2/ast/ast.hpp" to be used anywhere, just like how
# the includes should work in a project using libcqasm2.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)


# -----------------------------------------------------------------------------
# Linking libcqasm2
# -----------------------------------------------------------------------------

file(GLOB_RECURSE LIBCQASM2_SRCS
  "ast/*.cpp"
  "ast/*.hpp"
#  "semantic/*.cpp"
#  "semantic/*.hpp"
)

add_library(
    cqasm2 SHARED
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/ast.gen.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/ast.gen.hpp
    ${LIBCQASM2_SRCS}
)


# -----------------------------------------------------------------------------
# Building cqasm2tool
# -----------------------------------------------------------------------------

# cqasm2tool is an executable exposing some of libcqasm2's functionality to a
# command line environment.

file(GLOB_RECURSE CQASM2TOOL_SRCS
  "cqasm2tool/*.cpp"
  "cqasm2tool/*.hpp"
)

add_executable(cqasm2tool ${CQASM2TOOL_SRCS})

target_link_libraries(
    cqasm2tool
    cqasm2
)


# -----------------------------------------------------------------------------
# Library installation
# -----------------------------------------------------------------------------

# 
# install(TARGETS libqasm2 RUNTIME DESTINATION bin)
