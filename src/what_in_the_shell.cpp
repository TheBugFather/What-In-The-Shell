// Included libraries //
#include <iostream>
// Header files //
#include "hdrs/arg_validate.h"
#include "hdrs/io_operations.h"
#include "hdrs/utils.h"
#include "hdrs/what_in_the_shell.h"
// Local Macros //
#define LINE_COUNT 90
#define MAX_ARGS 3


int usageDisplay(std::string* program_name) {
    /* Purpose - Displays the program usage due to improper args provided upon initial execution.
     * Parameters:
     *      @ program_name - The name of the binary currently being executed.
     *
     * Returns - Erroneous exit code.
     */
    // Initialize border line variables //
    const char equals = '=';
    const char plus = '+';
    // Format border line //
    std::string equal_line(LINE_COUNT, equals);
    std::string full_line = plus + equal_line + plus;

    // Print usage with borders //
    std::cout << full_line << "\n"
              << "\t\t[!] Usage: " << *program_name << " <payload_file> <obfuscation_mode>" << "\n"
              << "\t\t[+] Payload obfuscation modes:\n"
              << "\t\t\t[1] MAC address  => BF-35-CE-3F-5A-6C\n"
              << "\t\t\t[2] IPv4 address => 192.168.4.37\n"
              << "\t\t\t[3] IPv6 address => 0C4F:E834:0000:000C:4151:0000:4150:5251\n"
              << full_line << std::endl;
    return -1;
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


int main(int argc, char *argv[]) {
    /* Purpose -
     * Parameters:
     * Returns -
     */
    // Display the program banner //
    bannerDisplay();
    // Set exe path as string //
    std::string exe_path = argv[0];
    // Get the name of the program name from arg file path //
    std::string exe_name = exe_path.substr(exe_path.find_last_of("/\\") + 1);

    // If an improper number of args were passed in //
    if (argc != MAX_ARGS) {
        // Print an error notifying that the shellcode arg was missing //
        printErr("Missing input parameter .. check usage and try again");
        // Display program usage & exit with error code //
        return usageDisplay(&exe_name);
    }
    // Validate the payload file arg //
    const filesys::path payload_file = validatePayloadFile(argv[1]);
    // Validate the obfuscation mode arg //
    const int obfuscation_mode = validateObfuscationMode(argv[2]);

    // If either parameter validation functions failed //
    if (payload_file.empty() || (obfuscation_mode < MIN_MODE || obfuscation_mode > MAX_MODE)) {
        // Display program usage & exit with error code //
        return usageDisplay(&exe_name);
    }
    // Initialize the shellcode struct populated with null bytes //
    struct ShellcodeStruct ShellStruct = { 0 };

    // If the data fails to be read from the binary into shellcode struct //
    if (!readBinFile(payload_file, ShellStruct) || ShellStruct.bytes_read == 0
    || ShellStruct.src_shellcode_ptr == nullptr) {
        // Print error and return error code //
        printErr("Either binary file read operation failed, no bytes were read, or there is no"
                 "loaded shellcode in memory buffer.");
        return -2;
    }
    std::cout << "[+] The size of the shellcode read from " << payload_file
              << ": " << ShellStruct.bytes_read << std::endl;

    // Check the obfuscation mode to execute the proper obfuscation function //
    switch (obfuscation_mode) {
        case MODE_MAC:
            // If shellcode is not divisible by 6 (needs NOP padding) //
            if (!ShellStruct.bytes_read % 6 == 0) {
                std::cout << "[+] The shellcode is not divisible by 6 based on MAC address"
                             " .. padding NOP slides" << std::endl;
                // Allocate shellcode malloc buffer with appended NOP slide padding //
                nopPaddingCopy(ShellStruct, 6);
            }
            // If the shellcode is divisible by 6 (no padding needed) //
            else {
                // Allocate shellcode malloc buffer //
                shellCopy(ShellStruct);
            }
            break;

        case MODE_IPV4:
            // If shellcode is not divisible by 4 (needs NOP padding) //
            if (!ShellStruct.bytes_read % 4 == 0) {
                std::cout << "[+] The shellcode is not divisible by 4 based on IPv4 address"
                             " .. padding NOP slides" << std::endl;
                // Allocate shellcode malloc buffer with appended NOP slide padding //
                nopPaddingCopy(ShellStruct, 4);
            }
            // If the shellcode is divisible by 4 (no padding needed) //
            else {
                // Allocate shellcode malloc buffer //
                shellCopy(ShellStruct);
            }
            break;

        case MODE_IPV6:
            // If shellcode is not divisible by 16 (needs NOP padding) //
            if (!ShellStruct.bytes_read % 16 == 0) {
                std::cout << "[+] The shellcode is not divisible by 16 based on IPv6 address"
                             " .. passing NOP slides" << std::endl;
                // Allocate shellcode malloc buffer with appended NOP slide padding //
                nopPaddingCopy(ShellStruct, 16);
            }
            // If the shellcode is divisible by 16 (no padding needed) //
            else {
                // Allocate shellcode malloc buffer //
                shellCopy(ShellStruct);
            }
            break;

        default:
            // Print error and return error code //
            printErr("Error occurred checking obfuscation mode, this logic should not happen");
            return -3;
    }

    // TODO: allocate malloc buffer for transferring shellcode as unsigned char type

    // TODO: do another case statement like above for generating the obfuscated payload array
    //      in its generated decryptor source code

    return 0;
}





// TODO: Remove after dev
/* Purpose -
 * Parameters:
 * Returns -
 */
