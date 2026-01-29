cmake_minimum_required(VERSION 4.0)

project (kaleidoscope-lang)

set(SOURCES
  main.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
  include
)
