#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "token_stream.h"
typedef enum parser_error {

  PARSER_ERROR_NONE = 0,

  PARSER_ERROR_SINTAX_ERROR = 0,
} parser_error_e;

typedef struct parser parser_t;

parser_t *parser_create(ast_t *ast, token_stream_t *ts);
void parser_delete(parser_t *ctx);

parser_error_e parser_parse(parser_t *ctx);

#endif
