// Included libraries //
#include <bits/stdc++.h>
#include <cstdio>
#include <filesystem>
#include <iostream>
// Header files //
#include "hdrs/arg_validate.h"
#include "hdrs/utils.h"
#include "hdrs/what_in_the_shell.h"
// Local Macros //
#define MAX_ARGS 3


int usageDisplay(std::string *program_name) {
    /* Purpose - Displays the program usage due to improper args provided upon initial execution.
     * Parameters:
     *      @ program_name - The name of the binary currently being executed.
     *
     * Returns - Erroneous exit code.
     */
    // Initialize a character array based on the program name with null byte
    char char_name[program_name->length()];
    // Copy converted string back into array buffer
    std::strcpy(char_name, program_name->c_str());

    puts("+===============================================================================+");
    printf("[!] Usage: %s <payload_file> <obfuscation_mode>", char_name);
    puts("[+] Payload obfuscation modes:\n"
         "\t[1] MAC address  => BF-35-CE-3F-5A-6C\n"
         "\t[2] IPv4 address => 192.168.4.37\n"
         "\t[3] IPv6 address => 0C4F:E834:0000:000C:4151:0000:4150:5251\n"
         "+===============================================================================+"
         );
    return -1;
}


void bannerDisplay() {
    /* Purpose - Displays the program banner and author.
     */
    std::cout << R"(
     _                                     ___                                               -_
    - - /, /, ,,            ,  _-_,       -   ---___- ,,            -_-/  ,,          ,, ,, / \\
      )/ )/ ) ||      _    ||    //          (' ||    ||           (_ /   ||          || || ` ||
      )__)__) ||/\\  < \, =||=   || \\/\\   ((  ||    ||/\\  _-_  (_ --_  ||/\\  _-_  || ||   |,
     ~)__)__) || ||  /-||  ||   ~|| || ||  ((   ||    || || || \\   --_ ) || || || \\ || ||  ((
      )  )  ) || || (( ||  ||    || || ||   (( //     || || ||/    _/  )) || || ||/   || ||
     /-_/-_/  \\ |/  \/\\  \\, _-_, \\ \\     -____-  \\ |/ \\,/  (_-_-   \\ |/ \\,/  \\ \\  <>
                _/                                      _/                  _/
    )" << "\t\t\t\t[+] Author => TheBugFather" << std::endl;
}


int main(int argc, char *argv[]) {
    /* Purpose -
     * Parameters:
     * Returns -
     */
    // Display the program banner
    bannerDisplay();
    // Set exe path as string
    std::string exe_path = argv[0];
    // Get the name of the program name from arg file path
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
    return 0;
}





// TODO: Remove after dev
/* Purpose -
 * Parameters:
 * Returns -
 */
