// Included libraries //
#include <list>
// Header files //
#include "hdrs/arg_validate.h"
#include "hdrs/utils.h"


filesys::path validatePayloadFile(const char* arg_file_path) {
    /* Purpose - Confirms that the passed in string reference to file actually exists on disk.
     * Parameters:
     *      @ arg_file_path - The passed in string file path to the file on disk to be validated.
     *
     * Returns - The validated file path on success, and a empty file path on failure.
     */
    // Set the file path based on the passed in string //
    filesys::path file_path = arg_file_path;

    // If the file path does not exist //
    if (!filesys::exists(file_path)) {
        // Print error return empty string //
        printErr("Passed in file path does not exist .. check usage and try again");
        // Set the filesystem path to empty string //
        file_path = "";
    }
    return file_path;
}


int validateObfuscationMode(const char* arg_obfuscation_mode) {
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
    const std::list<int> mode_list = {MIN_MODE, 2, MAX_MODE};

    try {
        // Convert the parsed obfuscation mode to integer //
        const int operation_mode = std::stoi(arg_obfuscation_mode);
        // Iterate through the list of specified modes //
        for (const int mode : mode_list) {
            // If the current iteration mode is equal to the passed in arg //
            if (mode == operation_mode) {
                // Set the temp buffer value to the current iteration //
                temp_mode = mode;
                break;
            }
        }
    }
    // If the char string to in int conversion fails due to out of range data type //
    catch (const std::out_of_range& conversion_err) {
        // Print conversion error and return error code //
        printErr(conversion_err.what());
        return -1;
    }

    return temp_mode;
}