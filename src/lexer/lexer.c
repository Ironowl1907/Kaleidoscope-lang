#include "lexer.h"
#include "token_stream.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct lexer {
  const char *raw_data;
  size_t size;
  size_t cursor;

  token_stream_t *token_stream;
};

// Static functions
/* -------------------------------------------------- */

static char peek(const lexer_t *ctx) {
  if (ctx->cursor >= ctx->size)
    return '\0';
  return ctx->raw_data[ctx->cursor];
}

static char consume(lexer_t *ctx) {
  char c = peek(ctx);
  if (c)
    ctx->cursor++;
  return c;
}

static lexer_error_e emit(lexer_t *ctx, token_t tok) {
  if (!ctx || !ctx->token_stream)
    return LEXER_ERROR_NULL_PARAMETER;

  token_stream_error_e err = token_stream_append(ctx->token_stream, tok);

  if (err == TOKEN_STREAM_ERROR_INSUFFICIENT_MEMORY)
    return LEXER_ERROR_OUT_OF_MEMORY;

  return LEXER_ERROR_NONE;
}

/* -------------------------------------------------- */

lexer_t *lexer_create(void) {
  lexer_t *ctx = malloc(sizeof *ctx);
  if (!ctx)
    return NULL;

  ctx->raw_data = NULL;
  ctx->size = 0;
  ctx->cursor = 0;
  ctx->token_stream = NULL;
  return ctx;
}

void lexer_free(lexer_t *ctx) { free(ctx); }

lexer_error_e lexer_set_raw_data(lexer_t *ctx, const char *raw_data,
                                 size_t size) {
  if (!ctx || !raw_data)
    return LEXER_ERROR_NULL_PARAMETER;

  ctx->raw_data = raw_data;
  ctx->size = size;
  ctx->cursor = 0;
  return LEXER_ERROR_NONE;
}

lexer_error_e lexer_set_token_stream(lexer_t *ctx, token_stream_t *stream) {
  if (!ctx || !stream)
    return LEXER_ERROR_NULL_PARAMETER;

  ctx->token_stream = stream;
  return LEXER_ERROR_NONE;
}

static int ident_eq(const char *str, size_t size, const char *kw) {
  size_t kw_size = strlen(kw);
  if (size != kw_size)
    return 0;

  return memcmp(str, kw, size) == 0;
}

lexer_error_e lexer_process_data(lexer_t *ctx) {
  if (!ctx || !ctx->token_stream)
    return LEXER_ERROR_NULL_PARAMETER;

  while (peek(ctx)) {
    char c = consume(ctx);

    if (isspace((unsigned char)c))
      continue;

    /* -------- operators & punctuation -------- */

    // clang-format off
    switch (c) {
    case '+': emit(ctx, (token_t){ .type = TOKEN_TYPE_PLUS }); continue;
    case '-': emit(ctx, (token_t){ .type = TOKEN_TYPE_MINUS }); continue;
    case '*': emit(ctx, (token_t){ .type = TOKEN_TYPE_STAR }); continue;
    case '/': emit(ctx, (token_t){ .type = TOKEN_TYPE_FSLASH }); continue;
    case '(': emit(ctx, (token_t){ .type = TOKEN_TYPE_LPARENTESIS }); continue;
    case ')': emit(ctx, (token_t){ .type = TOKEN_TYPE_RPARENTESIS }); continue;
    }
    // clang-format on

    if (isdigit((unsigned char)c) || c == '.') {
      ctx->cursor--; // rewind for strtod

      char *end;
      double value = strtod(ctx->raw_data + ctx->cursor, &end);

      size_t consumed = (size_t)(end - (ctx->raw_data + ctx->cursor));
      ctx->cursor += consumed;

      emit(ctx, (token_t){.type = TOKEN_TYPE_LITERAL, .as.number = value});
      continue;
    }

    if (isalpha((unsigned char)c) || c == '_') {
      size_t size = 1;
      const char *start = &ctx->raw_data[ctx->cursor - 1];

      while (isalnum((unsigned char)peek(ctx)) || peek(ctx) == '_') {
        consume(ctx);
        ++size;
      }
      token_t token = {.as.ident.size = size,
                       .as.ident.str = start,
                       .type = TOKEN_TYPE_IDENTIFIER};

      if (ident_eq(start, size, "fn"))
        token.type = TOKEN_TYPE_FN;
      else if (ident_eq(start, size, "extern"))
        token.type = TOKEN_TYPE_EXTERN;

      emit(ctx, token);

      continue;
    }

    return LEXER_ERROR_UNRECOGNIZED_CHAR;
  }

  emit(ctx, (token_t){.type = TOKEN_TYPE_EOF});
  return LEXER_ERROR_NONE;
}

void lexer_debug_print_tokens(const lexer_t *ctx) {
  size_t count = token_stream_size(ctx->token_stream);

  for (size_t i = 0; i < count; ++i) {
    token_t tok = token_stream_get(ctx->token_stream, i);

    printf("TOKEN %-2d ", tok.type);

    if (tok.type == TOKEN_TYPE_IDENTIFIER) {
      printf("ident=\"%.*s\"", tok.as.ident.size,
             tok.as.ident.str);
    } else if (tok.type == TOKEN_TYPE_LITERAL) {
      printf("number=%f", tok.as.number);
    }

    printf("\n");

    if (tok.type == TOKEN_TYPE_EOF)
      break;
  }
}
