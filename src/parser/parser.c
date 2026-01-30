#include "parser.h"
#include "ast.h"
#include "parser_internal.h"
#include <assert.h>
#include <stdlib.h>

static node_t parser_consume(parser_t *ctx) {
  assert(ctx);
  return ast_get_node(ctx->ast, ctx->cursor++);
}
static node_t parser_peek(parser_t *ctx) {
  assert(ctx);
  return ast_get_node(ctx->ast, ctx->cursor);
}

parser_t *parser_create(ast_t *ast) {
  parser_t *parser = malloc(sizeof *parser);
  if (!parser) {
    return NULL;
  }
  parser->ast = ast;
  parser->cursor = 0;

  return parser;
}
void parser_delete(parser_t *ctx) {
  if (ctx) {
    free(ctx);
  }
}

parser_error_e parser_parse(void);
