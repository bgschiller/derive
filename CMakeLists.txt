project(derive)
cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(LLVM_INSTALL_PREFIX $ENV{LLVM_INSTALL_PREFIX} CACHE PATH "Path to LLVM installation root directory")

list(APPEND CMAKE_MODULE_PATH ${LLVM_INSTALL_PREFIX}/lib/cmake/llvm)
list(APPEND CMAKE_MODULE_PATH ${LLVM_INSTALL_PREFIX}/lib/cmake/clang)

include(AddLLVM)
include(ClangConfig)

include_directories(${CLANG_INCLUDE_DIRS} ${LLVM_INCLUDE_DIRS})

add_llvm_executable(derive derive.cpp)

target_link_libraries(derive
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangTooling
  clangToolingRefactoring
)
