// Included libraries
#include <iostream>
#include <stdio>
using namespace std


int ValidateObfuscationMode(char *arg_obfuscation_mode) {
    /* Purpose -
     * Parameters:
     * Returns -
     */
    // Set temporary buffer for storing converted obfuscation mode
    int temp_mode = 0;
    // Declare a list of int modes to compare after arg is converted
    list<int> mode_list = {1, 2, 3}

    try {
        // Convert the parsed obfuscation mode to integer
        int operation_mode = stoi(arg_obfuscation_mode);
        // Iterate through the list of specified modes
        for (int mode : mode_list) {
            // If the current iteration mode is equal to the passed in arg
            if (mode == operation_mode) {
                // Set the temp buffer value to the current iteration
                temp_mode = mode;
            }
        }
    }
    // If error occurs converting the obfuscation mode arg to integer
    catch (exception const & conversion_err) {
        // Print conversion error and return error code
        PrintErr(conversion_err.what())
        return -1;
    }

    return temp_mode;
}


 filesystem ValidatePayloadFile(char *arg_file_path) {
    /* Purpose -
     * Parameters:
     * Returns -
     */
    // Set the file path based on the passed in string
    filesystem file_path = arg_file_path;

    // If the file path does not exist
    if (!filesystem::exists(file_path)) {
        // Print error return empty string
        PrintErr("Passed in file path does not exist .. check usage and try again");
        return string()
    }
    return file_path;
}


int UsageDisplay(char *program_name) {
    /* Purpose -
     * Parameters:
     * Returns -
     */
    puts("+================================================================+")
    printf("[!] Usage: %s <payload_file> <obfuscation_mode>", program_name);
    puts("[+] Payload obfuscation modes:\n"
         "\t[1] MAC address  => BF-35-CE-3F-5A-6C\n"
         "\t[2] IPv4 address => 192.168.4.37\n"
         "\t[3] IPv6 address => 0C4F:E834:0000:000C:4151:0000:4150:5251\n"
         "+================================================================+"
         )
    return -1;
}


void PrintErr(const std::string& err_message) {
    /* Purpose -  Displays the error message via stderr.
    * Parameters:  A pointer to the string message to be displayed.
    */
    // Display the error message via stderr
    cerr << "\n* [ERROR] " << err_message << " *\n" << endl;
}


int main(int argc, char *argv[]) {
    /* Purpose -
     * Parameters:
     * Returns -
     */

    // If an improper number of args were passed in
    if (argc != 3) {
        // Print an error notifying that the shellcode arg was missing
        PrintErr("Missing input parameter .. check usage and try again");
        // Display program usage & exit with error code
        return UsageDisplay(argv[0]);
    }
    // Validate the payload file arg
    filesystem payload_file = ValidatePayloadFile(argv[1]);
    // Validate the obfuscation mode arg
    int obfuscation_mode = ValidateObfuscatioMode(argv[2]);

    // If either parameter validation functions failed
    if (payload_file.empty() || obfuscation_mode < 1) {
        // Display program usage & exit with error code
        return UsageDisplay(argv[0])
    }
    // Validate the input file
    return 0;
}





// TODO: Remove after dev
/* Purpose -
 * Parameters:
 * Returns -
 */
