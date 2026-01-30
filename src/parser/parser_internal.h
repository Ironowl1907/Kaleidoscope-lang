#ifndef PARSER_INTERNAL_H
#define PARSER_INTERNAL_H

#include "parser.h"
#include <stddef.h>

struct parser {
  ast_t *ast;

  size_t next_token;
};

#endif
