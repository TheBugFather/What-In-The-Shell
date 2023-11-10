// Included libraries //
#include <cstdlib>
// Header files //
#include "hdrs/cleanup_manager.h"


namespace {
    void cleanupRoutine() {
        // Iterate through the registered buffers //
        for (unsigned char* buffer : cleanupManager.getCleanupList()) {
            // Free the current buffer //
            delete[] buffer;
        }
    }
}

// Constructor routine //
CleanupManager::CleanupManager() {
    atexit(cleanupRoutine);
}

// Deconstructor routine //
CleanupManager::~CleanupManager() {}

// Routine to register buffer in management vector //
void CleanupManager::registerBuffer(unsigned char* buffer) {
    cleanup_list.push_back(buffer);
}

// Routine to access buffer vector //
const std::vector<unsigned char*>& CleanupManager::getCleanupList() const {
    return cleanup_list;
}
