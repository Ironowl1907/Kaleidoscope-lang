#ifndef AST_H
#define AST_H

#include "token_stream.h"
#include <stddef.h>

typedef size_t node_id;

typedef enum ast_error {
  AST_ERROR_NONE = 0,

  AST_ERROR_NULL_PARAMETER,
  AST_ERROR_FAILED_REALLOCATION,

} ast_error_e;

typedef enum {
  NODE_TYPE_BINARY,
  NODE_TYPE_URINARY,
  NODE_TYPE_LITERAL,
  NODE_TYPE_IDENTIFIER,
  NODE_TYPE_CALL,

} node_type_e;

typedef enum {
  NODE_OP_SUM,
  NODE_OP_RES,
  NODE_OP_MUL,
  NODE_OP_DIV,

  // Urinary
  NODE_OP_NEG,

  // Comparation
  NODE_OP_EQUAL,
  NODE_OP_DIFF,
  NODE_OP_LESS_THAN,
  NODE_OP_MORE,

} node_operation_e;

typedef struct {
  size_t size;
  size_t args_n;
  node_id *args;
  char *str;

} function_prototype_t;

typedef struct {
  function_prototype_t prototype;
  node_id body;
} function_definition_t;

typedef struct {
  node_id l, r;
  node_operation_e op;
} node_binary;

typedef struct {
  node_id node;
  node_operation_e opp;
} node_urinary;

typedef struct {
  node_id l, r;
  node_operation_e opp;
} node_comparation;

typedef struct {
  const char *str;
  size_t size;

} node_identifier;

typedef struct {
  node_id calle;
  node_id *args;
  size_t size;
} node_call;

typedef struct node {
  node_type_e node_type;

  union {
    node_binary binary;
    node_urinary urinary;
    node_comparation comparison;
    node_call call;
    node_identifier identifier;
    double litteral;
  } as;

} node_t;

typedef struct ast {
  node_t *arena;
  size_t size;
  size_t capacity;
  node_id root_node;
} ast_t;

ast_t *ast_create(void);
void ast_delete(ast_t *ctx);

node_id ast_new_node(ast_t *ctx, node_t node);

node_t ast_get_node(ast_t *ctx, node_id index);

void ast_debug_print(ast_t *ctx);

#endif
