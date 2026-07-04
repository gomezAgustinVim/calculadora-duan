// codigo de referencia hecho con claude
// no se implementa
#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H

#include "lexer.h"

#define MAX_STACK 128

typedef struct {
  Token items[MAX_STACK];
  int top; // -1 = vacía
} TokenStack;

typedef struct {
  double items[MAX_STACK];
  int top;
} NumberStack;

void token_stack_push(TokenStack *s, Token t);
Token token_stack_pop(TokenStack *s);
Token token_stack_peek(TokenStack *s);
int token_stack_empty(TokenStack *s);

void number_stack_push(NumberStack *s, double n);
double number_stack_pop(NumberStack *s);

int a_rpn(Lexer *lexer, Token *salida, int max_salida);
double evaluar_rpn(Token *rpn, int n);

#endif
