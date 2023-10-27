#ifndef ARG_VALIDATE_H
#define ARG_VALIDATE_H

// Preprocessor constants //
#define MIN_MODE 1
#define MAX_MODE 3;
// Function declarations //
filesystem validatePayloadFile(const char *arg_file_path);
int validateObfuscationMode(const char *arg_obfuscation_mode);

#endif