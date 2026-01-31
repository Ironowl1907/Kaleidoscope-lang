#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H

#include <stddef.h>
#include <stdint.h>

typedef struct token_stream token_stream_t;

typedef enum token_type {
  TOKEN_TYPE_PLUS,
  TOKEN_TYPE_MINUS,
  TOKEN_TYPE_FSLASH,
  TOKEN_TYPE_STAR,

  TOKEN_TYPE_LITERAL,
  TOKEN_TYPE_IDENTIFIER,

  TOKEN_TYPE_RPARENTESIS,
  TOKEN_TYPE_LPARENTESIS,

  TOKEN_TYPE_FN,
  TOKEN_TYPE_EXTERN,

  TOKEN_TYPE_EOF,
} token_type_e;

typedef enum token_stream_error {
  TOKEN_STREAM_ERROR_NONE = 0,
  TOKEN_STREAM_ERROR_NULL_PARAMETER,
  TOKEN_STREAM_ERROR_INSUFFICIENT_MEMORY,
} token_stream_error_e;

typedef struct {
  token_type_e type;

  union {
    double number;

    struct {
      const char *str;
      size_t size;
    } ident;
  } as;
} token_t;

// clang-format off
token_stream_t * token_stream_create(void);
void token_stream_delete(token_stream_t *ctx);

token_stream_error_e token_stream_append(token_stream_t *ctx, token_t token);
size_t token_stream_size(const token_stream_t *ctx);

token_t token_stream_get(const token_stream_t *ctx, size_t index);

#endif
