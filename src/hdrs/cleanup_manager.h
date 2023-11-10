#ifndef CLEANUP_MANAGER_H
#define CLEANUP_MANAGER_H
#include <vector>

class CleanupManager {
public:
    // Constructor & deconstructor //
    CleanupManager();
    ~CleanupManager();

    // Buffer registry function //
    void registerBuffer(unsigned char* buffer);
    // Public accessor function //
    [[nodiscard]] const std::vector<unsigned char*>& getCleanupList() const;

private:
    std::vector<unsigned char*> cleanup_list;
};

CleanupManager cleanupManager;

#endif