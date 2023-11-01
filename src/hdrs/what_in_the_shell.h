#ifndef WHAT_IN_THE_SHELL_H
#define WHAT_IN_THE_SHELL_H

// Data structures //
typedef struct ShellcodeStruct {
    unsigned int bytesRead;
    void *srcShellcode;
    void *dstShellcode;
    unsigned int resultSize;
} ShellcodeStruct;
// Pointer alias data type for ShellcodeStruct //
typedef ShellcodeStruct* pShellcodeStruct;

#endif