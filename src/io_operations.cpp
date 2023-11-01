// Included libraries //
#include <fstream>
// Header files //
#include "hdrs/io_operations.h"
#include "hdrs/what_in_the_shell.h"
// Define namespace aliases //
namespace filesys = ns_filesystem;


bool readBinFile(const filesys::path arg_file, ShellcodeStruct &shell_struct) {
    try {
        // Ensure the file exists and is a regular file //
        if (filesys::exists(arg_file) && filesys::is_regular_file(arg_file)) {
            // Open the input file in binary mode and set the file pointer at the end //
            std::ifstream readStream(arg_file, std::ios::binary | std::ios::ate);

            // If the file failed to open //
            if (!readStream.is_open()) {
                throw std::ios_base::failure("\n[*] Unable to open input file to read data.\n\n");
            }
            // TODO: add more code here

        }
    }
    // If an error occurs during file operation //
    catch (const std::ios_base::failure& file_err) {
        // Print error and return file boolean //
        std::cerr << "[*] Error occurred during file operation: " << file_err.what() << std::endl;
        return false;
    }

    return true;
}