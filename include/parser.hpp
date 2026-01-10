#ifndef MYLISP_PARSER_HPP
#define MYLISP_PARSER_HPP

#include "asttree.hpp"
#include "lexer.hpp"

struct Parser {
   Parser(std::vector<Token> &tokens): tokens(tokens) {}
   ASTTree &parse();
   StmtId parsePrimaryStmt();

   void advance();
   bool is(TokenType type) const;

   // Members

   std::vector<Token> &tokens;
   ASTTree tree;
   size_t index = 0;
};

#endif
