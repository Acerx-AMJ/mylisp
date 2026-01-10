#ifndef MYLISP_ENVIRONMENT_HPP
#define MYLISP_ENVIRONMENT_HPP

#include <string>
#include <unordered_map>

using StmtId = size_t;

struct Environment {
   Environment(Environment *parent);
   Environment(struct ASTTree &tree);

   // Variable functions

   void declare(const std::string &identifier, StmtId value, int line);
   StmtId get(const std::string &identifier, int line);
   Environment &resolve(const std::string &identifier, int line);

   // Members

   std::unordered_map<std::string, StmtId> variables;
   Environment *parent;
};

#endif
