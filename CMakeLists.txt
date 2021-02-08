project(derive)
cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(LLVM_INSTALL_PREFIX $ENV{LLVM_INSTALL_PREFIX} CACHE PATH "Path to LLVM installation root directory")

get_filename_component(CLANG_BIN_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
set(CLANG_BASE ${CLANG_BIN_DIR}/..)
message(STATUS "CLANG_BASE:" ${CLANG_BASE})
message(STATUS "CMAKE_CXX_COMPILER:" ${CMAKE_CXX_COMPILER})
# Guess include paths for the standard C++ library.
# These paths are used from CoARCT tests and applications to configure the
# compilers instantiated in those programs.
set(CLANG_INCLUDE_DIR1 ${CLANG_BASE}/include/c++/v1)
set(CLANG_INCLUDE_DIR2 ${CLANG_BASE}/lib/clang/10.0.0/include)
message(STATUS "CLANG_INCLUDE_DIR1: " ${CLANG_INCLUDE_DIR1})
add_definitions(-DCLANG_INC_DIR1="${CLANG_INCLUDE_DIR1}")
add_definitions(-DCLANG_INC_DIR2="${CLANG_INCLUDE_DIR2}")



list(APPEND CMAKE_MODULE_PATH ${LLVM_INSTALL_PREFIX}/lib/cmake/llvm)
list(APPEND CMAKE_MODULE_PATH ${LLVM_INSTALL_PREFIX}/lib/cmake/clang)

include(AddLLVM)
include(ClangConfig)


message(STATUS "API file: " ${CMAKE_CURRENT_SOURCE_DIR}/api.cpp)

add_library(derive ${CMAKE_CURRENT_SOURCE_DIR}/api.cpp)

target_include_directories(derive
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CLANG_INCLUDE_DIRS}
  ${LLVM_INCLUDE_DIRS})

target_link_libraries(derive
  PUBLIC
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangTooling
  clangToolingRefactoring
)

add_llvm_executable(deriveExecutable derive.cpp api.cpp)

target_link_libraries(deriveExecutable
  PRIVATE
  derive
)
