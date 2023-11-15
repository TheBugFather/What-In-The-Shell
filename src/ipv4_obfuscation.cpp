// Header files //
#include "hdrs/io_operations.h"
#include "hdrs/ipv4_obfuscation.h"


void ipv4Gen(uint32_t hex_ip_addr, char* output_buffer, size_t buffer_size) {
    /* Purpose -
     * Parameters:
     */
    unsigned char bytes[4];
    // Convert each hex byte into ipv4 format //
    bytes[0] = hex_ip_addr & 0xFF;
    bytes[1] = (hex_ip_addr >> 8) & 0xFF;
    bytes[2] = (hex_ip_addr >> 16) & 0xFF;
    bytes[3] = (hex_ip_addr >> 24) & 0xFF;
    // Copy each converted octet into buffer formatted as ip address //
    snprintf(output_buffer, buffer_size, "%d.%d.%d.%d", bytes[3], bytes[2], bytes[1], bytes[0]);
}


uint32_t ipv4HexGen(int byte_1, int byte_2, int byte_3, int byte_4) {
    /* Purpose -
     * Parameters:
     * Returns -
     */
    uint32_t hex_ipv4 = ((uint32_t)byte_1 << 24) | ((uint32_t)byte_2 << 16) | (byte_3 << 8) | byte_4;
    return hex_ipv4;
}


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
    char format_comma[64];
    char formatted_ip[32];
    uint32_t hex_ip_addr;

    // Iterate over the shellcode by increments of 4 //
    for (int iter = 0; iter <= shell_struct.result_size; iter += 4) {
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

        // Check if a newline is needed (every 6 entries) and not for the last entry
        if ((iter + 4) % 24 == 0 && iter + 4 < shell_struct.result_size) {
            writeOutputData(shell_struct, "\n\t");
        }
        num_elements++;
    }
    // Move the file stream pointer back one index to overwrite the last comma //
    shell_struct.output_stream.seekp(-1, std::ios_base::end);

    char payload_bracket_outer[] = "\n};\n";
    // Write the outer bracket of the obfuscated payload //
    writeOutputData(shell_struct, payload_bracket_outer);

    char shellcode_constants[68];
    char shellcode_preparse[58] = "#define ElementsNumber %d\n"
                                  "#define SizeOfShellcode %d\n\n";
    // Parse the formatted string in result buffer //
    std::snprintf(shellcode_constants, sizeof(shellcode_constants), shellcode_preparse,
                  num_elements, shell_struct.result_size);
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