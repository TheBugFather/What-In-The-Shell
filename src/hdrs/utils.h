#ifndef UTILS_H
#define UTILS_H
// Included modules //
#include <filesystem>
// Forward declarations //
struct ShellcodeStruct;
// Header files //
#include "what_in_the_shell.h"

// Function prototypes //
void filenameExtract(const char* path_arg, ShellcodeStruct& shell_struct);
void nopPaddingCopy(ShellcodeStruct& shell_struct, const unsigned int nop_count);
void printErr(const std::string& err_message);
void regularCopy(ShellcodeStruct& shell_struct);

#endif