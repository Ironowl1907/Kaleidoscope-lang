#ifndef LEXER_H
#define LEXER_H

#include "token_stream.h"
#include <stddef.h>
#include <stdint.h>

typedef enum lexer_error {
  LEXER_ERROR_NONE = 0,
  LEXER_ERROR_UNRECOGNIZED_CHAR,
  LEXER_ERROR_NULL_PARAMETER,
  LEXER_ERROR_OUT_OF_MEMORY,
} lexer_error_e;

typedef struct lexer lexer_t;

lexer_t *lexer_create(void);
void lexer_free(lexer_t *ctx);

lexer_error_e lexer_set_raw_data(lexer_t *ctx, const char *raw_data,
                                 size_t size);

lexer_error_e lexer_set_token_stream(lexer_t *ctx, token_stream_t *stream);

lexer_error_e lexer_process_data(lexer_t *ctx);

void lexer_debug_print_tokens(const lexer_t *ctx);

#endif
