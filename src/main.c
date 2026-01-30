#include "lexer.h"
#include "token_stream.h"
#include <stdio.h>
#include <string.h>

int main() {
  char *data = "fn (a) a+1";
  token_stream_t *token_stream = token_stream_create();
  lexer_t *lexer = lexer_create();

  lexer_set_raw_data(lexer, data, strlen(data));
  lexer_set_token_stream(lexer, token_stream);

  lexer_error_e error = lexer_process_data(lexer);
  if (error != LEXER_ERROR_NONE) {
    printf("Lexer error %d\n", error);
  }

  lexer_debug_print_tokens(lexer);

  lexer_free(lexer);
  token_stream_delete(token_stream);
  return 0;
}
