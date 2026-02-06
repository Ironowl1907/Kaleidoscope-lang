cmake_minimum_required(VERSION 3.15)
project(kaleidoscope-lang C)

set(SOURCES
    ./src/main.c
    ./src/lexer/lexer.c
    ./src/token_stream/token_stream.c
    ./src/parser/parser.c
    ./src/ast/ast.c
    ./src/codegen/codegen.c
)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

add_executable(${PROJECT_NAME} ${SOURCES})

# Always-on warnings
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

# Debug-specific
target_compile_options(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:-g>"
)

# Release-specific
target_compile_options(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Release>:-O3 -DNDEBUG>"
)

# Your includes + LLVM includes
target_include_directories(${PROJECT_NAME} PRIVATE
    include
    ${LLVM_INCLUDE_DIRS}
)

# LLVM compile definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    ${LLVM_DEFINITIONS}
)

target_link_libraries(${PROJECT_NAME}
    LLVM
)
