#include "asttree.hpp"
#include "environment.hpp"
#include "util.hpp" // IWYU pragma: export

// Constructors

Environment::Environment(Environment *parent): parent(parent) {}

// Global scope constructor
Environment::Environment(ASTTree &tree) {
   declare("nil", tree.emplace(BoolStmt{false}), 0);
   declare("t",   tree.emplace(BoolStmt{true}),  0);
}

// Variable functions

void Environment::declare(const std::string &identifier, StmtId value, int line) {
   variables[identifier] = value;
}

StmtId Environment::get(const std::string &identifier, int line) {
   return resolve(identifier, line).variables[identifier];
}

Environment &Environment::resolve(const std::string &identifier, int line) {
   if (variables.find(identifier) != variables.end()) {
      return *this;
   }

   assert(parent, "Variable '{}' does not exist in the given scope at line '{}'.", identifier, line);
   return parent->resolve(identifier, line);
}
