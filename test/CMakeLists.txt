project(derive_tests)

cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_custom_command(
    OUTPUT showy-meal.h
    COMMAND echo ${CMAKE_CURRENT_SOURCE_DIR}/../build-debug/derive showy-meal.derive.h -p ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS showy-meal.derive.h
    COMMENT "Generating showy-meal.h from showy-meal.derive.h"
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_executable(test test.cpp showy-meal.h)
