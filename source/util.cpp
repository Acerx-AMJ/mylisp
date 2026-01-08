#include "util.hpp"
#include <filesystem>
#include <fstream>

// File utility

void readIntoStringIfFileExists(std::string &code) {
   std::ifstream file (code);
   if (!file.is_open()) {
      return;
   }

   check(std::filesystem::path(code).extension() == ".mlsp", "Expected the file '{}' to have the extension '.mlsp'.", code);
   code.clear();

   std::string temporary;
   while (std::getline(file, temporary)) {
      code += temporary + '\n';
   }
}

// Format utility

size_t vformatCount(const char *base) {
   std::string string = base;
   size_t position = 0;
   size_t count = 0;

   // My beloved comma operators
   while (position = string.find("{}", position), position != std::string::npos) {
      count += 1;
      position += 2; // To skip any braces
   }
   return count;
}
