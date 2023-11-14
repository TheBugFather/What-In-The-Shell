// Included libraries //
#include <iostream>
// Header files //
#include "hdrs/arg_validate.h"
#include "hdrs/ipv4_obfuscation.h"
#include "hdrs/ipv6_obfuscation.h"
#include "hdrs/mac_obfuscation.h"
#include "hdrs/utils.h"
#include "hdrs/what_in_the_shell.h"
// Local Macros //
#define LINE_COUNT 90
#define MAX_ARGS 3


// Function prototypes //
int usageDisplay(const filesys::path* program_name, const int exit_code);


void shellCopyHandler(ShellcodeStruct& shell_struct, const unsigned int divisibility) {
    /* Purpose -
     * Parameters:
     */
    std::string obfuscator;

    // Execute logic based on selected obfuscation mode //
    switch (shell_struct.obfuscation_mode) {
        case MODE_IPV4:
            obfuscator = "IPv4 address";
            shell_struct.output_file = "ipv4_src_out.cpp";
            break;
        case MODE_IPV6:
            obfuscator = "IPv6 address";
            shell_struct.output_file = "ipv6_src_out.cpp";
            break;
        case MODE_MAC:
            obfuscator = "MAC address";
            shell_struct.output_file = "mac_src_out.cpp";
            break;
        default:
            // Print error and return error code //
            printErr("Error occurred checking obfuscation mode, this logic should not happen");
            std::exit(-4);
    }

    // If shellcode is not divisible by 6 (needs NOP padding) //
    if (!shell_struct.bytes_read % divisibility == 0) {
        std::cout << "[+] The shellcode is not divisible by " << divisibility << " based on"
                     "passed in " << obfuscator << " .. padding NOP slides" << std::endl;
        // Allocate shellcode malloc buffer with appended NOP slide padding //
        nopPaddingCopy(shell_struct, divisibility);
    }
    // If the shellcode is divisible by 6 (no padding needed) //
    else {
        // Allocate shellcode malloc buffer //
        regularCopy(shell_struct);
    }

    // Establish the output file stream to write the resulting source code //
    openSourceFile(shell_struct);
}


void argParse(char* arg_array[], ShellcodeStruct& shellcode_struct) {
    /* Purpose -
     * Parameters:
     */
    // Validate the payload file arg //
    const filesys::path payload_file = validatePayloadFile(arg_array[1]);
    // Validate the obfuscation mode arg //
    const int obfuscation_mode = validateObfuscationMode(arg_array[2]);

    // If either parameter validation functions failed //
    if (payload_file.empty() || (obfuscation_mode < MIN_MODE || obfuscation_mode > MAX_MODE)) {
        // Display program usage & exit with error code //
        usageDisplay(&shellcode_struct.file_name, -2);
    }
    // If the data fails to be read from the binary into shellcode struct //
    if (!readBinFile(payload_file, shellcode_struct) || shellcode_struct.bytes_read == 0
        || shellcode_struct.in_shellcode_ptr == nullptr) {
        // Print error and return error code //
        printErr("Either binary file read operation failed, no bytes were read, or there is no"
                 " loaded shellcode in memory buffer.");
        std::exit(-2);
    }
    // Assign validated payload file path and obfuscation mode to ShellcodeStruct //
    shellcode_struct.file_path = payload_file;
    shellcode_struct.obfuscation_mode = obfuscation_mode;
}


int usageDisplay(const filesys::path* program_name, const int exit_code) {
    /* Purpose - Displays the program usage due to improper args provided upon initial execution.
     * Parameters:
     *      @ program_name - The name of the binary currently being executed.
     *      @ exit_code - The numerical code to be passed into the exit call.
     *
     * Returns - Erroneous exit code.
     */
    // Initialize border line variables //
    const char equals = '=';
    const char plus = '+';
    // Format border line //
    const std::string equal_line(LINE_COUNT, equals);
    const std::string full_line = plus + equal_line + plus;

    // Print usage with borders //
    std::cout << full_line << "\n"
              << "\t\t[!] Usage: " << *program_name << " <payload_file> <obfuscation_mode>" << "\n"
              << "\t\t[+] Payload obfuscation modes:\n"
              << "\t\t\t[1] IPv4 address => 192.168.4.37\n"
              << "\t\t\t[2] IPv6 address => 0C4F:E834:0000:000C:4151:0000:4150:5251\n"
              << "\t\t\t[3] MAC address  => BF-35-CE-3F-5A-6C\n"
              << full_line << std::endl;
    // Exit with passed in exit code //
    std::exit(exit_code);
}


