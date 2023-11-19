// Included libraries //
#include <iostream>
// Header files //
#include "hdrs/cleanup_manager.h"
#include "hdrs/utils.h"


unsigned int closestMultiple(const unsigned int bytes_read, const unsigned int factor) {
    /* Purpose - Ensures the bytes read is a multiple or divisible by the passed in factor. If not
     *           the offset is calculated to ensure the shellcode is divisible of its obfuscation
     *           method.
     * Parameters:
     *      @ bytes_read - The length of the original shellcode read from the source file.
     *      @ factor - The divisibility factor associated with the obfuscation mode.
     *
     * Returns - Returns either the bytes read if a multiple or bytes read plus calculated offset.
     */
    // If the factor is 0, exit with original number of bytes //
    if (factor <= 0) {
        // Format error message with obfuscation factor and display it //
        std::string err_message = "Configured obfuscation factor " + std::to_string(factor) +
                                  " needs to be greater than 0";
        printErr(err_message);
        std::exit(-5);
    }
    // Get the modulus of bytes read and factor //
    const unsigned int remainder = bytes_read % factor;
    // Return bytes read if remainder is 0, otherwise round up bytes read to the nearest factor //
    return (remainder == 0) ? bytes_read : bytes_read + factor - remainder;
}


void filenameExtract(const char* path_arg, ShellcodeStruct& shell_struct) {
    /* Purpose - Set the arg char array to string type and save file name in ShellcodeStruct.
     * Parameters:
     *      @ path_arg - Passed in reference the to programs argv[] array.
     *      @ shell_struct - Reference to the shellcode struct for data storage.
     */
    std::string exe_path = path_arg;
    shell_struct.file_name = exe_path.substr(exe_path.find_last_of("/\\") + 1);
}


unsigned char* managedHeapAlloc(size_t buffer_size) {
    /* Purpose - Registers heap buffer with cleanup manager automated heap management system.
     * Parameters:
     *      @ buffer_size - The size of the malloc buffer to be allocated and stored.
     *
     * Returns - Pointer to the allocated heap buffer.
     */
    // Allocate unsigned char buffer based on passed in size //
    auto* buffer = new unsigned char[buffer_size];
    // Register buffer in atexit heap manager //
    cleanupManager.registerBuffer(buffer);
    return buffer;
}


void nopPaddingCopy(ShellcodeStruct& shell_struct, const unsigned int divisor_factor) {
    /* Purpose - Allocated heap buffer based on the shellcode read from the source file with added
     *           NOP slides based on calculated needs based on divisibility of obfuscation mode.
     * Parameters:
     *      @ shell_struct - The shellcode struct where the buffer pointers reside.
     *      @ divisor_factor - The divisibility factor based on the obfuscation mode.
     */
    const unsigned char nop_slide = 0x90;
    unsigned int index = 0;

    // Ensure passed in divisor factor is a multiple of read bytes or round it up to be //
    const unsigned int divisor = closestMultiple(shell_struct.bytes_read, divisor_factor);
    // Calculate the number of NOP slides to pad at the end of shellcode //
    const unsigned int needed_nops = divisor - shell_struct.bytes_read;

    // Allocate heap memory for the padded shellcode //
    unsigned char* padded_shellcode = managedHeapAlloc(shell_struct.bytes_read + needed_nops + 1);
    // Copy the existing shellcode into the created padded buffer //
    memcpy(padded_shellcode, shell_struct.in_shellcode_ptr, shell_struct.bytes_read);

    // While there are still NOP slides to append to the end of the payload //
    while (index != needed_nops) {
        // Add NOP slide to the current index in heap //
        padded_shellcode[shell_struct.bytes_read + index] = nop_slide;
        index++;
    }
    // Set pointer payload buffer and its size //
    shell_struct.pad_shellcode_ptr = padded_shellcode;
    shell_struct.result_size = shell_struct.bytes_read + needed_nops;
}


void printErr(const std::string& err_message) {
    /* Purpose -  Displays the error message via stderr.
     * Parameters:
     *      @ err_message - A pointer to the string message to be displayed.
     */
    std::cerr << "\n* [ERROR] " << err_message << " *" << std::endl;
}


void regularCopy(ShellcodeStruct& shell_struct) {
    /* Purpose - Allocated heap buffer based on the shellcode read from the source file with no need
     *           for additional NOP slide padding.
     * Parameters:
     *      @ shell_struct - The shellcode struct where the malloc buffer pointer is stored.
     */
    // Allocate heap memory for obfuscated shellcode //
    shell_struct.pad_shellcode_ptr = managedHeapAlloc(shell_struct.bytes_read);
    // Copy the input shellcode to obfuscated buffer for obfuscation process //
    memcpy(shell_struct.pad_shellcode_ptr, shell_struct.in_shellcode_ptr, shell_struct.bytes_read);
    // Save the bytes read as the result size //
    shell_struct.result_size = shell_struct.bytes_read;
}