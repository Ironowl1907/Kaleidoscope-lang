#include "token_stream.h"
#include <stdio.h>

int main() {
  token_stream_t *token_stream = token_stream_create();

  token_stream_delete(token_stream);
  return 0;
}
