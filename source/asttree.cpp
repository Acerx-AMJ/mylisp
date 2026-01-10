#include "asttree.hpp"

// Print statement type

struct StmtStringVisitor {
   const char *operator () (const IdentifierStmt &kind) const {
      return "Identifier Statement";
   }

   const char *operator () (const NumberStmt &kind) const {
      return "Number Statement";
   }

   const char *operator () (const IntegerStmt &kind) const {
      return "Integer Statement";
   }

   const char *operator () (const CharStmt &kind) const {
      return "Character Statement";
   }

   const char *operator () (const BoolStmt &kind) const {
      return "Boolean Statement";
   }

   const char *operator () (const StringStmt &kind) const {
      return "String Statement";
   }

   const char *operator () (const ListStmt &kind) const {
      return "List Statement";
   }

   const char *operator () (const ProgramStmt &kind) const {
      return "Program";
   }
};

const char *stmtTypeToString(const Stmt &stmt) {
   return std::visit(StmtStringVisitor{}, stmt);
}

// Get statement string

struct StmtValueStringVisitor {
   const ASTTree &tree;
   StmtValueStringVisitor(const ASTTree &tree): tree(tree) {}

   std::string operator () (const IdentifierStmt &kind) const {
      return "Identifier: " + kind.identifier;
   }

   std::string operator () (const NumberStmt &kind) const {
      return "Number: " + std::to_string(kind.number);
   }

   std::string operator () (const IntegerStmt &kind) const {
      return "Integer: " + std::to_string(kind.number);
   }

   std::string operator () (const CharStmt &kind) const {
      return "Character: " + std::string(1, kind.character);
   }

   std::string operator () (const BoolStmt &kind) const {
      return "Boolean: " + std::string(kind.boolean ? "t" : "nil");
   }

   std::string operator () (const StringStmt &kind) const {
      return "String: " + kind.string;
   }

   std::string operator () (const ListStmt &kind) const {
      std::string string = "List: [ ";
      for (int i = 0; i < kind.list.size(); ++i)
         string += stmtToString(tree, tree[kind.list[i]]) + " ";
      return string + "]";
   }

   std::string operator () (const ProgramStmt &kind) const {
      std::string string = "Program: ( ";
      for (int i = 0; i < kind.program.size(); ++i)
         string += stmtToString(tree, tree[kind.program[i]]) + " ";
      return string + ")";
   }
};

std::string stmtToString(const ASTTree &tree, const Stmt &stmt) {
   return std::visit(StmtValueStringVisitor(tree), stmt);
}
