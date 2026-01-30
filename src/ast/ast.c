#include "ast.h"
#include "ast_internal.h"
#include <assert.h>
#include <stdlib.h>

#define AST_ARENA_DEFAULT_SIZE 8

ast_t *ast_create(void) {
  ast_t *ast = malloc(sizeof *ast);
  if (!ast) {
    return NULL;
  }

  ast->arena = malloc(AST_ARENA_DEFAULT_SIZE * sizeof *ast->arena);
  if (!ast->arena) {
    free(ast);
    return NULL;
  }

  ast->size = 1; // Zero reserved for errors
  ast->capacity = AST_ARENA_DEFAULT_SIZE;
  ast->root_node = 0;

  return ast;
}
void ast_delete(ast_t *ctx) {
  if (ctx) {
    if (ctx->arena) {
      free(ctx->arena);
      free(ctx);
      return;
    }
    free(ctx);
  }
}

node_id ast_new_node(ast_t *ctx, node_t node, ast_error_e *error) {
  if (!ctx) {
    *error = AST_ERROR_NULL_PARAMETER;
    return 0;
  }

  if (ctx->size == ctx->capacity - 1) {
    ast_error_e err = ast_resize_arena(ctx, ctx->capacity * 2);
    if (err != AST_ERROR_NONE) {
      *error = err;
      return 0;
    }
  }

  ctx->arena[ctx->size] = node;

  return ctx->size++;
}

node_t ast_get_node(ast_t *ctx, node_id index) {
  assert(ctx);
  assert(index < ctx->capacity);
  return ctx->arena[index];
}

ast_error_e ast_resize_arena(ast_t *ctx, size_t new_size) {
  assert(ctx);
  ctx->arena = realloc(ctx->arena, new_size);
  if (!ctx->arena) {
    return AST_ERROR_FAILED_REALLOCATION;
  }
}
