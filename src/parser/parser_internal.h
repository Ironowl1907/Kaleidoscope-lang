#ifndef PARSER_INTERNAL_H
#define PARSER_INTERNAL_H

#include "parser.h"
#include "token_stream.h"
#include <stddef.h>
#include <stdint.h>

struct parser {
  ast_t *ast;
  token_stream_t *token_stream;

  size_t cursor;
};


#endif
