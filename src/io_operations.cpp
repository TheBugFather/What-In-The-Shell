// Included libraries //
#include <fstream>
#include <iostream>
#include <vector>
// Header files //
#include "hdrs/io_operations.h"
#include "hdrs/what_in_the_shell.h"


bool readBinFile(const filesys::path arg_file, ShellcodeStruct &shell_struct) {
    /* Purpose -
     * Parameters:
     * Returns -
     */
    try {
        // If the file does not exist or is not a regular file //
        if (!filesys::exists(arg_file) || !filesys::is_regular_file(arg_file)) {
            // Raise catch handler with following string message //
            throw std::ios_base::failure("Input file either does not exist or is not a file");
        }
        // Open the input file in binary mode and set the file pointer at the end //
        std::ifstream read_stream(arg_file, std::ios::binary);

        // If the file failed to open //
        if (!read_stream.is_open()) {
            // Raise catch handler with following string message //
            throw std::ios_base::failure("Unable to open input file to read data.");
        }
        // Get the size of the file data //
        const std::streampos file_size = filesys::file_size(arg_file);
        // Create vector to store read file data //
        std::vector<char> read_bytes(file_size);

        // If bytes are successfully read into vector //
        if (read_stream.read(read_bytes.data(), file_size)) {
            // Set the number of bytes read //
            shell_struct.bytes_read = file_size;
            // Set the reference to read file data //
            shell_struct.src_shellcode_ptr = read_bytes.data();
        }
        // If bytes fail to be read into vector //
        else {
            // Raise catch handler with following string message //
            throw std::ios_base::failure("Unable to read data from the input file.");
        }
    }
    // If an error occurs during file operation //
    catch (const std::ios_base::failure& file_err) {
        // Print error and return false boolean //
        std::cerr << "[*] Error occurred during file operation: " << file_err.what() << std::endl;
        return false;
    }
    return true;
}


// bool writeSourceFile();