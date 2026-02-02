#ifndef AST_INTERNAL_H
#define AST_INTERNAL_H

#include "ast.h"

struct ast {
  node_t *arena;
  size_t size;
  size_t capacity;
  node_id root_node;
};


#endif
