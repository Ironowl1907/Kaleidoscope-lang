#include "parser.h"
#include "ast.h"
#include "parser_internal.h"
#include "token_stream.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static token_t consume(parser_t *ctx) {
  assert(ctx);
  return token_stream_get(ctx->token_stream, ctx->cursor++);
}
static token_t peek(parser_t *ctx) {
  assert(ctx);
  return token_stream_get(ctx->token_stream, ctx->cursor);
}

static node_id parse_primary(parser_t *ctx) {}

static node_id parse_number_expr(parser_t *ctx) {
  return ast_new_node(ctx->ast, (node_t){
                                    .node_type = NODE_TYPE_LITERAL,
                                    .as.litteral = consume(ctx).as.number,
                                });
}

static node_id parse_paren_expr(parser_t *ctx) {
  consume(ctx); // Eat  '('
  node_id expr = parse_expresion(ctx);
  if (!expr)
    return 0;
  if (peek(ctx).type != TOKEN_TYPE_RPARENTESIS) {
    printf("Expected token ')' at position %zu", ctx->cursor);
    return 0;
  }
  consume(ctx); // Eat ')'

  return expr;
}

static node_id parse_identifier_expr(parser_t *ctx) {


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

parser_error_e parser_parse(void) {}
