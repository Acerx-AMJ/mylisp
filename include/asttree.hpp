#ifndef MYLISP_ASTTREE_HPP
#define MYLISP_ASTTREE_HPP

#include <string>
#include <variant>
#include <vector>

// Statements

using StmtId = size_t;

struct IdentifierStmt {
   std::string identifier;
   size_t line = 0;
};

struct NumberStmt {
   long double number = 0;
   size_t line = 0;
};

struct IntegerStmt {
   long int number = 0;
   size_t line = 0;
};

struct CharStmt {
   char character = 0;
   size_t line = 0;
};

struct BoolStmt {
   bool boolean = false;
   size_t line = 0;
};

struct StringStmt {
   std::string string;
   size_t line = 0;
};

struct ListStmt {
   std::vector<StmtId> list;
   size_t line = 0;
};

struct ProgramStmt {
   std::vector<StmtId> program;
   size_t line = 0;
};

using Stmt = std::variant<
   struct IdentifierStmt,
   struct NumberStmt,
   struct IntegerStmt,
   struct CharStmt,
   struct BoolStmt,
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

#endif
