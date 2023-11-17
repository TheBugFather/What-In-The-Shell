// Header files //
#include "hdrs/mac_obfuscation.h"


void macGenerate() {
    /* Purpose -
     * Parameters:
     * Returns -
     */
    ;
}


uint64_t macHexGen(int byte_1, int byte_2, int byte_3, int byte_4, int byte_5, int byte_6) {
    /* Purpose - Takes the 6 bytes of passed in shellcode data and generates a hex value to be
     *           converted to an MAC address in another function.
     * Parameters:
     *      @ byte_1 - First byte of shellcode.
     *      @ byte_2 - Second byte of shellcode.
     *      @ byte_3 - Third byte of shellcode.
     *      @ byte_4 - Fourth byte of shellcode.
     *
     * Returns - The formatted hex value of the passed in 6 bytes.
     */
    return ((uint64_t)byte_1 << 40) | ((uint64_t)byte_2 << 32) | ((uint64_t)byte_3 << 24) |
            ((uint64_t)byte_4 << 16) | ((uint64_t)byte_5 << 8) | byte_6;
}


void macObfuscationHandler(ShellcodeStruct& shell_struct) {
    /* Purpose -
     * Parameters:
     * Returns -
     */
    ;
}