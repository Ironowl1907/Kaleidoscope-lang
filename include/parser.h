#ifndef PARSER_H
#define PARSER_H

typedef enum parser_error {
  PARSER_ERROR_NONE = 0,

} parser_error_e;

typedef struct parser parser_t;

// clang-format off

parser_t *parser_create(void);
void parser_delete(parser_t *ctx);

void parser_set_ast(ast_t *ast);

parser_error_e parser_parse();

// clang-format on
#endif
