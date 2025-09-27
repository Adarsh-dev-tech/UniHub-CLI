#pragma once
#include <string>
#include <optional>

namespace uni {

struct Profile {
    std::string firstName;
    std::string lastName;
    std::string email; // college email (unique key)
    int year{};        // 1..5
    int semester{};    // 1..10
    std::string branch; // one of 10 branches
    char section{'A'};  // 'A' or 'B'
};

struct UserRecord {
    Profile profile;
    std::string salt;
    std::size_t passwordHash; // hash(salt + password)
};

// Returns error message on failure
std::optional<std::string> registerUser(const Profile& profile, const std::string& password);

// On success, returns loaded UserRecord; otherwise std::nullopt
std::optional<UserRecord> login(const std::string& email, const std::string& password);

// Load and save profile by email
std::optional<Profile> loadProfile(const std::string& email);
std::optional<std::string> saveProfile(const Profile& profile);

std::string usersDir();

}
