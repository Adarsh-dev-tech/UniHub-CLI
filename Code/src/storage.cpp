/*
    storage.cpp

    This source file implements file and directory utility functions for the UniHub-CLI application.
    It provides operations for managing data and resource directories, reading and writing text files,
    listing files in directories, and copying files. These functions abstract the underlying file system
    interactions required for persistent storage and resource management.
*/

#include "storage.h"         // Include storage interface definitions
#include <filesystem>        // Include filesystem operations
#include <fstream>           // Include file stream operations
#include <iostream>          // Include input/output stream operations
#include <optional>          // Include optional type for return values
#include <vector>            // Include vector type for dynamic arrays

using namespace std; // Allows usage of standard library types without std:: prefix

namespace fs = std::filesystem; // Alias for std::filesystem namespace

namespace uni { // Begin namespace uni

string dataDir() {
    return "data"; // Returns the base data directory name
}

string resourcesDir() {
    return dataDir() + string("/resources"); // Returns the resources directory path
}

bool ensureDir(const string& path) {
    try {
        fs::create_directories(path); // Creates the directory and any parent directories if needed
        return true; // Return true on success
    } catch (...) { return false; } // Return false on exception
}

optional<string> readTextFile(const string& path) {
    ifstream in(path); // Open file for reading
    if (!in) return nullopt; // Return nullopt if file can't be opened
    string content((istreambuf_iterator<char>(in)), istreambuf_iterator<char>()); // Read entire file content
    return content; // Return file content
}

optional<string> writeTextFile(const string& path, const string& content) {
    ofstream out(path); // Open file for writing
    if (!out) return optional<string>("Failed to write: " + path); // Return error message if file can't be opened
    out << content; // Write content to file
    return nullopt; // Return nullopt on success
}

vector<string> listFiles(const string& path) {
    vector<string> result; // Vector to store filenames
    try {
        for (auto& p : fs::directory_iterator(path)) { // Iterate over files in directory
            result.push_back(p.path().filename().string()); // Add filename to result
        }
    } catch (...) {} // Ignore exceptions (e.g., directory not found)
    return result; // Return list of filenames
}

bool copyFile(const string& src, const string& dst) {
    try {
        ensureDir(fs::path(dst).parent_path().string()); // Ensure destination directory exists
        fs::copy_file(src, dst, fs::copy_options::overwrite_existing); // Copy file, overwrite if exists
        return true; // Return true on success
    } catch (...) { return false; } // Return false on exception
}

} // End namespace uni
