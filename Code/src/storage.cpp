#include "storage.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace uni {

std::string dataDir() {
    return "data";
}

std::string resourcesDir() {
    return dataDir() + std::string("/resources");
}

bool ensureDir(const std::string& path) {
    try {
        fs::create_directories(path);
        return true;
    } catch (...) { return false; }
}

std::optional<std::string> readTextFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) return std::nullopt;
    std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    return content;
}

std::optional<std::string> writeTextFile(const std::string& path, const std::string& content) {
    std::ofstream out(path);
    if (!out) return std::optional<std::string>("Failed to write: " + path);
    out << content;
    return std::nullopt;
}

std::vector<std::string> listFiles(const std::string& path) {
    std::vector<std::string> result;
    try {
        for (auto& p : fs::directory_iterator(path)) {
            result.push_back(p.path().filename().string());
        }
    } catch (...) {}
    return result;
}

bool copyFile(const std::string& src, const std::string& dst) {
    try {
        ensureDir(fs::path(dst).parent_path().string());
        fs::copy_file(src, dst, fs::copy_options::overwrite_existing);
        return true;
    } catch (...) { return false; }
}

}
