#include "codegen.h"
#include <llvm-c/Core.h>
#include <stdlib.h>

codegen_t *codegen_create(void) {
  codegen_t *ctx = malloc(sizeof *ctx);
  if (!ctx) {
    return NULL;
  }

  ctx->context = LLVMContextCreate();
  ctx->builder = LLVMCreateBuilderInContext(ctx->context);
  ctx->module =
      LLVMModuleCreateWithNameInContext("Testing-this-shi", ctx->context);

  return ctx;
}

void codegen_destroy(codegen_t *ctx) {
  if (ctx) {
    LLVMDisposeModule(ctx->module);
    LLVMDisposeBuilder(ctx->builder);
    LLVMContextDispose(ctx->context);

    free(ctx);
  }
}

void codegen_gen(codegen_t *ctx, ast_t *ast){}
