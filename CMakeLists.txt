cmake_minimum_required(VERSION 4.0)

project (kaleidoscope-lang)

set(SOURCES
  main.c
	./src/lexer/lexer.c
	./src/token_stream/token_stream.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
  include
)
