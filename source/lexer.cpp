#include "lexer.hpp"
#include "util.hpp" // IWYU pragma: export
#include <unordered_map>

// Constants

static inline const std::unordered_map<char, char> escapeCodeMap {
   {'a', '\a'}, {'b', '\b'}, {'t', '\t'}, {'n', '\n'}, {'v', '\v'}, {'f', '\f'},
   {'r', '\r'}, {'e', '\e'}, {'\\', '\\'}, {'\'', '\''}, {'"', '"'},
};

static inline const char * const tokenTypeStrings[] = {
   "Left Parenthesis", "Right Parenthesis", "Left Bracket", "Right Bracket",
   "Number", "Integer", "Character", "String", "Identifier", "End Of File",
};

// Lex the code

std::vector<Token> &Lexer::lex() {
   for (char ch = current(); index < code.size(); ch = advance()) {
      // Skip spaces and count new lines
      if (std::isspace(ch)) {
         line += (ch == '\n');
         continue;
      }

      // Lex comments
      else if (ch == ';') {
         while (advance() != '\n' && index < code.size());
         line += 1;
      }

      // Lex parentheses
      else if (ch == '(') {
         tokens.push_back(Token{"(", TokenType::leftParen, line});
      }

      else if (ch == ')') {
         tokens.push_back(Token{")", TokenType::rightParen, line});
      }

      else if (ch == '[') {
         tokens.push_back(Token{"[", TokenType::leftBracket, line});
      }

      else if (ch == ']') {
         tokens.push_back(Token{"]", TokenType::rightBracket, line});
      }

      // Lex characters
      else if (ch == '\'') {
         char character = advance();
         if (character == '\\')
            character = getEscapeCode(advance());
         tokens.push_back(Token{std::string(1, character), TokenType::character, line});  
      }

      // Lex strings
      else if (ch == '"') {
         std::string string;
         size_t originalLine = line;

         for (ch = advance(); index < code.size() && ch != '"'; ch = advance()) {
            line += (ch == '\n'); // Account for new lines in multi-line strings
            if (ch == '\\')
               ch = getEscapeCode(advance());
            string += ch;
         }
         assert(ch == '"', "Unterminated string at line {}.", originalLine);
         tokens.push_back(Token{string, TokenType::string, originalLine});
      }

      // Lex numbers (including negative)
      else if (std::isdigit(ch) || ch == '-') {
         std::string number;
         if (ch == '-' && !std::isdigit(advance())) {
            index -= 1;
            goto LEX_IDENTIFIER;
         } else if (ch == '-') {
            number += '-';
            ch = current(); // wtf dude
         }

         bool dot = false;
         bool lastDash = false;

         for (; index < code.size(); ch = advance()) {
            if (std::isdigit(ch)) {
               lastDash = false;
               number += ch;
            } else if (ch == '.') {
               if (dot) break;
               dot = true;
               number += '.';
            } else if (ch != '_') {
               break;
            }

            assert((ch != '_' && ch != '.') || !lastDash, "Expected number '{}' to not have two or more consecutive dashes at line {}.", number, line);
            lastDash = (ch == '_' || ch == '.');
         }

         assert(!lastDash, "Expected number '{}' to not end with a dash or a dot at line {}.", number, line);
         tokens.push_back(Token{number, (dot ? TokenType::number : TokenType::integer), line});
         index -= 1;
      }

      // Lex identifiers
      else LEX_IDENTIFIER: {
         std::string identifier;
         for (; index < code.size() && ch != '"' && ch != '\'' && ch != ';' && ch != '(' && ch != ')' && !std::isspace(ch); ch = advance())
            identifier += ch;
         tokens.push_back(Token{identifier, TokenType::identifier, line});
         index -= 1;
      }
   }

   tokens.push_back(Token{"EOF", TokenType::eof, line});
   return tokens;
}

// Helper functions

char Lexer::current() {
   return (index >= code.size() ? 0 : code.at(index));
}

char Lexer::advance() {
   index += 1;
   return current();
}

char Lexer::getEscapeCode(char character) {
   assert(escapeCodeMap.find(character) != escapeCodeMap.end(), "Unknown escape code '\\{}' at line {}.", character, line);
   return escapeCodeMap.at(character);
}

// Helper functions

const char *tokenTypeToString(TokenType type) {
   return tokenTypeStrings[static_cast<int>(type)];
}
