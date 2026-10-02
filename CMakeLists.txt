cmake_minimum_required(VERSION 3.10)
project(cardval VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_compile_options(
    -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Werror
)

# Library
add_library(cardval STATIC src/cardval.c)
target_include_directories(cardval PUBLIC include)

# CLI Example
add_executable(cardval_cli examples/cli.c)
target_link_libraries(cardval_cli PRIVATE cardval)

# Tests
add_executable(cardval_test tests/test_cardval.c)
target_link_libraries(cardval_test PRIVATE cardval)
enable_testing()
add_test(NAME cardval_test COMMAND cardval_test)

# Benchmarks
add_executable(cardval_bench bench/bench_cardval.c)
target_link_libraries(cardval_bench PRIVATE cardval)

# Sanitizer options
option(ENABLE_SANITIZERS "Enable Address and Undefined Behavior Sanitizers" OFF)
if(ENABLE_SANITIZERS)
    set(SAN_FLAGS "-fsanitize=address,undefined,leak -fno-omit-frame-pointer")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SAN_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SAN_FLAGS}")
endif()
