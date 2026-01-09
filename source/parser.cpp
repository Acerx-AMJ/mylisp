#include "parser.hpp"
#include "util.hpp" // IWYU pragma: export

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
      return "Number: " + toString(kind.number);
   }

   std::string operator () (const IntegerStmt &kind) const {
      return "Integer: " + toString(kind.number);
   }

   std::string operator () (const CharStmt &kind) const {
      return "Character: " + toString(kind.character);
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

// Parse functions

ASTTree &Parser::parse() {
   while (!is(TokenType::eof)) {
      tree.program.program.push_back(parsePrimaryStmt());
   }
   return tree;
}

StmtId Parser::parsePrimaryStmt() {
   Token &token = tokens[index];
   
   switch (token.type) {
   case TokenType::leftParen: {
      size_t originalLine = token.line;
      index += 1;
      ProgramStmt stmt;

      while (!is(TokenType::eof) && !is(TokenType::rightParen))
         stmt.program.push_back(parsePrimaryStmt());
      
      assert(is(TokenType::rightParen), "Unterminated scope at line {}.", originalLine);
      index += 1;
      return tree.emplace(std::move(stmt));
   } break;

   case TokenType::leftBracket: {
      size_t originalLine = token.line;
      index += 1;
      ListStmt stmt;

      while (!is(TokenType::eof) && !is(TokenType::rightBracket))
         stmt.list.push_back(parsePrimaryStmt());

      assert(is(TokenType::rightBracket), "Unterminated list at line {}.", originalLine);
      index += 1;
      return tree.emplace(std::move(stmt));
   } break;

   case TokenType::number: {
      NumberStmt stmt;
      try {
         stmt.number = std::stold(token.lexeme);
      } catch (...) {
         raise("Number '{}' could not be converted to number at line {}. It may be too large.", token.lexeme, token.line);
      }

      index += 1;
      return tree.emplace(std::move(stmt));
   } break;

   case TokenType::integer: {
      IntegerStmt stmt;
      try {
         stmt.number = std::stol(token.lexeme);
      } catch (...) {
         raise("Integer '{}' could not be converted to integer at line {}. It may be too large.", token.lexeme, token.line);
      }

      index += 1;
      return tree.emplace(std::move(stmt));
   } break;

   case TokenType::character: {
      CharStmt stmt;
      stmt.character = token.lexeme.front();

      index += 1;
      return tree.emplace(std::move(stmt));
   } break;

   case TokenType::string: {
      StringStmt stmt;
      stmt.string = token.lexeme;

      index += 1;
      return tree.emplace(std::move(stmt));
   } break;

   case TokenType::identifier: {
      IdentifierStmt stmt;
      stmt.identifier = token.lexeme;

      index += 1;
      return tree.emplace(std::move(stmt));
   } break;

   default:
      raise("Unexpected token '{}' while parsing at line {}.", tokenTypeToString(token.type), token.line);
   }
}

// Parser utility functions

void Parser::advance() {
   // Branchless increment with logic. Original would look like so:
   // if (index + 1 < tokens.size()) index += 1;
   index += (index + 1 < tokens.size());
}

bool Parser::is(TokenType type) const {
   return index < tokens.size() && tokens[index].type == type;
}
