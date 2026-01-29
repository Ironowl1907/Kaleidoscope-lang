#include "token_stream.h"
#include "token_stream_interal.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define TOKEN_STREAM_INIT_SIZE 16

token_stream_t *token_stream_create(void) {
  token_stream_t *ctx = malloc(sizeof *ctx);
  if (!ctx)
    return NULL;

  ctx->capacity = TOKEN_STREAM_INIT_SIZE;

  ctx->arr = malloc(ctx->capacity * sizeof(token_t));

  if (!ctx->arr) {
    free(ctx);
    return NULL;
  }

  ctx->size = 0;
  return ctx;
}
void token_stream_delete(token_stream_t *ctx) {
  if (ctx) {
    free(ctx->arr);
    free(ctx);
  }
}

token_stream_error_e token_stream_append(token_stream_t *ctx, token_t token) {
  if (!ctx)
    return TOKEN_STREAM_ERROR_NULL_PARAMETER;

  if (ctx->size == ctx->capacity) {
    size_t new_capacity = ctx->capacity ? ctx->capacity * 2 : 1;
    token_stream_error_e err = token_stream_resize(ctx, new_capacity);
    if (err != TOKEN_STREAM_ERROR_NONE)
      return err;
  }
  ctx->arr[ctx->size++] = token;

  return TOKEN_STREAM_ERROR_NONE;
}

const token_t token_stream_get(const token_stream_t *ctx, size_t index) {
  if (!ctx || index >= ctx->size)
    return (token_t){0, TOKEN_TYPE_EOF};
  return ctx->arr[index];
}

token_stream_error_e token_stream_resize(token_stream_t *ctx, size_t new_size) {
  printf("Resising to:  %zu", new_size);
  if (!ctx)
    return TOKEN_STREAM_ERROR_NULL_PARAMETER;

  token_t *new_arr = realloc(ctx->arr, new_size * sizeof *ctx->arr);
  if (!new_arr)
    return TOKEN_STREAM_ERROR_INSUFFICIENT_MEMORY;

  ctx->arr = new_arr;
  ctx->capacity = new_size;

  return TOKEN_STREAM_ERROR_NONE;
}

size_t token_stream_size(const token_stream_t *ctx) { return ctx->size; }
