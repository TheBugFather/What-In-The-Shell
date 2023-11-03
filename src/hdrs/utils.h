#ifndef UTILS_H
#define UTILS_H
// Header files //
#include "hdrs/what_in_the_shell.h"

// Function declarations //
void nopPaddingCopy(ShellcodeStruct &shell_struct, const unsigned int nop_count);
void printErr(const std::string& err_message);
void shellCopy(const ShellcodeStruct &shell_struct);

#endif