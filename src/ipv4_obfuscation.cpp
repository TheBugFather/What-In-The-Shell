// Header files //
#include "hdrs/io_operations.h"
#include "hdrs/ipv4_obfuscation.h"


void ipv4ObfuscationHandler(ShellcodeStruct& shell_struct) {
    /* Purpose -
     * Parameters:
     * Returns -
     */
    char file_header[256] = "#include <Windows.h>\n"
                            "#include <stdio.h>\n"
                            "#include <Ip2string.h>\n"
                            "#pragma comment(lib, \"Ntdll.lib\")\n\n"
                            "#ifndef NT_SUCCESS\n"
                            "#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)\n"
                            "#endif\n";
    // Write the header of the deobfuscator source code //
    writeOutputData(shell_struct, file_header);
}