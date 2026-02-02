#ifndef PARSER_INTERNAL_H
#define PARSER_INTERNAL_H

#include "parser.h"
#include "token_stream.h"
#include <stddef.h>
#include <stdint.h>

#define bool uint8_t
#define false 0
#define true 1

struct parser {
  ast_t *ast;
  token_stream_t *token_stream;

  size_t cursor;
};

typedef enum precedence {
  LOWEST,
  EQUALITY,         // == or !=
  LESS_GREATER,     // < or >
  ADD_SUBRACT,      // + or -
  TIMES_DIVIDE_MOD, // * or /
  PREFIX,           // -X or !X
  CALL,             // myFunction(X)
} precedence_e;

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

#endif
