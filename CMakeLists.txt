cmake_minimum_required(VERSION 4.0)

project(kaleidoscope-lang C)

set(SOURCES
    ./src/main.c
    ./src/lexer/lexer.c
    ./src/token_stream/token_stream.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

# Always-on warnings
target_compile_options(${PROJECT_NAME} PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)

# Debug-specific: symbols, no optimization
target_compile_options(${PROJECT_NAME} PRIVATE 
    "$<$<CONFIG:Debug>:-g>"
)

# Release-specific: optimization, no debug, no asserts
target_compile_options(${PROJECT_NAME} PRIVATE 
    "$<$<CONFIG:Release>:-O3 -DNDEBUG>"
)

target_include_directories(${PROJECT_NAME} PRIVATE
    include
)
