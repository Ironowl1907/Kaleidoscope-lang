#include "lexer_internal.h"
#include "token_stream.h"

#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHANUMERIC_BUFFER_SIZE 32

lexer_t *lexer_create(void) {
  lexer_t *ctx = malloc(sizeof *ctx);
  if (!ctx)
    return NULL;

  ctx->cursor = 0;
  ctx->raw_data[0] = '\0';
  ctx->token_stream = NULL;
  return ctx;
}
void lexer_free(lexer_t *ctx) { free(ctx); }

lexer_error_e lexer_set_raw_data(lexer_t *ctx, const char *raw_data,
                                 size_t size) {
  if (!ctx || !raw_data)
    return LEXER_ERROR_NULL_PARAMETER;

  if (size >= LEXER_RAW_BUFFER_SIZE)
    return LEXER_ERROR_BUFFER_OVERFLOW;

  memcpy(ctx->raw_data, raw_data, size);
  ctx->raw_data[size] = '\0';
  ctx->cursor = 0;

  return LEXER_ERROR_NONE;
}

lexer_error_e lexer_set_token_stream(lexer_t *ctx,
                                     token_stream_t *token_stream) {
  if (!ctx || !token_stream)
    return LEXER_ERROR_NULL_PARAMETER;

  ctx->token_stream = token_stream;
  return LEXER_ERROR_NONE;
}

const char *lexer_get_raw_data(const lexer_t *ctx) {
  if (!ctx)
    return NULL;
  return ctx->raw_data;
}

static char lexer_peek_char(const lexer_t *ctx) {
  if (!ctx || ctx->cursor >= LEXER_RAW_BUFFER_SIZE)
    return '\0';
  return ctx->raw_data[ctx->cursor];
}

static char lexer_consume_char(lexer_t *ctx) {
  char c = lexer_peek_char(ctx);
  if (c != '\0')
    ctx->cursor++;
  return c;
}

static lexer_error_e lexer_emit_token(lexer_t *ctx, token_type_e type,
                                      const uint32_t data) {
  if (!ctx || !ctx->token_stream)
    return LEXER_ERROR_NULL_PARAMETER;

  token_t token;
  token.type = type;

  token.data = data;

  token_stream_error_e err = token_stream_append(ctx->token_stream, token);

  if (err == TOKEN_STREAM_ERROR_INSUFFICIENT_MEMORY)
    return LEXER_ERROR_OUT_OF_MEMORY;

  return LEXER_ERROR_NONE;
}

lexer_error_e lexer_process_data(lexer_t *ctx) {
  if (!ctx || !ctx->token_stream)
    return LEXER_ERROR_NULL_PARAMETER;

  while (lexer_peek_char(ctx) != '\0') {
    char c = lexer_consume_char(ctx);

    if (isspace((unsigned char)c))
      continue;

    switch (c) {
    case '+':
      lexer_emit_token(ctx, TOKEN_TYPE_PLUS, 0);
      break;
    case '-':
      lexer_emit_token(ctx, TOKEN_TYPE_MINUS, 0);
      break;
    case '*':
      lexer_emit_token(ctx, TOKEN_TYPE_STAR, 0);
      break;
    case '/':
      lexer_emit_token(ctx, TOKEN_TYPE_FSLASH, 0);
      break;
    case '(':
      lexer_emit_token(ctx, TOKEN_TYPE_LPARENTESIS, 0);
      break;
    case ')':
      lexer_emit_token(ctx, TOKEN_TYPE_RPARENTESIS, 0);
      break;

    default:
      break;
    }
  }

  lexer_emit_token(ctx, TOKEN_TYPE_EOF, 0);
  return LEXER_ERROR_NONE;
}

void lexer_debug_print_tokens(const lexer_t *ctx) {
  if (!ctx || !ctx->token_stream)
    return;

  size_t count = token_stream_size(ctx->token_stream);

  for (size_t i = 0; i < count; ++i) {
    const token_t tok = token_stream_get(ctx->token_stream, i);

    printf("TOKEN: %d  Data: %u\n", tok.type, tok.data);

    if (tok.type == TOKEN_TYPE_EOF)
      break;
  }
}
