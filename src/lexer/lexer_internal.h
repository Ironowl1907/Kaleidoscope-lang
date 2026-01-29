#include "lexer.h"
#include "token_stream.h"

struct lexer {
  char raw_data[LEXER_RAW_BUFFER_SIZE];
  token_stream_t *token_stream;

  uint32_t cursor;
  uint32_t token_count;
};

// clang-format off
static char lexer_consume_char(lexer_t *ctx);
static char lexer_peek_char(const lexer_t *ctx);
lexer_error_e lexer_append_token(lexer_t *ctx, token_type_e type, const char *data);
