#include "ast.h"
#include "ast_internal.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define AST_ARENA_DEFAULT_SIZE 8

static ast_error_e ast_resize_arena(ast_t *ctx, size_t new_size);

static void ast_debug_print_node(ast_t *ctx, node_id id, int indent);
static const char *node_type_to_str(node_type_e type);
static const char *node_op_to_str(node_operation_e op);

// ====---------------------------------------------------------------====

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

node_id ast_new_node(ast_t *ctx, node_t node) {
  assert(ctx);
  if (ctx->size == ctx->capacity - 1) {
    ast_error_e err = ast_resize_arena(ctx, ctx->capacity * 2);
    assert(err != AST_ERROR_NONE);
  }

  ctx->arena[ctx->size] = node;

  return ctx->size++;
}

node_t ast_get_node(ast_t *ctx, node_id index) {
  assert(ctx);
  assert(index < ctx->capacity);
  return ctx->arena[index];
}

static ast_error_e ast_resize_arena(ast_t *ctx, size_t new_size) {
  assert(ctx);
  ctx->arena = realloc(ctx->arena, new_size);
  if (!ctx->arena) {
    return AST_ERROR_FAILED_REALLOCATION;
  }
  return AST_ERROR_NONE;
}

void ast_debug_print(ast_t *ctx) {
  assert(ctx);

  if (ctx->root_node == 0) {
    printf("(empty AST)\n");
    return;
  }

  ast_debug_print_node(ctx, ctx->root_node, 0);
}

static void ast_debug_print_node(ast_t *ctx, node_id id, int indent) {
  if (id == 0) {
    printf("%*s<error-node>\n", indent * 2, "");
    return;
  }

  assert(id < ctx->size);

  node_t node = ctx->arena[id];

  printf("%*s[%zu] %s", indent * 2, "", id, node_type_to_str(node.node_type));

  switch (node.node_type) {
  case NODE_TYPE_LITERAL:
    printf(" value=%f\n", node.as.litteral);
    break;

  case NODE_TYPE_IDENTIFIER:
    printf(" name=\"%.*s\"\n", (int)node.as.identifier.size,
           node.as.identifier.str);
    break;

  case NODE_TYPE_BINARY:
    printf(" op=%s\n", node_op_to_str(node.as.binary.op));
    ast_debug_print_node(ctx, node.as.binary.l, indent + 1);
    ast_debug_print_node(ctx, node.as.binary.r, indent + 1);
    break;

  case NODE_TYPE_URINARY:
    printf(" op=%s\n", node_op_to_str(node.as.urinary.opp));
    ast_debug_print_node(ctx, node.as.urinary.node, indent + 1);
    break;

  case NODE_TYPE_CALL:
    printf(" args=%zu\n", node.as.call.size);
    ast_debug_print_node(ctx, node.as.call.calle, indent + 1);
    for (size_t i = 0; i < node.as.call.size; ++i) {
      ast_debug_print_node(ctx, node.as.call.args[i], indent + 1);
    }
    break;

  default:
    printf(" <unknown>\n");
    break;
  }
}

static const char *node_type_to_str(node_type_e type) {
  switch (type) {
  case NODE_TYPE_BINARY:
    return "BINARY";
  case NODE_TYPE_URINARY:
    return "URINARY";
  case NODE_TYPE_LITERAL:
    return "LITERAL";
  case NODE_TYPE_IDENTIFIER:
    return "IDENTIFIER";
  case NODE_TYPE_CALL:
    return "CALL";
  default:
    return "UNKNOWN";
  }
}

static const char *node_op_to_str(node_operation_e op) {
  switch (op) {
  case NODE_OP_SUM:
    return "+";
  case NODE_OP_RES:
    return "-";
  case NODE_OP_MUL:
    return "*";
  case NODE_OP_DIV:
    return "/";
  case NODE_OP_NEG:
    return "NEG";
  case NODE_OP_EQUAL:
    return "==";
  case NODE_OP_DIFF:
    return "!=";
  case NODE_OP_LESS_THAN:
    return "<";
  case NODE_OP_MORE:
    return ">";
  default:
    return "?";
  }
}
