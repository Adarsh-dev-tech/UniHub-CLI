#pragma once
#include <string>
#include <optional>
#include <vector>

namespace uni {

std::string dataDir();
std::string resourcesDir();

bool ensureDir(const std::string& path);
std::optional<std::string> readTextFile(const std::string& path);
std::optional<std::string> writeTextFile(const std::string& path, const std::string& content);
std::vector<std::string> listFiles(const std::string& path);
bool copyFile(const std::string& src, const std::string& dst);

}
