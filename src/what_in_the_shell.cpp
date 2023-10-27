// Included libraries
#include <iostream>
#include <stdio>
using namespace std


int ValidateObfuscationMode(const char *arg_obfuscation_mode) {
    /* Purpose -  Confirms that the passed in obfuscation mode is one of the available options.
     * Parameters:
     *      @ arg_obfuscation_mode - The passed in obfuscation mode to be validated.
     *
     * Returns - The validated obfuscation mode on success, and either 0 if number is out available
     *           mode range or -1 if error occurred during char array to int conversion.
     */
    // Set temporary buffer for storing converted obfuscation mode //
    int temp_mode = 0;
    // Declare a list of int modes to compare after arg is converted //
    const list<int> mode_list = {1, 2, 3};

    try {
        // Convert the parsed obfuscation mode to integer //
        const int operation_mode = stoi(arg_obfuscation_mode);
        // Iterate through the list of specified modes //
        for (int mode : mode_list) {
            // If the current iteration mode is equal to the passed in arg //
            if (mode == operation_mode) {
                // Set the temp buffer value to the current iteration //
                temp_mode = mode;
                break;
            }
        }
    }
    // If error occurs converting the obfuscation mode arg to integer //
    catch (const exception &conversion_err) {
        // Print conversion error and return error code //
        PrintErr(conversion_err.what());
        return -1;
    }

    return temp_mode;
}


filesystem ValidatePayloadFile(const char *arg_file_path) {
    /* Purpose - Confirms that the passed in string reference to file actually exists on disk.
     * Parameters:
     *      @ arg_file_path - The passed in string file path to the file on disk to be validated.
     *
     * Returns - The validated file path on success, and a empty file path on failure.
     */
    // Set the file path based on the passed in string //
    filesystem file_path = arg_file_path;

    // If the file path does not exist //
    if (!filesystem::exists(file_path)) {
        // Print error return empty string //
        PrintErr("Passed in file path does not exist .. check usage and try again");
        // Set the filesystem path to empty string //
        file_path = "";
        return file_path;
    }
    return file_path;
}


int UsageDisplay(char *program_name) {
    /* Purpose - Displays the program usage due to improper args provided upon initial execution.
     * Parameters:
     *      @ program_name - The name of the binary currently being executed.
     *
     * Returns - Erroneous exit code.
     */
    puts("+================================================================+");
    printf("[!] Usage: %s <payload_file> <obfuscation_mode>", program_name);
    puts("[+] Payload obfuscation modes:\n"
         "\t[1] MAC address  => BF-35-CE-3F-5A-6C\n"
         "\t[2] IPv4 address => 192.168.4.37\n"
         "\t[3] IPv6 address => 0C4F:E834:0000:000C:4151:0000:4150:5251\n"
         "+================================================================+"
         );
    return -1;
}


void PrintErr(const std::string& err_message) {
    /* Purpose -  Displays the error message via stderr.
     * Parameters:
     *      @ err_message - A pointer to the string message to be displayed.
     */
    cerr << "\n* [ERROR] " << err_message << " *\n" << endl;
}


int main(int argc, char *argv[]) {
    /* Purpose -
     * Parameters:
     * Returns -
     */

    // If an improper number of args were passed in //
    if (argc != 3) {
        // Print an error notifying that the shellcode arg was missing //
        PrintErr("Missing input parameter .. check usage and try again");
        // Display program usage & exit with error code //
        return UsageDisplay(argv[0]);
    }
    // Validate the payload file arg //
    const filesystem payload_file = ValidatePayloadFile(argv[1]);
    // Validate the obfuscation mode arg //
    const int obfuscation_mode = ValidateObfuscatioMode(argv[2]);

    // If either parameter validation functions failed //
    if (payload_file.path.empty() || (obfuscation_mode < 1 || obfuscation_mode > 3)) {
        // Display program usage & exit with error code //
        return UsageDisplay(argv[0])
    }
    // Validate the input file //
    return 0;
}





// TODO: Remove after dev
/* Purpose -
 * Parameters:
 * Returns -
 */
