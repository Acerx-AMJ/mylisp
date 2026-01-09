#ifndef MYLISP_PARSER_HPP
#define MYLISP_PARSER_HPP

#include "lexer.hpp"
#include <string>
#include <variant>
#include <vector>

// Statements

using StmtId = size_t;

struct IdentifierStmt {
   std::string identifier;
};

struct NumberStmt {
   long double number = 0;
};

struct IntegerStmt {
   long int number = 0;
};

struct CharStmt {
   char character = 0;
};

struct StringStmt {
   std::string string;
};

struct ListStmt {
   std::vector<StmtId> list;
};

struct ProgramStmt {
   std::vector<StmtId> program;
};

using Stmt = std::variant<
   struct IdentifierStmt,
   struct NumberStmt,
   struct IntegerStmt,
   struct CharStmt,
   struct StringStmt,
   struct ListStmt,
   struct ProgramStmt
>;

// AST tree

struct ASTTree {
   ProgramStmt program;
   std::vector<Stmt> nodes;

   template<typename T>
   StmtId emplace(T &&stmt) {
      nodes.emplace_back(std::forward<T>(stmt));
      return nodes.size() - 1;
   }

   inline Stmt &operator [] (StmtId id) {
      return nodes[id];
   }

   inline const Stmt &operator [] (StmtId id) const {
      return nodes[id];
   }
};

// Statement functions

const char *stmtTypeToString(const Stmt &stmt);
std::string stmtToString(const ASTTree &tree, const Stmt &stmt);

// Parser

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
