# CMake script for building very simple parser
# with bison and 'reflex' scanner generator.
#
#
project(Quack
        # DESCRIPTION "Quack parser for CIS 461/561"
        # LANGUAGES CXX
        )
cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 14)


# Where are the reflex include and library files?
#
set(REFLEX_LIB "/usr/local/lib/libreflex.a")
set(REFLEX_INCLUDE "/usr/local/include/reflex")
set(LOCAL_INCLUDE "/usr/local/include")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I ${LOCAL_INCLUDE} -Wall")


# Recipes for generating C source files from .lxx and .yxx files
#

add_custom_command(
        OUTPUT lex.yy.cpp lex.yy.h
        COMMAND   reflex --bison-cc --bison-locations --header-file
        ${CMAKE_CURRENT_SOURCE_DIR}/quack.lxx
        DEPENDS quack.lxx quack.tab.hxx
)
add_custom_command(
        OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/quack.tab.cxx
        ${CMAKE_CURRENT_BINARY_DIR}/quack.tab.hxx
        ${CMAKE_CURRENT_BINARY_DIR}/location.hh
        ${CMAKE_CURRENT_BINARY_DIR}/position.hh
        ${CMAKE_CURRENT_BINARY_DIR}/stack.hh
        COMMAND bison  --defines --debug --token-table --report=all ${CMAKE_CURRENT_SOURCE_DIR}/quack.yxx
        DEPENDS quack.yxx
)

include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}
        ${REFLEX_INCLUDE}
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# I want the executables in the top-level 'build' directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_executable(parser
        quack.tab.cxx lex.yy.cpp lex.yy.h
        parser.cxx
        ASTNode.cxx ASTNode.h
        Messages.h Messages.cxx
        CodegenContext.cpp CodegenContext.h
        # EvalContext.h
        )


target_link_libraries(parser ${REFLEX_LIB})