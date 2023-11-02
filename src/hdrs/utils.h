#ifndef UTILS_H
#define UTILS_H
// Header files //
#include "hdrs/what_in_the_shell.h"

// Function declarations //
void nopPadding(ShellcodeStruct &shell_struct, unsigned int nop_count);
void printErr(const std::string& err_message);

#endif