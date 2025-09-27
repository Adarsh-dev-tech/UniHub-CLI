#include "resources.h"
#include "storage.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace uni {

std::vector<ResourceItem> listResources(const std::string& folder) {
    std::vector<ResourceItem> items;
    try {
        for (auto& p : fs::directory_iterator(folder)) {
            if (!p.is_regular_file()) continue;
            auto sz = fs::file_size(p.path());
            items.push_back(ResourceItem{p.path().string(), p.path().filename().string(), sz});
        }
    } catch (...) {}
    return items;
}

std::pair<bool,std::string> uploadResource(const std::string& localPath, const std::string& folder) {
    try {
        ensureDir(folder);
        std::string dst = folder + "/" + fs::path(localPath).filename().string();
        if (!copyFile(localPath, dst)) return {false, "Copy failed"};
        return {true, dst};
    } catch (...) {
        return {false, "Upload failed"};
    }
}

std::pair<bool,std::string> downloadResource(const std::string& storedPath, const std::string& localDest) {
    try {
        if (!copyFile(storedPath, localDest)) return {false, "Copy failed"};
        return {true, localDest};
    } catch (...) {
        return {false, "Download failed"};
    }
}

}
