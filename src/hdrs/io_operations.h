#ifndef IO_OPERATIONS_H
#define IO_OPERATIONS_H
// Included modules //
#include <filesystem>
// Define namespace aliases //
namespace ns_filesystem = std::filesystem;

// Function declarations //
bool readBinFile(filesys::path arg_file, ShellcodeStruct &shell_struct);

#endif