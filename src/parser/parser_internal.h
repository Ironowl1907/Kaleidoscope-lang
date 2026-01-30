#ifndef PARSER_INTERNAL_H
#define PARSER_INTERNAL_H

#include "parser.h"
#include <stddef.h>

struct parser {
  ast_t *ast;

  size_t cursor;
};

static node_t parser_consume(parser_t *ctx);
static node_t parser_peek(parser_t *ctx);

#endif
