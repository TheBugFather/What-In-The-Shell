#ifndef IO_OPERATIONS_H
#define IO_OPERATIONS_H
// Included modules //
#include <filesystem>
// Header files //
#include "hdrs/what_in_the_shell.h"
// Define namespace aliases //
namespace filesys = std::filesystem;

// Function declarations //
bool readBinFile(filesys::path arg_file, ShellcodeStruct &shell_struct);

#endif