#include "parser.hpp"
#include "util.hpp" // IWYU pragma: export

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

   // Programs
   case TokenType::leftParen: {
      size_t originalLine = token.line;
      index += 1;
      ProgramStmt stmt {.line = originalLine};

      while (!is(TokenType::eof) && !is(TokenType::rightParen))
         stmt.program.push_back(parsePrimaryStmt());
      
      assert(is(TokenType::rightParen), "Unterminated scope at line {}.", originalLine);
      index += 1;
      return tree.emplace(std::move(stmt));
   } break;

   // Lists
   case TokenType::leftBracket: {
      size_t originalLine = token.line;
      index += 1;
      ListStmt stmt {.line = originalLine};

      while (!is(TokenType::eof) && !is(TokenType::rightBracket))
         stmt.list.push_back(parsePrimaryStmt());

      assert(is(TokenType::rightBracket), "Unterminated list at line {}.", originalLine);
      index += 1;
      return tree.emplace(std::move(stmt));
   } break;

   // Decimal numbers
   case TokenType::number: {
      NumberStmt stmt {.line = token.line};
      try {
         stmt.number = std::stold(token.lexeme);
      } catch (...) {
         raise("Number '{}' could not be converted to number at line {}. It may be too large.", token.lexeme, token.line);
      }

      index += 1;
      return tree.emplace(std::move(stmt));
   } break;

   // Integral numbers
   case TokenType::integer: {
      IntegerStmt stmt {.line = token.line};
      try {
         stmt.number = std::stol(token.lexeme);
      } catch (...) {
         raise("Integer '{}' could not be converted to integer at line {}. It may be too large.", token.lexeme, token.line);
      }

      index += 1;
      return tree.emplace(std::move(stmt));
   } break;

   // Characters
   case TokenType::character: {
      CharStmt stmt {.line = token.line};
      stmt.character = token.lexeme.front();

      index += 1;
      return tree.emplace(std::move(stmt));
   } break;

   // Strings
   case TokenType::string: {
      StringStmt stmt {.line = token.line};
      stmt.string = token.lexeme;

      index += 1;
      return tree.emplace(std::move(stmt));
   } break;

   // Identifiers
   case TokenType::identifier: {
      IdentifierStmt stmt {.line = token.line};
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
