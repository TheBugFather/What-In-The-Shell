#ifndef IO_OPERATIONS_H
#define IO_OPERATIONS_H
// Included libraries //
#include <filesystem>
// Forward declarations //
struct ShellcodeStruct;
// Header files //
#include "what_in_the_shell.h"

// Define namespace aliases //
namespace filesys = std::filesystem;
// Function prototypes //
void openSourceFile(ShellcodeStruct& shell_struct);
bool readBinFile(const filesys::path& arg_file, ShellcodeStruct& shell_struct);

#endif