#ifndef ARG_VALIDATE_H
#define ARG_VALIDATE_H
// Header files //
#include "io_operations.h"
// Preprocessor constants //
#define MIN_MODE 1
#define MAX_MODE 3

// Function declarations //
ns_filesystem::path validatePayloadFile(const char* arg_file_path);
int validateObfuscationMode(const char* arg_obfuscation_mode);

#endif