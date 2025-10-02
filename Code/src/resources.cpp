/*
    resources.cpp

    This source file implements resource management functions for the UniHub-CLI application.
    It provides functionality to list files in resource folders, upload files to resource
    directories, and download files from resource storage. These operations interact with
    the file system to facilitate sharing and accessing academic resources.
*/

#include "resources.h"      // Include resource management interface
#include "storage.h"        // Include file and directory utility functions
#include <filesystem>       // Include filesystem operations
#include <fstream>          // Include file stream operations
#include <iostream>         // Include input/output stream operations

using namespace std; // Allows usage of standard library types without std:: prefix

namespace fs = std::filesystem; // Alias for std::filesystem namespace

namespace uni { // Begin namespace uni

// Lists all resource files in the specified folder
vector<ResourceItem> listResources(const string& folder) {
    vector<ResourceItem> items; // Vector to store resource items
    try {
        for (auto& p : fs::directory_iterator(folder)) { // Iterate over files in folder
            if (!p.is_regular_file()) continue; // Skip non-regular files
            auto sz = fs::file_size(p.path()); // Get file size
            items.push_back(ResourceItem{p.path().string(), p.path().filename().string(), sz}); // Add resource item
        }
    } catch (...) {} // Ignore exceptions (e.g., folder not found)
    return items; // Return list of resources
}

// Uploads a local file to the specified resource folder
pair<bool,string> uploadResource(const string& localPath, const string& folder) {
    try {
        ensureDir(folder); // Ensure destination folder exists
        string dst = folder + "/" + fs::path(localPath).filename().string(); // Build destination path
        if (!copyFile(localPath, dst)) return {false, "Copy failed"}; // Copy file, check for failure
        return {true, dst}; // Return success and destination path
    } catch (...) {
        return {false, "Upload failed"}; // Return failure on exception
    }
}

// Downloads a file from resource storage to a local destination
pair<bool,string> downloadResource(const string& storedPath, const string& localDest) {
    try {
        if (!copyFile(storedPath, localDest)) return {false, "Copy failed"}; // Copy file, check for failure
        return {true, localDest}; // Return success and destination path
    } catch (...) {
        return {false, "Download failed"}; // Return failure on exception
    }
}

} // End namespace uni
