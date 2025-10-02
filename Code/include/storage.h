/*
    storage.h

    This header file defines interfaces for file and directory operations used in
    the UniHub-CLI application. It provides functions for managing data and resource
    directories, reading and writing text files, listing files in directories, and
    copying files. These functions abstract the underlying file system operations
    required for persistent storage and resource management in the application.
*/

#pragma once // Ensures this header is included only once during compilation

#include <string>      // Provides std::string for string handling
#include <optional>    // Provides std::optional for optional return values
#include <vector>      // Provides std::vector for dynamic arrays
#include <filesystem> // Provides filesystem utilities (C++17 feature)

using namespace std;   // Allows usage of standard library types without std:: prefix

namespace uni {        // Encapsulates all declarations within the 'uni' namespace

string dataDir();         // Returns the path to the application's data directory

string resourcesDir();    // Returns the path to the application's resources directory

bool ensureDir(const string& path); // Ensures the specified directory exists (creates if needed), returns true on success

optional<string> readTextFile(const string& path); // Reads the contents of a text file, returns content or std::nullopt on failure

optional<string> writeTextFile(const string& path, const string& content); // Writes content to a text file, returns error message or std::nullopt on success

vector<string> listFiles(const string& path); // Lists all files in the specified directory, returns a vector of filenames

bool copyFile(const string& src, const string& dst); // Copies a file from src to dst, returns true on success

} // namespace uni
