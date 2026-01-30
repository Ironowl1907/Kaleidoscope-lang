#ifndef AST_INTERNAL_H
#define AST_INTERNAL_H

#include "ast.h"

struct ast {
  node_t *arena;
  size_t size;
  size_t capacity;
  node_id root_node;
};

ast_error_e ast_resize_arena(ast_t *ctx, size_t new_size);

#endif
