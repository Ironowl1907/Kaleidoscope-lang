#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
typedef enum parser_error {
  PARSER_ERROR_NONE = 0,

} parser_error_e;

typedef struct parser parser_t;

// clang-format off

parser_t *parser_create(ast_t * ast);
void parser_delete(parser_t *ctx);


parser_error_e parser_parse(void);

// clang-format on
#endif
