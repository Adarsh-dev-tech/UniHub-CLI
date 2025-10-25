#pragma once
#include "auth.h"
#include "data_structures.h"
#include <unordered_map>
#include <list>
#include <memory>
#include <optional>

namespace uni {

// ============================================================================
// Enhanced User Management with Hybrid Data Structures
// ============================================================================
class UserManager {
private:
    // Hash Table: O(1) email lookup
    std::unordered_map<std::string, std::shared_ptr<UserRecord>> emailIndex;
    
    // AVL Tree: Sorted user access (by email)
    AVLTree<std::string> sortedEmails;
    
    // LRU Cache: Recently active users
    std::list<std::string> recentUsers;
    std::unordered_map<std::string, std::list<std::string>::iterator> recentIndex;
    static const size_t MAX_RECENT = 10;
    
    // Social Graph: User connections (study groups, friends)
    Graph<std::string> socialGraph;
    
    void updateRecentAccess(const std::string& email) {
        // Remove from current position if exists
        auto it = recentIndex.find(email);
        if (it != recentIndex.end()) {
            recentUsers.erase(it->second);
        }
        
        // Add to front
        recentUsers.push_front(email);
        recentIndex[email] = recentUsers.begin();
        
        // Maintain max size
        while (recentUsers.size() > MAX_RECENT) {
            std::string last = recentUsers.back();
            recentUsers.pop_back();
            recentIndex.erase(last);
        }
    }

public:
    UserManager() : sortedEmails([](const std::string& a, const std::string& b) { return a < b; }) {}
    
    // Register new user
    std::optional<std::string> registerUser(const Profile& profile, const std::string& password) {
        if (emailIndex.find(profile.email) != emailIndex.end()) {
            return "User already exists";
        }
        
        // Create user record using existing auth logic
        auto error = uni::registerUser(profile, password);
        if (error) return error;
        
        // Load the created user record
        auto userRecord = uni::login(profile.email, password);
        if (!userRecord) return "Failed to load created user";
        
        // Add to all data structures
        auto userPtr = std::make_shared<UserRecord>(*userRecord);
        emailIndex[profile.email] = userPtr;
        sortedEmails.insert(profile.email);
        socialGraph.addNode(profile.email);
        updateRecentAccess(profile.email);
        
        return std::nullopt;
    }
    
    // Login user
    std::optional<UserRecord> loginUser(const std::string& email, const std::string& password) {
        // Try hash table first (O(1))
        auto it = emailIndex.find(email);
        if (it != emailIndex.end()) {
            // Verify password using existing auth
            auto userRecord = uni::login(email, password);
            if (userRecord) {
                updateRecentAccess(email);
                return *userRecord;
            }
        }
        
        // Fallback to file-based auth for users not in memory
        auto userRecord = uni::login(email, password);
        if (userRecord) {
            // Add to memory structures
            auto userPtr = std::make_shared<UserRecord>(*userRecord);
            emailIndex[email] = userPtr;
            sortedEmails.insert(email);
            socialGraph.addNode(email);
            updateRecentAccess(email);
        }
        
        return userRecord;
    }
    
    // Get all users sorted by email
    std::vector<std::string> getSortedUsers() {
        return sortedEmails.getSorted();
    }
    
    // Get recently active users
    std::vector<std::string> getRecentUsers() {
        return std::vector<std::string>(recentUsers.begin(), recentUsers.end());
    }
    
    // Add friendship/study group connection
    void addConnection(const std::string& user1, const std::string& user2) {
        socialGraph.addEdge(user1, user2);
    }
    
    // Get connected users (friends/study group members)
    std::vector<std::string> getConnections(const std::string& email) {
        return socialGraph.getConnected(email);
    }
    
    // Update profile
    std::optional<std::string> updateProfile(const Profile& profile) {
        auto error = uni::saveProfile(profile);
        if (error) return error;
        
        // Update in-memory record if it exists
        auto it = emailIndex.find(profile.email);
        if (it != emailIndex.end()) {
            it->second->profile = profile;
        }
        
        return std::nullopt;
    }
    
    // Search users by email prefix
    std::vector<std::string> searchUsersByPrefix(const std::string& prefix) {
        std::vector<std::string> results;
        auto sorted = getSortedUsers();
        
        for (const auto& email : sorted) {
            if (email.substr(0, prefix.length()) == prefix) {
                results.push_back(email);
            }
        }
        
        return results;
    }
};

}