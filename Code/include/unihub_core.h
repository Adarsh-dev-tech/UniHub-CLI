#pragma once
#include "user_manager.h"
#include "academic_manager.h"
#include "resource_index.h"
#include <stack>
#include <string>
#include <vector>

namespace uni {

// ============================================================================
// Navigation System with Stack-based History
// ============================================================================
struct NavigationState {
    std::string location;
    std::string description;
    std::unordered_map<std::string, std::string> context;
    
    NavigationState(const std::string& loc, const std::string& desc) 
        : location(loc), description(desc) {}
};

class NavigationManager {
private:
    std::stack<NavigationState> history;
    NavigationState currentState;
    
public:
    NavigationManager() : currentState("main_menu", "Main Menu") {}
    
    void navigateTo(const std::string& location, const std::string& description, 
                   const std::unordered_map<std::string, std::string>& context = {}) {
        history.push(currentState);
        currentState = NavigationState(location, description);
        currentState.context = context;
    }
    
    bool goBack() {
        if (!history.empty()) {
            currentState = history.top();
            history.pop();
            return true;
        }
        return false;
    }
    
    std::string getCurrentLocation() const { return currentState.location; }
    std::string getCurrentDescription() const { return currentState.description; }
    
    std::vector<std::string> getBreadcrumbs() {
        std::vector<std::string> breadcrumbs;
        std::stack<NavigationState> temp = history;
        
        while (!temp.empty()) {
            breadcrumbs.insert(breadcrumbs.begin(), temp.top().description);
            temp.pop();
        }
        
        breadcrumbs.push_back(currentState.description);
        return breadcrumbs;
    }
    
    std::string getContext(const std::string& key) {
        auto it = currentState.context.find(key);
        if (it != currentState.context.end()) {
            return it->second;
        }
        return "";
    }
    
    void setContext(const std::string& key, const std::string& value) {
        currentState.context[key] = value;
    }
};

// ============================================================================
// Central Hub Managing All Data Structures
// ============================================================================
class UniHubCore {
private:
    UserManager userManager;
    AcademicManager academicManager;
    ResourceIndex resourceIndex;
    NavigationManager navigationManager;
    
    // Current session
    std::optional<UserRecord> currentUser;

public:
    UniHubCore() = default;
    
    // User Management
    std::optional<std::string> registerUser(const Profile& profile, const std::string& password) {
        return userManager.registerUser(profile, password);
    }
    
    std::optional<UserRecord> loginUser(const std::string& email, const std::string& password) {
        auto user = userManager.loginUser(email, password);
        if (user) {
            currentUser = user;
            navigationManager.navigateTo("main_menu", "Main Menu");
            navigationManager.setContext("user_email", email);
        }
        return user;
    }
    
    void logoutUser() {
        currentUser.reset();
        navigationManager = NavigationManager(); // Reset navigation
    }
    
    std::optional<UserRecord> getCurrentUser() const { return currentUser; }
    
    std::vector<std::string> getSortedUsers() { return userManager.getSortedUsers(); }
    std::vector<std::string> getRecentUsers() { return userManager.getRecentUsers(); }
    std::vector<std::string> searchUsersByPrefix(const std::string& prefix) {
        return userManager.searchUsersByPrefix(prefix);
    }
    
    // Academic Management
    std::vector<EnhancedSubject> getSubjects(int year, int semester, const std::string& branch, char section) {
        return academicManager.getSubjects(year, semester, branch, section);
    }
    
    std::vector<std::string> getPrerequisites(const std::string& subjectCode) {
        return academicManager.getPrerequisites(subjectCode);
    }
    
    std::vector<std::string> getSuggestedCourseSequence() {
        return academicManager.getSuggestedCourseSequence();
    }
    
    bool canTakeSubject(const std::string& subjectCode, const std::vector<std::string>& completedSubjects) {
        return academicManager.canTakeSubject(subjectCode, completedSubjects);
    }
    
    std::optional<EnhancedSubject> getSubject(const std::string& code) {
        return academicManager.getSubject(code);
    }
    
    // Resource Management
    void addResource(const ResourceMetadata& resource) {
        resourceIndex.addResource(resource);
    }
    
    std::vector<std::string> autocompleteResourceName(const std::string& prefix) {
        return resourceIndex.autocompleteResourceName(prefix);
    }
    
    std::vector<ResourceMetadata> getPopularResources(int count = 10) {
        return resourceIndex.getPopularResources(count);
    }
    
    std::vector<ResourceMetadata> searchResourcesByKeyword(const std::string& keyword) {
        return resourceIndex.searchByKeyword(keyword);
    }
    
    std::vector<ResourceMetadata> getResourcesByTag(const std::string& tag) {
        return resourceIndex.getResourcesByTag(tag);
    }
    
    std::vector<std::string> getRelatedResources(const std::string& resourceFilename) {
        return resourceIndex.getRelatedResources(resourceFilename);
    }
    
    void incrementDownloadCount(const std::string& filename) {
        resourceIndex.incrementDownloadCount(filename);
    }
    
    // Navigation Management
    void navigateTo(const std::string& location, const std::string& description,
                   const std::unordered_map<std::string, std::string>& context = {}) {
        navigationManager.navigateTo(location, description, context);
    }
    
    bool goBack() { return navigationManager.goBack(); }
    
    std::string getCurrentLocation() const { return navigationManager.getCurrentLocation(); }
    std::string getCurrentDescription() const { return navigationManager.getCurrentDescription(); }
    
    std::vector<std::string> getBreadcrumbs() { return navigationManager.getBreadcrumbs(); }
    
    std::string getNavigationContext(const std::string& key) {
        return navigationManager.getContext(key);
    }
    
    void setNavigationContext(const std::string& key, const std::string& value) {
        navigationManager.setContext(key, value);
    }
    
    // Profile Management
    std::optional<std::string> updateProfile(const Profile& profile) {
        auto error = userManager.updateProfile(profile);
        if (!error && currentUser) {
            currentUser->profile = profile;
        }
        return error;
    }
};

}