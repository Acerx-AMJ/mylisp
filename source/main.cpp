#include "lexer.hpp"
#include "parser.hpp"
#include "util.hpp"

int main(int argc, char *argv[]) {
   assert(argc == 2, "Expected argument count to be equal to 2, but it is {} instead.", argc);
   std::string code = argv[1];
   readIntoStringIfFileExists(code);
   
   Lexer lexer (code);
   std::vector<Token> &tokens = lexer.lex();

   // // Print the tokens
   // for (const Token &token: tokens) {
   //    printfn("LEXEME: {}, TYPE: {}, LINE: {}.", token.lexeme, tokenTypeToString(token.type), token.line);
   // }

   Parser parser (tokens);
   ASTTree &tree = parser.parse();

   // Print the AST tree
   for (int i = 0; i < tree.program.program.size(); ++i) {
      printn(stmtToString(tree, tree[tree.program.program[i]]));
   }

   return 0;
}
