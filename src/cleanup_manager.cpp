// Included libraries //
#include <cstdlib>
// Header files //
#include "hdrs/cleanup_manager.h"


CleanupManager cleanupManager;


namespace {
    void cleanupRoutine() {
        /* Purpose - Atexit routine to free all the registered pointers to heap buffers.
         */
        // Iterate through the registered buffers //
        for (unsigned char* buffer : cleanupManager.getCleanupList()) {
            // Free the current buffer //
            delete[] buffer;
        }
    }
}


// Constructor routine //
CleanupManager::CleanupManager() {
    // Register heap buffer cleanup routine at program exit //
    atexit(cleanupRoutine);
}


// Deconstructor routine //
CleanupManager::~CleanupManager() {}


void CleanupManager::registerBuffer(unsigned char* buffer) {
    /* Purpose - Routine to register buffer in management vector.
     * Parameters:
     *      @ buffer - The pointer to the binary data buffer.
     */
    cleanup_list.push_back(buffer);
}


const std::vector<unsigned char*>& CleanupManager::getCleanupList() const {
    /* Purpose - Routine to access the pointers to buffers stored in buffer vector.
     * Returns - Access to internally stored buffer pointers in heap management vector.
     */
    return cleanup_list;
}
