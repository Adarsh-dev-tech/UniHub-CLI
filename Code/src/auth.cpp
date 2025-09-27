#include "auth.h"
#include "storage.h"
#include <filesystem>
#include <random>
#include <sstream>
#include <unordered_map>

namespace fs = std::filesystem;

namespace uni {

static std::string sanitizeEmail(const std::string& email) {
    std::string s = email;
    for (char& c : s) { if (c=='/'||c=='\\'||c==' ') c = '_'; }
    return s;
}

std::string usersDir() { return dataDir() + std::string("/users"); }

static std::size_t hashPassword(const std::string& salt, const std::string& password) {
    return std::hash<std::string>{}(salt + password);
}

static std::string randSalt() {
    std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<unsigned long long> dist;
    std::ostringstream oss; oss << std::hex << dist(rng);
    return oss.str();
}

static std::string profilePath(const std::string& email) {
    return usersDir() + std::string("/") + sanitizeEmail(email) + ".profile";
}

static std::string credentialsPath(const std::string& email) {
    return usersDir() + std::string("/") + sanitizeEmail(email) + ".cred";
}

std::optional<std::string> registerUser(const Profile& profile, const std::string& password) {
    ensureDir(usersDir());
    auto credP = credentialsPath(profile.email);
    if (std::filesystem::exists(credP)) {
        return std::optional<std::string>("User already exists");
    }
    std::string salt = randSalt();
    std::size_t h = hashPassword(salt, password);
    // store credentials: salt\n hash
    {
        std::ostringstream oss; oss << salt << "\n" << h;
        if (auto err = writeTextFile(credP, oss.str())) return err;
    }
    // store profile as simple CSV-like
    {
        std::ostringstream oss;
        oss << profile.firstName << "," << profile.lastName << "," << profile.email << "," 
            << profile.year << "," << profile.semester << "," << profile.branch << "," << profile.section;
        if (auto err = writeTextFile(profilePath(profile.email), oss.str())) return err;
    }
    return std::nullopt;
}

std::optional<UserRecord> login(const std::string& email, const std::string& password) {
    auto credP = credentialsPath(email);
    auto content = readTextFile(credP);
    if (!content) return std::nullopt;
    std::istringstream iss(*content);
    std::string salt; std::string hashStr;
    if (!std::getline(iss, salt)) return std::nullopt;
    if (!std::getline(iss, hashStr)) return std::nullopt;
    std::size_t storedHash = 0;
    try { storedHash = std::stoull(hashStr); } catch (...) { return std::nullopt; }
    if (hashPassword(salt, password) != storedHash) return std::nullopt;
    auto p = loadProfile(email);
    if (!p) return std::nullopt;
    return UserRecord{*p, salt, storedHash};
}

std::optional<Profile> loadProfile(const std::string& email) {
    auto path = profilePath(email);
    auto content = readTextFile(path);
    if (!content) return std::nullopt;
    Profile pr;
    std::istringstream iss(*content);
    std::string tok;
    if (!std::getline(iss, pr.firstName, ',')) return std::nullopt;
    if (!std::getline(iss, pr.lastName, ',')) return std::nullopt;
    if (!std::getline(iss, pr.email, ',')) return std::nullopt;
    if (!std::getline(iss, tok, ',')) return std::nullopt;
    pr.year = std::stoi(tok);
    if (!std::getline(iss, tok, ',')) return std::nullopt;
    pr.semester = std::stoi(tok);
    if (!std::getline(iss, pr.branch, ',')) return std::nullopt;
    if (!std::getline(iss, tok, ',')) return std::nullopt;
    pr.section = tok.empty()? 'A' : tok[0];
    return pr;
}

std::optional<std::string> saveProfile(const Profile& profile) {
    return writeTextFile(profilePath(profile.email),
        profile.firstName + "," + profile.lastName + "," + profile.email + "," +
        std::to_string(profile.year) + "," + std::to_string(profile.semester) + "," + profile.branch + "," + profile.section);
}

}
