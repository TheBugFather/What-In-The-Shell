// Included libraries //
#include <fstream>
#include <iostream>
#include <vector>
// Header files //
#include "hdrs/io_operations.h"
#include "hdrs/utils.h"


void openSourceFile(ShellcodeStruct& shell_struct) {
    // Open the file in binary mode for reading and writing //
    std::ofstream file_stream(shell_struct.output_file, std::ios::binary);

    // Ensure the file stream was properly opened //
    if (!file_stream.is_open()) {
        // Format error message, display it, and exit with error code //
        std::string err_message = "Unable to open file - " + shell_struct.output_file.string();
        printErr(err_message);
        std::exit(-6);
    }
    // Set the file stream in shellcode struct //
    shell_struct.output_stream = std::move(file_stream);
}


bool readBinFile(const filesys::path& arg_file, ShellcodeStruct& shell_struct) {
    /* Purpose -  Reads shellcode from source file to store ShellcodeStruct until obfuscation
     *            mutation is generated.
     * Parameters:
     *      @ arg_file - The file path to the file where the binary shellcode data will be read.
     *      @ shell_struct - Reference to the shellcode struct.
     *
     * Returns - true/false on success/failure.
     */
    try {
        // If the file does not exist or is not a regular file //
        if (!filesys::exists(arg_file) || !filesys::is_regular_file(arg_file)) {
            // Raise catch handler with following string message //
            throw std::ios_base::failure("Input file either does not exist or is not a file");
        }
        // Open the input file in binary mode pointed at the end of file //
        std::ifstream read_stream(arg_file, std::ios::binary | std::ios::ate);

        // If the file failed to open //
        if (!read_stream.is_open()) {
            // Raise catch handler with following string message //
            throw std::ios_base::failure("Unable to open input file to read data.");
        }
        // Get the position of the end of file (total size) //
        const std::streampos file_size = read_stream.tellg();
        // Set the file pointer back to start of file to read data //
        read_stream.seekg(0, std::ios::beg);
        // Create vector to store read file data //
        std::vector<unsigned char> read_bytes(file_size);

        // If bytes are successfully read into vector as unsigned char type //
        if (read_stream.read(reinterpret_cast<char*>(read_bytes.data()), file_size)) {
            // Set the number of bytes read //
            shell_struct.bytes_read = file_size;
            // Set the reference to read file data //
            shell_struct.in_shellcode_ptr = read_bytes.data();
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