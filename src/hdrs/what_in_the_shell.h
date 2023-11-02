#ifndef WHAT_IN_THE_SHELL_H
#define WHAT_IN_THE_SHELL_H
// Header files //
// Preprocessor constants //
#define MODE_MAC 1
#define MODE_IPV4 2
#define MODE_IPV6 3

// Data structures //
typedef struct ShellcodeStruct {
    unsigned int bytes_read;    // Number of bytes read from passed in binary file
    void* src_shellcode_ptr;    // Pointer to the original shellcode
    void* dst_shellcode_ptr;    // Pointer to the obfuscated shellcode
    unsigned int result_size;   // Number of obfuscated bytes generated from source
    std::string output_file;    // Name of source code exploit that is generated
} ShellcodeStruct;
// Pointer alias data type for ShellcodeStruct //
typedef ShellcodeStruct* pShellcodeStruct;

#endif