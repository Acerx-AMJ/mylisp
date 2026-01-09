#ifndef MYLISP_LEXER_HPP
#define MYLISP_LEXER_HPP

#include <string>
#include <vector>

enum class TokenType: char {
   leftParen,
   rightParen,
   leftBracket,
   rightBracket,
   number,
   integer,
   character,
   string,
   identifier,
   eof,
};

struct Token {
   std::string lexeme;
   TokenType type;
   size_t line = 0;
};

// We put it into a struct so we do not have to copy the
// vector when returning it. If that wouldn't be the case,
// a simple function would be better for this
struct Lexer {
   Lexer(std::string &code): code(code) {}
   std::vector<Token> &lex();

   char current();
   char advance();
   char getEscapeCode(char character);

   // Members

   std::string &code;
   std::vector<Token> tokens;
   size_t index = 0;
   size_t line = 1;
};

// Utility functions

const char *tokenTypeToString(TokenType type);

#endif
