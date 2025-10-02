/*
    resources.h

    This header file defines the interfaces and structures for managing resource files
    within the UniHub-CLI application. It provides functionality for listing, uploading,
    and downloading files in resource folders. The ResourceItem structure represents
    metadata for each resource file, and the declared functions allow interaction
    with the resource storage system.

    These interfaces are used to facilitate file sharing and management among users
    in the application.
*/

#pragma once // Ensures this header is included only once during compilation

#include <string>      // Provides std::string for string handling
#include <vector>      // Provides std::vector for dynamic arrays
#include <utility>    // Provides std::pair for returning multiple values

using namespace std;   // Allows usage of standard library types without std:: prefix

namespace uni {        // Encapsulates all declarations within the 'uni' namespace

// Structure representing metadata for a resource file
struct ResourceItem {
    string filename;     // Name of the stored file
    string displayName;  // Display name or title for the resource
    size_t sizeBytes{};  // Size of the file in bytes, default initialized to 0
};

// Lists all files under the specified resource folder
// Returns a vector of ResourceItem objects representing each file
vector<ResourceItem> listResources(const string& folder);

// Uploads a local file to the specified resource folder
// Returns a pair: (success flag, destination path or error message)
pair<bool,string> uploadResource(const string& localPath, const string& folder);

// Downloads a file from the resource folder to a local destination
// Returns a pair: (success flag, message indicating result)
pair<bool,string> downloadResource(const string& storedPath, const string& localDest);

} // namespace uni
