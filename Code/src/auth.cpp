/*
    auth.cpp

    This source file implements the authentication and user profile management logic
    for the UniHub-CLI application. It provides functions for registering users,
    logging in, loading and saving profiles, and handling user credential storage.
    The file interacts with the file system to persist user data and credentials,
    and uses hashing and random salt generation for password security.
*/

#include "auth.h"         // Includes the authentication and profile interface definitions
#include "storage.h"      // Includes file and directory utility functions
#include <filesystem>     // Provides file system operations (e.g., checking file existence)
#include <random>         // Provides random number generation for salt creation
#include <sstream>        // Provides string stream utilities for parsing and formatting
#include <unordered_map>  // Provides hash map containers (may be used elsewhere)

using namespace std; // Allows usage of standard library types without std:: prefix

namespace fs = filesystem; // Alias for filesystem namespace

namespace uni { // Begin namespace uni

// Helper function to sanitize email addresses for use as filenames
static string sanitizeEmail(const string& email) {
    string s = email;
    for (char& c : s) { if (c=='/'||c=='\\'||c==' ') c = '_'; } // Replace invalid filename characters with '_'
    return s;
}

// Returns the directory path where user data is stored
string usersDir() { return dataDir() + string("/users"); }

// Hashes the password using the provided salt
static size_t hashPassword(const string& salt, const string& password) {
    return hash<string>{}(salt + password); // Hashes the concatenated salt and password
}

// Generates a random salt string for password hashing
static string randSalt() {
    mt19937_64 rng(random_device{}()); // Random number generator
    uniform_int_distribution<unsigned long long> dist; // Distribution for random numbers
    ostringstream oss; oss << hex << dist(rng); // Convert random number to hex string
    return oss.str();
}

// Builds the file path for storing a user's profile
static string profilePath(const string& email) {
    return usersDir() + string("/") + sanitizeEmail(email) + ".profile";
}

// Builds the file path for storing a user's credentials
static string credentialsPath(const string& email) {
    return usersDir() + string("/") + sanitizeEmail(email) + ".cred";
}

// Registers a new user with the given profile and password
// Returns an error message on failure, or std::nullopt on success
optional<string> registerUser(const Profile& profile, const string& password) {
    ensureDir(usersDir()); // Ensure the users directory exists
    auto credP = credentialsPath(profile.email); // Get credentials file path
    if (filesystem::exists(credP)) { // Check if user already exists
        return optional<string>("User already exists");
    }
    string salt = randSalt(); // Generate random salt
    size_t h = hashPassword(salt, password); // Hash the password with salt
    // Store credentials: salt and hash, separated by newline
    {
        ostringstream oss; oss << salt << "\n" << h;
        if (auto err = writeTextFile(credP, oss.str())) return err; // Write credentials to file
    }
    // Store profile as a simple CSV-like string
    {
        ostringstream oss;
        oss << profile.firstName << "," << profile.lastName << "," << profile.email << "," 
            << profile.year << "," << profile.semester << "," << profile.branch << "," << profile.section;
        if (auto err = writeTextFile(profilePath(profile.email), oss.str())) return err; // Write profile to file
    }
    return nullopt; // Success
}

// Attempts to log in a user with the given email and password
// Returns the loaded UserRecord on success, or std::nullopt on failure
optional<UserRecord> login(const string& email, const string& password) {
    auto credP = credentialsPath(email); // Get credentials file path
    auto content = readTextFile(credP); // Read credentials file
    if (!content) return nullopt; // Fail if file not found
    istringstream iss(*content); // Parse credentials
    string salt; string hashStr;
    if (!getline(iss, salt)) return nullopt; // Read salt
    if (!getline(iss, hashStr)) return nullopt; // Read hash
    size_t storedHash = 0;
    try { storedHash = stoull(hashStr); } catch (...) { return nullopt; } // Convert hash to integer
    if (hashPassword(salt, password) != storedHash) return nullopt; // Check password
    auto p = loadProfile(email); // Load user profile
    if (!p) return nullopt; // Fail if profile not found
    return UserRecord{*p, salt, storedHash}; // Return user record
}

// Loads a user's profile by their email address
// Returns the Profile on success, or std::nullopt if not found
optional<Profile> loadProfile(const string& email) {
    auto path = profilePath(email); // Get profile file path
    auto content = readTextFile(path); // Read profile file
    if (!content) return nullopt; // Fail if file not found
    Profile pr;
    istringstream iss(*content); // Parse CSV-like profile string
    string tok;
    if (!getline(iss, pr.firstName, ',')) return nullopt;
    if (!getline(iss, pr.lastName, ',')) return nullopt;
    if (!getline(iss, pr.email, ',')) return nullopt;
    if (!getline(iss, tok, ',')) return nullopt;
    pr.year = stoi(tok);
    if (!getline(iss, tok, ',')) return nullopt;
    pr.semester = stoi(tok);
    if (!getline(iss, pr.branch, ',')) return nullopt;
    if (!getline(iss, tok, ',')) return nullopt;
    pr.section = tok.empty()? 'A' : tok[0];
    return pr;
}

// Saves the given profile information
// Returns an error message on failure, or std::nullopt on success
optional<string> saveProfile(const Profile& profile) {
    return writeTextFile(profilePath(profile.email),
        profile.firstName + "," + profile.lastName + "," + profile.email + "," +
        to_string(profile.year) + "," + to_string(profile.semester) + "," + profile.branch + "," + profile.section);
}

} // End namespace uni
