#pragma comment(lib, "pthreadVC3.lib")

cmake_minimum_required(VERSION 3.22)
project(t)

set(CMAKE_CXX_STANDARD 14)

set(BUILD_ARCH "-m64")
set(TARGET_64 ON)

find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(t ./src/main.cpp)

llvm_map_components_to_libnames(llvm_libs support core irreader)

target_link_libraries(t ${llvm_libs})