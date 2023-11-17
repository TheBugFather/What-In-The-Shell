// Header files //
#include "hdrs/ipv6_obfuscation.h"


void ipv6Gen() {
    /* Purpose -
     * Parameters:
     */
    ;
}


uint32_t ipv6HexGen(int byte_1, int byte_2, int byte_3, int byte_4) {
    /* Purpose - Takes the 4 bytes of passed in shellcode data and generates a hex value to be
     *           converted to an IPv6 address in another function.
     * Parameters:
     *      @ byte_1 - First byte of shellcode.
     *      @ byte_2 - Second byte of shellcode.
     *      @ byte_3 - Third byte of shellcode.
     *      @ byte_4 - Fourth byte of shellcode.
     *
     * Returns - The formatted hex value of the passed in 4 bytes.
     */
    return ((uint32_t)byte_1 << 24) | ((uint32_t)byte_2 << 16) | (byte_3 << 8) | byte_4;
}



void ipv6ObfuscationHandler(ShellcodeStruct& shell_struct) {
    /* Purpose -
     * Parameters:
     */
    ;
}