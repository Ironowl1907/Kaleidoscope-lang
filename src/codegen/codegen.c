#include "codegen.h"
#include "ast.h"
#include <llvm-c/Core.h>
#include <llvm-c/Types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *identifier_to_heap_str(node_identifier id) {
  char *s = malloc(id.size + 1);
  memcpy(s, id.str, id.size);
  s[id.size] = '\0';
  return s;
}

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

LLVMValueRef codegen_node(codegen_t *ctx, ast_t *ast, size_t index) {
  if (index == 0)
    return NULL;

  node_t node = ast_get_node(ctx, index);
  LLVMTypeRef double_type = LLVMDoubleTypeInContext(ctx->context);

  switch (node.node_type) {
  case NODE_TYPE_LITERAL:
    return LLVMConstReal(double_type, node.as.litteral);

  case NODE_TYPE_IDENTIFIER: {
    // TODO:
    // In a real compiler, you'd look this up in a Symbol Table
    // This example assumes identifiers refer to global variables or function
    // params
    char *name = identifier_to_heap_str(node.as.identifier);
    LLVMValueRef var = LLVMGetNamedGlobal(ctx->module, name);
    free(name);
    if (!var) {
      fprintf(stderr, "Error: Unknown identifier\n");
      return NULL;
    }
    return LLVMBuildLoad2(ctx->builder, double_type, var, "loadtmp");
  }

  case NODE_TYPE_BINARY: {
    LLVMValueRef L = codegen_node(ctx, ast, node.as.binary.l);
    LLVMValueRef R = codegen_node(ctx, ast, node.as.binary.r);
    if (!L || !R)
      return NULL;

    switch (node.as.binary.op) {
    case NODE_OP_SUM:
      return LLVMBuildFAdd(ctx->builder, L, R, "addtmp");
    case NODE_OP_RES:
      return LLVMBuildFSub(ctx->builder, L, R, "subtmp");
    case NODE_OP_MUL:
      return LLVMBuildFMul(ctx->builder, L, R, "multmp");
    case NODE_OP_DIV:
      return LLVMBuildFDiv(ctx->builder, L, R, "divtmp");

    case NODE_OP_EQUAL: {
      LLVMValueRef cmp =
          LLVMBuildFCmp(ctx->builder, LLVMRealOEQ, L, R, "cmptmp");
      return LLVMBuildUIToFP(ctx->builder, cmp, double_type, "booltmp");
    }
    case NODE_OP_LESS_THAN: {
      LLVMValueRef cmp =
          LLVMBuildFCmp(ctx->builder, LLVMRealOLT, L, R, "cmptmp");
      return LLVMBuildUIToFP(ctx->builder, cmp, double_type, "booltmp");
    }
    default:
      return NULL;
    }
  }

  case NODE_TYPE_URINARY: {
    LLVMValueRef val = codegen_node(ctx, ast, node.as.urinary.node);
    if (node.as.urinary.opp == NODE_OP_NEG) {
      return LLVMBuildFNeg(ctx->builder, val, "negtmp");
    }
    return val;
  }

  case NODE_TYPE_CALL: {
    // Find the function
    node_t callee_node = ast_get_node(ctx, node.as.call.calle);
    if (callee_node.node_type != NODE_TYPE_IDENTIFIER)
      return NULL;

    char *func_name = identifier_to_heap_str(callee_node.as.identifier);
    LLVMValueRef callee_func = LLVMGetNamedFunction(ctx->module, func_name);
    free(func_name);

    if (!callee_func)
      return NULL;

    // Handle arguments
    unsigned arg_count = (unsigned)node.as.call.size;
    LLVMValueRef *args_v = malloc(sizeof(LLVMValueRef) * arg_count);
    for (unsigned i = 0; i < arg_count; i++) {
      args_v[i] = codegen_node(ctx, ast, node.as.call.args[i]);
    }

    // In LLVM-C, we need the function type for LLVMBuildCall2
    LLVMTypeRef func_type = LLVMGetElementType(LLVMTypeOf(callee_func));
    LLVMValueRef call = LLVMBuildCall2(ctx->builder, func_type, callee_func,
                                       args_v, arg_count, "calltmp");

    free(args_v);
    return call;
  }

  default:
    return NULL;
  }
}
