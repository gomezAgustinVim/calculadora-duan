// codigo de referencia hecho con claude
// no se implementa
#include "shunting_yard.h"
#include <stdio.h>
#include <stdlib.h>

void token_stack_push(TokenStack *s, Token t) { s->items[++s->top] = t; }
Token token_stack_pop(TokenStack *s) { return s->items[s->top--]; }
Token token_stack_peek(TokenStack *s) { return s->items[s->top]; }
int token_stack_empty(TokenStack *s) { return s->top == -1; }

void number_stack_push(NumberStack *s, double n) { s->items[++s->top] = n; }
double number_stack_pop(NumberStack *s) { return s->items[s->top--]; }

static int precedencia(TokenType tipo) {
  switch (tipo) {
  case TokenType_Plus:
  case TokenType_Minus:
    return 1;
  case TokenType_Star:
  case TokenType_Slash:
  case TokenType_Percent:
    return 2;
  case TokenType_Caret:
    return 3;
  default:
    return 0;
  }
}

// derecha-asociativo solo para '^'
static int es_derecha_asociativo(TokenType tipo) {
  return tipo == TokenType_Caret;
}

static int es_operador(TokenType tipo) {
  return tipo == TokenType_Plus || tipo == TokenType_Minus ||
         tipo == TokenType_Star || tipo == TokenType_Slash ||
         tipo == TokenType_Percent || tipo == TokenType_Caret;
}

int a_rpn(Lexer *lexer, Token *salida, int max_salida) {
  TokenStack operadores = {.top = -1};
  int n_salida = 0;
  Token token;

  while ((token = siguiente_token(lexer)).tipo != TokenType_EOF) {
    if (n_salida >= max_salida) {
      fprintf(stderr, "Expresión demasiado larga\n");
      return -1;
    }
    if (token.tipo == TokenType_Number) {
      salida[n_salida++] = token;
    } else if (es_operador(token.tipo)) {
      while (!token_stack_empty(&operadores) &&
             token_stack_peek(&operadores).tipo != TokenType_OpenParenthesis &&
             (precedencia(token_stack_peek(&operadores).tipo) >
                  precedencia(token.tipo) ||
              (precedencia(token_stack_peek(&operadores).tipo) ==
                   precedencia(token.tipo) &&
               !es_derecha_asociativo(token.tipo)))) {
        salida[n_salida++] = token_stack_pop(&operadores);
      }
      token_stack_push(&operadores, token);
    } else if (token.tipo == TokenType_OpenParenthesis) {
      token_stack_push(&operadores, token);
    } else if (token.tipo == TokenType_CloseParenthesis) {
      int encontro_paren = 0;
      while (!token_stack_empty(&operadores)) {
        if (token_stack_peek(&operadores).tipo == TokenType_OpenParenthesis) {
          token_stack_pop(&operadores);
          encontro_paren = 1;
          break;
        }
        salida[n_salida++] = token_stack_pop(&operadores);
      }
      if (!encontro_paren) {
        fprintf(stderr, "Paréntesis desbalanceados\n");
        return -1;
      }
    } else if (token.tipo == TokenType_Error) {
      fprintf(stderr, "Token inválido: '%.*s'\n", (int)token.lexema.count,
              token.lexema.data);
      return -1;
    }
  }

  while (!token_stack_empty(&operadores)) {
    Token op = token_stack_pop(&operadores);
    if (op.tipo == TokenType_OpenParenthesis) {
      fprintf(stderr, "Paréntesis desbalanceados\n");
      return -1;
    }
    salida[n_salida++] = op;
  }

  return n_salida;
}

double evaluar_rpn(Token *rpn, int n) {
  NumberStack pila = {.top = -1};

  for (int i = 0; i < n; i++) {
    Token t = rpn[i];
    if (t.tipo == TokenType_Number) {
      double valor = strtod(t.lexema.data, NULL);
      number_stack_push(&pila, valor);
    } else if (es_operador(t.tipo)) {
      if (pila.top < 1) {
        fprintf(stderr, "Expresión inválida: faltan operandos\n");
        exit(1);
      }
      double b = number_stack_pop(&pila);
      double a = number_stack_pop(&pila);
      double r;
      switch (t.tipo) {
      case TokenType_Plus:
        r = a + b;
        break;
      case TokenType_Minus:
        r = a - b;
        break;
      case TokenType_Star:
        r = a * b;
        break;
      case TokenType_Slash:
        r = a / b;
        break;
      case TokenType_Percent:
        r = (double)((long)a % (long)b);
        break;
      case TokenType_Caret:
        r = pow(a, b);
        break;
      default:
        r = 0;
      }
      number_stack_push(&pila, r);
    }
  }

  if (pila.top != 0) {
    fprintf(stderr, "Expresión inválida: sobran operandos\n");
    exit(1);
  }

  return number_stack_pop(&pila);
}