void bannerDisplay() {
    /* Purpose - Displays the program banner and author (TheBugFather).
     */
    std::cout << R"(
 _                                     ___                                               -_
- - /, /, ,,            ,  _-_,       -   ---___- ,,            -_-/  ,,          ,, ,, / \\
  )/ )/ ) ||      _    ||    //          (' ||    ||           (_ /   ||          || || ` ||
  )__)__) ||/\\  < \, =||=   || \\/\\   ((  ||    ||/\\  _-_  (_ --_  ||/\\  _-_  || ||   |,
 ~)__)__) || ||  /-||  ||   ~|| || ||  ((   ||    || || || \\   --_ ) || || || \\ || ||  ((
  )  )  ) || || (( ||  ||    || || ||   (( //     || || ||/    _/  )) || || ||/   || ||
 /-_/-_/  \\ |/  \/\\  \\, _-_, \\ \\     -____-  \\ |/ \\,/  (_-_-   \\ |/ \\,/  \\ \\  <>
            _/                                      _/                  _/)" << "\n"
            << "\t\t\t[+] Author => TheBugFather\n"
            << "\t\t\t[$] Github @ github.com/TheBugFather" << std::endl;
}


int main(int argc, char* argv[]) {
    /* Purpose -
     * Parameters:
     */
    // Display the program banner //
    bannerDisplay();
    // Initialize the shellcode struct populated with NULL/empty types //
    struct ShellcodeStruct ShellStruct = {filesys::path(), filesys::path(), 0, 0, nullptr,
                                          nullptr, 0, filesys::path(), nullptr};
    // Get the name from the path arg as string //
    filenameExtract(argv[0], ShellStruct);

    // If an improper number of args were passed in //
    if (argc != MAX_ARGS) {
        // Print an error notifying that the shellcode arg was missing //
        printErr("Missing input parameter .. check usage and try again");
        // Display program usage & exit with error code //
        usageDisplay(&ShellStruct.file_name, -1);
    }
    // Validate parse the appropriate args into the shellcode struct //
    argParse(argv, ShellStruct);

    std::cout << "[+] The size of the shellcode read from " << ShellStruct.file_name
              << ": " << ShellStruct.bytes_read << std::endl;

    // Execute logic based on selected obfuscation mode //
    switch (ShellStruct.obfuscation_mode) {
        case MODE_IPV4:
            // Copy the read shellcode into malloc buffer, appending NOP slides if needed //
            shellCopyHandler(ShellStruct, 4);
            // Call the IPv4 based shellcode obfuscation handler //
            ipv4ObfuscationHandler(ShellStruct);
            break;
        case MODE_IPV6:
            // Copy the read shellcode into malloc buffer, appending NOP slides if needed //
            shellCopyHandler(ShellStruct, 16);
            // Call the IPv6 based shellcode obfuscation handler //
            ipv6ObfuscationHandler(ShellStruct);
            break;
        case MODE_MAC:
            // Copy the read shellcode into malloc buffer, appending NOP slides if needed //
            shellCopyHandler(ShellStruct, 6);
            // Call the MAC based shellcode obfuscation handler //
            macObfuscationHandler(ShellStruct);
            break;
        default:
            // Print error and return error code //
            printErr("Error occurred checking obfuscation mode, this logic should not happen");
            return -3;
    }
    // Ensure the output file stream is closed //
    closeFileStream(ShellStruct.output_stream);
    return 0;
}





// TODO: Remove after dev
/* Purpose -
 * Parameters:
 * Returns -
 */
