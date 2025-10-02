/*
    auth.h

    This header file defines the authentication and user profile management interfaces
    for the UniHub-CLI application. It provides structures to represent user profiles
    and user records, and declares functions for registering users, logging in,
    loading and saving profiles, and retrieving the users directory path.

    The functions and structures here are used to manage user authentication,
    profile storage, and access control throughout the application.
*/

#pragma once // Ensures this header is included only once during compilation

#include <string>      // Provides the std::string type
#include <optional>    // Provides std::optional for optional return values

using namespace std;   // Allows usage of standard library types without std:: prefix

namespace uni {        // All declarations are encapsulated in the 'uni' namespace

// Structure representing a user's profile information
struct Profile {
    string firstName;      // User's first name
    string lastName;       // User's last name
    string email;          // User's college email (serves as a unique key)
    int year{};            // Academic year (values from 1 to 5)
    int semester{};        // Semester number (values from 1 to 10)
    string branch;         // Branch of study (one of 10 possible branches)
    char section{'A'};     // Section identifier ('A' or 'B')
};

// Structure representing a user's authentication record
struct UserRecord {
    Profile profile;       // Embedded user profile information
    string salt;           // Salt used for password hashing
    size_t passwordHash;   // Hashed value of (salt + password)
};

// Registers a new user with the given profile and password
// Returns an error message on failure, or std::nullopt on success
optional<string> registerUser(const Profile& profile, const string& password);

// Attempts to log in a user with the given email and password
// On success, returns the loaded UserRecord; otherwise returns std::nullopt
optional<UserRecord> login(const string& email, const string& password);

// Loads a user's profile by their email address
// Returns the Profile on success, or std::nullopt if not found
optional<Profile> loadProfile(const string& email);

// Saves the given profile information
// Returns an error message on failure, or std::nullopt on success
optional<string> saveProfile(const Profile& profile);

// Returns the directory path where user data is stored
string usersDir();

} // namespace uni
