// Included libraries //
#include <stdio>
// Header files //
#include "hdrs/utils.h"

void printErr(const string& err_message) {
    /* Purpose -  Displays the error message via stderr.
     * Parameters:
     *      @ err_message - A pointer to the string message to be displayed.
     */
    cerr << "\n* [ERROR] " << err_message << " *\n" << endl;
}