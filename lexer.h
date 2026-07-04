#include <string.h>
#define MAX_TOKENS 256

typedef struct string {
  char *data;
  size_t count;
} string;

// tokenizador para entrada de símbolos
typedef unsigned int u32;
typedef u32 TokenType;
enum {
  TokenType_Error,
  TokenType_EOF,
  TokenType_Ident,
  TokenType_Number,
  TokenType_Plus,
  TokenType_Minus,
  TokenType_Star,
  TokenType_Slash,
  TokenType_Percent,
  TokenType_Caret,
  TokenType_Equals,
  TokenType_OpenParenthesis,
  TokenType_CloseParenthesis,
  TokenType_Comma,
};

typedef struct Token {
  TokenType tipo;
  string lexema;
} Token;

typedef struct Lexer {
  char *start;
  char *current;
} Lexer;
