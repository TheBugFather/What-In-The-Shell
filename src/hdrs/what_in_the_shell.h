#ifndef WHAT_IN_THE_SHELL_H
#define WHAT_IN_THE_SHELL_H
// Included libraries //
#include <fstream>
// Header files //
#include "io_operations.h"
// Preprocessor constants //
#define MODE_MAC 1
#define MODE_IPV4 2
#define MODE_IPV6 3
// Define namespace aliases //
namespace filesys = std::filesystem;

// Data structures //
typedef struct ShellcodeStruct {
    filesys::path file_path;           // Path to the input binary file
    filesys::path file_name;           // Name of the input binary file
    int obfuscation_mode;              // The mode selected for shellcode obfuscation
    unsigned int bytes_read;           // Number of bytes read from passed in binary file
    unsigned char* in_shellcode_ptr;   // Pointer to the original shellcode
    unsigned char* pad_shellcode_ptr;  // Pointer to the shellcode after padding process
    unsigned int result_size;          // Number of obfuscated bytes generated from source
    filesys::path output_file;         // Name of source code exploit that is generated
    std::ofstream output_stream;       // Output file stream for writing generated code
} ShellcodeStruct;

#endif