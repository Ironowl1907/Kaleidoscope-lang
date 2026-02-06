#ifndef CODEGEN_H
#define CODEGEN_H
#include "ast.h"
#include <llvm-c/Core.h>
#include <llvm-c/Types.h>

typedef struct codegen {
  LLVMContextRef context;
  LLVMBuilderRef builder;
  LLVMModuleRef module;
  const char **values_names;
  LLVMValueRef *values_refs;

} codegen_t;

codegen_t *codegen_create(void);
void codegen_destroy(codegen_t *ctx);

void codegen_gen(codegen_t *ctx, ast_t *ast);

#endif
