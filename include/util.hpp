#ifndef MYLISP_UTIL_HPP
#define MYLISP_UTIL_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// File utility

void readIntoStringIfFileExists(std::string &code);

// Format utility

template<typename T>
std::string toString(const T &value) {
   std::stringstream s;
   s << value;
   return s.str();
}

template<typename... Args>
std::string format(const char *base, const Args&...args) {
   std::string result = base;

   size_t position = 0;
   (( position = result.find("{}", position),
      position != std::string::npos
         ? result = result.replace(position, 2, toString(args))
         : result
   ), ...);
   return result;
}

template<typename T>
std::string vformat(const char *base, const std::vector<T> &args = {}) {
   std::string result = base;
   size_t position = 0;

   for (const T &arg : args) {
      position = result.find("{}", position);
      result = (position != std::string::npos ? result.replace(position, 2, toString(arg)) : result);
   }
   return result;
}

size_t vformatCount(const char *base);

// Print format utility

template<typename... Args>
void printf(const char *base, const Args &...args) {
   std::cout << format(base, args...);
}

template<typename T>
void vprintf(const char *base, const std::vector<T> &args = {}) {
   std::cout << vformat(base, args);
}

template<typename... Args>
void printfn(const char *base, const Args &...args) {
   std::cout << format(base, args...) << '\n';
}

template<typename T>
void vprintfn(const char *base, const std::vector<T> &args = {}) {
   std::cout << vformat(base, args) << '\n';
}

// Print utility

template<typename... Args>
void print(const Args &...args) {
   ((std::cout << std::boolalpha << args << ' '), ...);
}

template<typename T>
void vprint(const std::vector<T> &args = {}) {
   for (const T &arg : args) {
      std::cout << std::boolalpha << arg << ' ';
   }
   std::cout << '\n';
}

template<typename... Args>
void printn(const Args &...args) {
   print(args...);
   std::cout << '\n';
}

template<typename T>
void vprintn(const std::vector<T> &args = {}) {
   vprint(args);
   std::cout << '\n';
}

// Raise functions

template<typename... Args>
[[noreturn]] void raise(const char *error, const Args &...args) {
   #ifdef unix
   std::cout << "\e[31mAn Error Ocurred:\e[0m\n";
   #else
   std::cout << "An Error Ocurred:\n";
   #endif

   printfn(error, args...);
   std::exit(EXIT_FAILURE);
}

template<typename T>
[[noreturn]] void vraise(const char *error, const std::vector<T> &args = {}) {
   #ifdef unix
   std::cout << "\e[31mAn Error Ocurred:\e[0m\n";
   #else
   std::cout << "An Error Ocurred:\n";
   #endif

   vprintfn(error, args);
   std::exit(EXIT_FAILURE);
}

template<typename... Args>
void assert(bool condition, const char *error, const Args &...args) {
   if (!condition) {
      raise(error, args...);
   }
}

template<typename T>
void vassert(bool condition, const char *error, const std::vector<T> &args = {}) {
   if (!condition) {
      vraise(error, args);
   }
}

// Warn functions

template<typename... Args>
void warn(const char *error, const Args &...args) {
   #ifdef unix
   std::cout << "\e[33mWarning:\e[0m\n";
   #else
   std::cout << "Warning:\n";
   #endif
   printfn(error, args...);
   printn();
}

template<typename T>
void vwarn(const char *error, const std::vector<T> &args = {}) {
   #ifdef unix
   std::cout << "\e[33mWarning:\e[0m\n";
   #else
   std::cout << "Warning:\n";
   #endif
   vprintfn(error, args);
   printn();
}

template<typename... Args>
void check(bool condition, const char *error, const Args &...args) {
   if (!condition) {
      warn(error, args...);
   }
}

template<typename T>
void vcheck(bool condition, const char *error, const std::vector<T> &args = {}) {
   if (!condition) {
      vwarn(error, args);
   }
}

#endif
