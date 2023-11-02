// Included libraries //
#include <iostream>
// Header files //
#include "hdrs/utils.h"


void nopPadding(ShellcodeStruct &shell_struct, const unsigned int nop_count) {
    /* Purpose -
     * Parameters:
     * Returns -
     */
    pass;
}


void printErr(const std::string& err_message) {
    /* Purpose -  Displays the error message via stderr.
     * Parameters:
     *      @ err_message - A pointer to the string message to be displayed.
     */
    std::cerr << "\n* [ERROR] " << err_message << " *" << std::endl;
}