#include "parser.h"
#include "ast.h"
#include "parser_internal.h"
#include "token_stream.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint8_t get_token_precedence(token_type_e type);

static token_t consume(parser_t *ctx);
static token_t peek(parser_t *ctx);

static node_id parse_primary(parser_t *ctx);
static node_id parse_number_expr(parser_t *ctx);
static node_id parse_paren_expr(parser_t *ctx);
static node_id parse_identifier_expr(parser_t *ctx);
static node_id *parse_func_args(parser_t *ctx, size_t *size);

static node_id parse_expresion(parser_t *ctx, uint32_t r_bp);

static node_operation_e token_to_binary_op(token_type_e type);
static node_id parse_protototype(parser_t *ctx);

// ====---------------------------------------------------------------====

static token_t consume(parser_t *ctx) {
  assert(ctx);
  return token_stream_get(ctx->token_stream, ctx->cursor++);
}
static token_t peek(parser_t *ctx) {
  assert(ctx);
  return token_stream_get(ctx->token_stream, ctx->cursor);
}

static node_id parse_primary(parser_t *ctx) {
  switch (peek(ctx).type) {
  case TOKEN_TYPE_LITERAL:
    return parse_number_expr(ctx);
  case TOKEN_TYPE_IDENTIFIER:
    return parse_identifier_expr(ctx);
  case TOKEN_TYPE_LPARENTESIS:
    return parse_paren_expr(ctx);
  default:
    printf("Unknown token when expecting expresion at: %zu\n", ctx->cursor);
    return 0;
    break;
  }
}

static node_id parse_number_expr(parser_t *ctx) {
  return ast_new_node(ctx->ast, (node_t){
                                    .node_type = NODE_TYPE_LITERAL,
                                    .as.litteral = consume(ctx).as.number,
                                });
}

static node_id parse_paren_expr(parser_t *ctx) {
  consume(ctx); // Eat  '('
  node_id expr = parse_expresion(ctx, 0);
  if (!expr)
    return 0;
  if (peek(ctx).type != TOKEN_TYPE_RPARENTESIS) {
    printf("Expected token ')' at position %zu\n", ctx->cursor);
    return 0;
  }
  consume(ctx); // Eat ')'

  return expr;
}

static node_id parse_identifier_expr(parser_t *ctx) {
  const char *ident_str = peek(ctx).as.ident.str;
  size_t str_size = peek(ctx).as.ident.size;

  consume(ctx);

  if (peek(ctx).type != TOKEN_TYPE_LPARENTESIS) {
    return ast_new_node(ctx->ast, (node_t){.node_type = NODE_TYPE_IDENTIFIER,
                                           .as.identifier.size = str_size,
                                           .as.identifier.str = ident_str});
  }

  node_id *args = NULL;
  size_t size = 0;
  consume(ctx); // Eat '('
  if (peek(ctx).type == TOKEN_TYPE_RPARENTESIS) {
    consume(ctx); // ')'
  } else {
    args = parse_func_args(ctx, &size);
    consume(ctx); // Eat ')'
  }

  node_id calle = ast_new_node(
      ctx->ast,
      (node_t){.node_type = NODE_TYPE_IDENTIFIER,
               .as.identifier = {.size = str_size, .str = ident_str}});
  return ast_new_node(
      ctx->ast,
      (node_t){.node_type = NODE_TYPE_CALL,
               .as.call = {.calle = calle, .args = args, .size = size}});
}

static node_id *parse_func_args(parser_t *ctx, size_t *size) {
  size_t arr_reserved_size = 4;
  *size = 0;
  node_id *args = malloc(sizeof *args * arr_reserved_size);

  for (;;) {
    node_id arg = parse_expresion(ctx, 0);
    if (!arg) {
      return NULL;
    }
    args[(*size)++] = arg;

    if (*size == arr_reserved_size) {
      arr_reserved_size *= 2;
      args = realloc(args, sizeof *args * arr_reserved_size);
    }

    if (peek(ctx).type == TOKEN_TYPE_RPARENTESIS)
      break;

    if (peek(ctx).type != TOKEN_TYPE_COMMA) {
      printf("Expected ',' at position %zu\n", ctx->cursor);
      return NULL;
    }
    consume(ctx); // Eat ','
  }

  return args;
}

