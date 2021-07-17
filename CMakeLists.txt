cmake_minimum_required(VERSION 3.14)

project(Parallel_Cpp_Linalg)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -pedantic -Werror)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build")

add_subdirectory(tests)
# add_subdirectory(benchmarks)
add_subdirectory(src/Matrix)
