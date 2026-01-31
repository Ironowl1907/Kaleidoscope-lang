#ifndef PARSER_INTERNAL_H
#define PARSER_INTERNAL_H

#include "parser.h"
#include "token_stream.h"
#include <stddef.h>

#define bool uint8_t
#define false 0
#define true 1

struct parser {
  ast_t *ast;
  token_stream_t *token_stream;

  size_t cursor;
};

static token_t consume(parser_t *ctx);
static token_t peek(parser_t *ctx);

static node_id parse_primary(parser_t *ctx);
static node_id parse_number_expr(parser_t *ctx);
static node_id parse_paren_expr(parser_t *ctx);
static node_id parse_identifier_expr(parser_t *ctx);

#endif