parser_t *parser_create(ast_t *ast, token_stream_t *ts) {
  parser_t *parser = malloc(sizeof *parser);
  if (!parser || !ast || !ts) {
    return NULL;
  }

  parser->ast = ast;
  parser->cursor = 0;
  parser->token_stream = ts;

  return parser;
}
void parser_delete(parser_t *ctx) {
  if (ctx) {
    free(ctx);
  }
}

static node_id parse_expresion(parser_t *ctx, uint32_t r_bp) {
  node_id lhs = parse_primary(ctx);
  if (!lhs)
    return 0;

  for (;;) {
    token_t tk = peek(ctx);
    uint32_t lbp = get_token_precedence(tk.type);

    if (lbp <= r_bp)
      break;

    consume(ctx); // eat operator

    node_id rhs = parse_expresion(ctx, lbp);
    if (!rhs)
      return 0;

    lhs = ast_new_node(ctx->ast, (node_t){
                                     .node_type = NODE_TYPE_BINARY,
                                     .as.binary =
                                         {
                                             .op = token_to_binary_op(tk.type),
                                             .l = lhs,
                                             .r = rhs,
                                         },
                                 });
  }

  return lhs;
}

static node_id parse_protototype(parser_t *ctx) {
  if (peek(ctx).type != TOKEN_TYPE_IDENTIFIER) {
    printf("Expected function name in prototype, position: %zu", ctx->cursor);
    return 0;
  }

  identifier_t ident = consume(ctx).as.ident;

  if (peek(ctx).type != TOKEN_TYPE_LPARENTESIS) {
    printf("Expected '(', position: %zu", ctx->cursor);
    return 0;
  }

  node_id *args;
  size_t size = 0;
  size_t args_reserved_size = 4;

  args = malloc(sizeof *args * args_reserved_size);

  while (peek(ctx).type == TOKEN_TYPE_IDENTIFIER) {
    if (size == args_reserved_size) {
      args_reserved_size *= 2;
      args = realloc(args, sizeof *args * args_reserved_size);
    }
    args[size++] = ast_new_node(
        ctx->ast, (node_t){.node_type = NODE_TYPE_IDENTIFIER,
                           .as.identifier.size = peek(ctx).as.ident.size,
                           .as.identifier.str = peek(ctx).as.ident.str});
    consume(ctx);
    if (peek(ctx).type == TOKEN_TYPE_RPARENTESIS)
      break;
    if (peek(ctx).type == TOKEN_TYPE_COMMA) {
      consume(ctx);
    }
  }
}

parser_error_e parser_parse(void) {}

static uint8_t get_token_precedence(token_type_e type) {
  switch (type) {
  case TOKEN_TYPE_PLUS:
  case TOKEN_TYPE_MINUS:
    return 10;

  case TOKEN_TYPE_STAR:
  case TOKEN_TYPE_FSLASH:
    return 20;

  default:
    return 0;
  }
}

static node_operation_e token_to_binary_op(token_type_e type) {
  switch (type) {
  case TOKEN_TYPE_PLUS:
    return NODE_OP_SUM;
  case TOKEN_TYPE_MINUS:
    return NODE_OP_RES;
  case TOKEN_TYPE_STAR:
    return NODE_OP_MUL;
  case TOKEN_TYPE_FSLASH:
    return NODE_OP_DIV;
  default:
    // This should never happen if precedence logic is correct
    assert(0 && "Invalid token for binary operation");
    return NODE_OP_SUM;
  }
}
