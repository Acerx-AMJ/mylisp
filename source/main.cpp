#include "util.hpp"

int main(int argc, char *argv[]) {
   assert(argc == 2, "Expected argument count to be equal to 2, but it is {} instead.", argc);
   std::string code = argv[1];
   readIntoStringIfFileExists(code);
   
   printn(code);
   return 0;
}
