// Header files //
#include "hdrs/io_operations.h"
#include "hdrs/utils.h"


void printErr(const std::string& err_message) {
    /* Purpose -  Displays the error message via stderr.
     * Parameters:
     *      @ err_message - A pointer to the string message to be displayed.
     */
    std::cerr << "\n* [ERROR] " << err_message << " *" << std::endl;
}