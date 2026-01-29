#ifndef LEXER_H
#define LEXER_H


#include "token_stream.h"
#define LEXER_RAW_BUFFER_SIZE 256
#define LEXER_TOKEN_BUFFER_SIZE 256

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum lexer_error {
  LEXER_ERROR_NONE = 0,
  LEXER_ERROR_UNRECOGNIZED_CHAR,
  LEXER_ERROR_BUFFER_OVERFLOW,
  LEXER_ERROR_NULL_PARAMETER,
  LEXER_ERROR_OUT_OF_MEMORY,
  LEXER_ERROR_TOKEN_TOO_LONG,
} lexer_error_e;

typedef struct lexer lexer_t;

// clang-format off

lexer_t *lexer_create();
void lexer_free(lexer_t *ctx);

lexer_error_e lexer_set_raw_data(lexer_t *ctx, const char *raw_data, size_t size);
const char *lexer_get_raw_data(const lexer_t *ctx);

lexer_error_e lexer_set_token_stream(lexer_t *ctx, token_stream_t *token_stream);

lexer_error_e lexer_process_data(lexer_t *ctx);


// Debug
void lexer_debug_print_tokens(const lexer_t *ctx);

#endif
