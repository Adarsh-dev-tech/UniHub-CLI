#pragma once
#include <string>
#include <vector>

namespace uni {

struct ResourceItem {
    std::string filename; // stored filename
    std::string displayName; // e.g., title
    std::size_t sizeBytes{};
};

// List files under a resource folder
std::vector<ResourceItem> listResources(const std::string& folder);

// Upload: copy local path into folder (returns destination path or error string)
std::pair<bool,std::string> uploadResource(const std::string& localPath, const std::string& folder);

// Download: copy from folder/file to localDest (returns success, message)
std::pair<bool,std::string> downloadResource(const std::string& storedPath, const std::string& localDest);

}
