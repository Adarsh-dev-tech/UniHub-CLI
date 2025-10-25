#pragma once
#include "unihub_core.h"
#include "storage.h"
#include "resources.h"
#include <iostream>
#include <limits>
#include <filesystem>
#include <algorithm>

namespace uni {

class EnhancedMenu {
private:
    UniHubCore core;
    
    void clearScreen() {
        // Simple screen clear for demo purposes
        for (int i = 0; i < 50; ++i) std::cout << "\n";
    }
    
    void showBreadcrumbs() {
        auto breadcrumbs = core.getBreadcrumbs();
        std::cout << "Navigation: ";
        for (size_t i = 0; i < breadcrumbs.size(); ++i) {
            if (i > 0) std::cout << " > ";
            std::cout << breadcrumbs[i];
        }
        std::cout << "\n";
    }
    
    void pause() {
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    bool registerFlow() {
        std::cout << "\n===== User Registration =====\n";
        Profile p;
        std::string password, confirm;
        
        std::cout << "First name: "; std::getline(std::cin, p.firstName);
        std::cout << "Last name: "; std::getline(std::cin, p.lastName);
        std::cout << "College email: "; std::getline(std::cin, p.email);
        std::cout << "Password: "; std::getline(std::cin, password);
        std::cout << "Confirm password: "; std::getline(std::cin, confirm);
        
        if (password != confirm) {
            std::cout << "Passwords do not match.\n";
            return false;
        }
        
        std::cout << "Year (1-5): "; std::cin >> p.year; std::cin.ignore(1,'\n');
        std::cout << "Semester (1-10): "; std::cin >> p.semester; std::cin.ignore(1,'\n');
        std::cout << "Branch (full name or code): "; std::getline(std::cin, p.branch);
        
        // Branch normalization
        std::string b = p.branch;
        std::transform(b.begin(), b.end(), b.begin(), ::toupper);
        if (b == "COMPUTER SCIENCE AND ENGINEERING" || b == "CSE") p.branch = "CSE";
        else if (b == "ELECTRONICS AND COMMUNICATION ENGINEERING" || b == "ECE") p.branch = "ECE";
        else if (b == "ELECTRICAL AND ELECTRONICS ENGINEERING" || b == "EEE") p.branch = "EEE";
        else if (b == "INSTRUMENTATION AND CONTROL ENGINEERING" || b == "ICE") p.branch = "ICE";
        else if (b == "MECHANICAL ENGINEERING" || b == "ME") p.branch = "ME";
        else if (b == "CHEMICAL ENGINEERING" || b == "CHE") p.branch = "CHE";
        else if (b == "PRODUCTION ENGINEERING" || b == "PROD") p.branch = "PROD";
        else if (b == "CIVIL ENGINEERING" || b == "CIVIL") p.branch = "CIVIL";
        else if (b == "METALLURGICAL AND MATERIALS ENGINEERING" || b == "MME") p.branch = "MME";
        else if (b == "ARCHITECTURE" || b == "ARCH") p.branch = "ARCH";
        else {
            std::cout << "Unknown branch, defaulting to CSE.\n";
            p.branch = "CSE";
        }
        
        std::cout << "Section (A/B): ";
        std::string s; std::getline(std::cin, s);
        p.section = s.empty() ? 'A' : std::toupper(s[0]);
        
        auto error = core.registerUser(p, password);
        if (error) {
            std::cout << *error << "\n";
            return false;
        }
        
        std::cout << "Registered successfully! You can now login.\n";
        return true;
    }
    
    std::optional<UserRecord> loginFlow() {
        std::cout << "\n===== User Login =====\n";
        std::string email, password;
        
        // Show recent users for convenience
        auto recentUsers = core.getRecentUsers();
        if (!recentUsers.empty()) {
            std::cout << "Recent users:\n";
            for (size_t i = 0; i < recentUsers.size() && i < 5; ++i) {
                std::cout << "  " << (i+1) << ") " << recentUsers[i] << "\n";
            }
            std::cout << "\n";
        }
        
        std::cout << "Email: "; std::getline(std::cin, email);
        std::cout << "Password: "; std::getline(std::cin, password);
        
        auto user = core.loginUser(email, password);
        if (!user) {
            std::cout << "Invalid credentials.\n";
        }
        
        return user;
    }
    
    void showProfile() {
        auto user = core.getCurrentUser();
        if (!user) return;
        
        core.navigateTo("profile", "Profile View");
        
        std::cout << "\n===== Your Profile =====\n";
        const auto& p = user->profile;
        std::cout << "Name: " << p.firstName << " " << p.lastName << "\n";
        std::cout << "Email: " << p.email << "\n";
        std::cout << "Year/Semester: " << p.year << "/" << p.semester << "\n";
        std::cout << "Branch: " << p.branch << "  Section: " << p.section << "\n";
        
        std::cout << "\nOptions:\n";
        std::cout << "1) Edit Profile\n";
        std::cout << "2) View Prerequisites for Current Semester\n";
        std::cout << "0) Back\n";
        std::cout << "Choose: ";
        
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            return;
        }
        std::cin.ignore(1, '\n');
        
        if (choice == 1) {
            editProfile();
        } else if (choice == 2) {
            showPrerequisites();
        }
    }
    
    void editProfile() {
        auto user = core.getCurrentUser();
        if (!user) return;
        
        Profile p = user->profile;
        std::string input;
        
        std::cout << "\n===== Edit Profile (leave blank to keep current) =====\n";
        
        std::cout << "First name [" << p.firstName << "]: ";
        std::getline(std::cin, input);
        if (!input.empty()) p.firstName = input;
        
        std::cout << "Last name [" << p.lastName << "]: ";
        std::getline(std::cin, input);
        if (!input.empty()) p.lastName = input;
        
        std::cout << "Year [" << p.year << "]: ";
        std::getline(std::cin, input);
        if (!input.empty()) p.year = std::stoi(input);
        
        std::cout << "Semester [" << p.semester << "]: ";
        std::getline(std::cin, input);
        if (!input.empty()) p.semester = std::stoi(input);
        
        std::cout << "Branch [" << p.branch << "]: ";
        std::getline(std::cin, input);
        if (!input.empty()) p.branch = input;
        
        std::cout << "Section [" << p.section << "]: ";
        std::getline(std::cin, input);
        if (!input.empty()) p.section = std::toupper(input[0]);
        
        auto error = core.updateProfile(p);
        if (error) {
            std::cout << "Error updating profile: " << *error << "\n";
        } else {
            std::cout << "Profile updated successfully!\n";
        }
    }
    
    void showPrerequisites() {
        auto user = core.getCurrentUser();
        if (!user) return;
        
        const auto& profile = user->profile;
        auto subjects = core.getSubjects(profile.year, profile.semester, profile.branch, profile.section);
        
        std::cout << "\n===== Subject Prerequisites =====\n";
        
        for (const auto& subject : subjects) {
            std::cout << "\n" << subject.name << " (" << subject.code << "):\n";
            auto prereqs = core.getPrerequisites(subject.code);
            
            if (prereqs.empty()) {
                std::cout << "  No prerequisites\n";
            } else {
                std::cout << "  Prerequisites:\n";
                for (const auto& prereq : prereqs) {
                    auto prereqSubject = core.getSubject(prereq);
                    if (prereqSubject) {
                        std::cout << "    - " << prereqSubject->name << " (" << prereq << ")\n";
                    } else {
                        std::cout << "    - " << prereq << "\n";
                    }
                }
            }
        }
        
        pause();
    }
    
    void showSubjectsMenu() {
        auto user = core.getCurrentUser();
        if (!user) return;
        
        core.navigateTo("subjects", "Subjects");
        
        const auto& profile = user->profile;
        auto subjects = core.getSubjects(profile.year, profile.semester, profile.branch, profile.section);
        
        if (subjects.empty()) {
            std::cout << "No subjects found for your year/semester/branch/section.\n";
            pause();
            return;
        }
        
        while (true) {
            clearScreen();
            showBreadcrumbs();
            
            std::cout << "\n===== Your Subjects =====\n";
            std::cout << "Year " << profile.year << ", Semester " << profile.semester;
            std::cout << ", " << profile.branch << " Section " << profile.section << "\n\n";
            
            for (size_t i = 0; i < subjects.size(); ++i) {
                const auto& subject = subjects[i];
                std::cout << (i+1) << ") " << subject.name << "\n";
                std::cout << "   Teacher: " << subject.teacher << "\n";
                std::cout << "   Code: " << subject.code << " | Credits: " << subject.credits << "\n";
                
                auto prereqs = core.getPrerequisites(subject.code);
                if (!prereqs.empty()) {
                    std::cout << "   Prerequisites: ";
                    for (size_t j = 0; j < prereqs.size(); ++j) {
                        if (j > 0) std::cout << ", ";
                        std::cout << prereqs[j];
                    }
                    std::cout << "\n";
                }
                std::cout << "\n";
            }
            
            std::cout << "s) Search Resources\n";
            std::cout << "p) Popular Resources\n";
            std::cout << "0) Back\n";
            std::cout << "Choose (1-" << subjects.size() << " or option): ";
            
            std::string input;
            std::getline(std::cin, input);
            
            if (input == "0") break;
            if (input == "s") {
                showResourceSearch();
                continue;
            }
            if (input == "p") {
                showPopularResources();
                continue;
            }
            
            try {
                int choice = std::stoi(input);
                if (choice >= 1 && choice <= (int)subjects.size()) {
                    showSubjectResources(subjects[choice-1]);
                }
            } catch (...) {
                // Invalid input, continue loop
            }
        }
    }
    
    void showResourceSearch() {
        core.navigateTo("search", "Resource Search");
        
        std::cout << "\n===== Resource Search =====\n";
        std::cout << "Enter search keyword: ";
        std::string keyword;
        std::getline(std::cin, keyword);
        
        auto results = core.searchResourcesByKeyword(keyword);
        
        std::cout << "\nSearch Results (" << results.size() << " found):\n";
        for (const auto& resource : results) {
            std::cout << "- " << resource.displayName;
            std::cout << " (" << resource.subject << " - " << resource.resourceType << ")\n";
            std::cout << "  Uploaded by: " << resource.uploader;
            std::cout << " | Downloads: " << resource.downloadCount << "\n\n";
        }
        
        pause();
    }
    
    void showPopularResources() {
        core.navigateTo("popular", "Popular Resources");
        
        auto popular = core.getPopularResources(10);
        
        std::cout << "\n===== Most Popular Resources =====\n";
        for (size_t i = 0; i < popular.size(); ++i) {
            const auto& resource = popular[i];
            std::cout << (i+1) << ") " << resource.displayName << "\n";
            std::cout << "   Subject: " << resource.subject;
            std::cout << " | Type: " << resource.resourceType << "\n";
            std::cout << "   Downloads: " << resource.downloadCount;
            std::cout << " | Rating: " << resource.rating << "/5.0\n\n";
        }
        
        pause();
    }
    
    void showSubjectResources(const EnhancedSubject& subject) {
        core.navigateTo("subject_resources", subject.name);
        core.setNavigationContext("subject_code", subject.code);
        
        while (true) {
            clearScreen();
            showBreadcrumbs();
            
            std::cout << "\n===== " << subject.name << " Resources =====\n";
            std::cout << "Teacher: " << subject.teacher << " | Code: " << subject.code << "\n\n";
            
            // Show resource types
            for (size_t i = 0; i < kResourceTypes.size(); ++i) {
                std::cout << (i+1) << ") " << kResourceTypes[i] << "\n";
            }
            
            std::cout << "\nr) Related Resources\n";
            std::cout << "0) Back\n";
            std::cout << "Choose: ";
            
            int choice;
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                continue;
            }
            std::cin.ignore(1, '\n');
            
            if (choice == 0) break;
            if (choice >= 1 && choice <= (int)kResourceTypes.size()) {
                showResourceType(subject, kResourceTypes[choice-1]);
            }
        }
    }
    
    void showResourceType(const EnhancedSubject& subject, const std::string& type) {
        core.navigateTo("resource_type", type);
        
        // For now, use the original file-based system
        // In a full implementation, we'd integrate with ResourceIndex
        std::string folder = resourcesBase(subject.year, subject.semester, subject.branch, 
                                         subject.section, subject.name, type);
        
        while (true) {
            clearScreen();
            showBreadcrumbs();
            
            auto items = listResources(folder);
            
            std::cout << "\n===== " << type << " for " << subject.name << " =====\n";
            
            for (size_t i = 0; i < items.size(); ++i) {
                std::cout << (i+1) << ") " << items[i].displayName;
                std::cout << " (" << items[i].sizeBytes << " bytes)\n";
            }
            
            std::cout << "\na) Upload\n";
            std::cout << "d) Download\n";
            std::cout << "s) Search in this type\n";
            std::cout << "0) Back\n";
            std::cout << "Choose: ";
            
            std::string option;
            std::getline(std::cin, option);
            
            if (option == "0") break;
            if (option == "a") {
                uploadResource(folder);
            } else if (option == "d") {
                downloadResource(items);
            } else if (option == "s") {
                searchInResourceType(type);
            } else {
                try {
                    int idx = std::stoi(option);
                    if (idx >= 1 && idx <= (int)items.size()) {
                        showResourceDetails(items[idx-1]);
                    }
                } catch (...) {
                    // Invalid input
                }
            }
        }
    }
    
    void uploadResource(const std::string& folder) {
        std::cout << "\nEnter local file path to upload: ";
        std::string localPath;
        std::getline(std::cin, localPath);
        
        auto [success, message] = uni::uploadResource(localPath, folder);
        
        if (success) {
            // Create ResourceMetadata for the hybrid system
            ResourceMetadata metadata;
            metadata.filename = message; // uploadResource returns the destination path
            metadata.displayName = std::filesystem::path(localPath).filename().string();
            metadata.filePath = message;
            metadata.resourceType = core.getCurrentDescription();
            metadata.subject = core.getNavigationContext("subject_code");
            
            auto user = core.getCurrentUser();
            if (user) {
                metadata.uploader = user->profile.email;
            }
            
            try {
                metadata.sizeBytes = std::filesystem::file_size(localPath);
            } catch (...) {
                metadata.sizeBytes = 0;
            }
            
            core.addResource(metadata);
            std::cout << "Uploaded successfully: " << message << "\n";
        } else {
            std::cout << "Upload failed: " << message << "\n";
        }
        
        pause();
    }
    
    void downloadResource(const std::vector<ResourceItem>& items) {
        if (items.empty()) {
            std::cout << "No files available for download.\n";
            pause();
            return;
        }
        
        std::cout << "Enter file number (1-" << items.size() << "): ";
        int idx;
        if (!(std::cin >> idx) || idx < 1 || idx > (int)items.size()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid selection.\n";
            pause();
            return;
        }
        std::cin.ignore(1, '\n');
        
        std::cout << "Enter destination file path (full path including filename): ";
        std::string destPath;
        std::getline(std::cin, destPath);
        
        auto [success, message] = uni::downloadResource(items[idx-1].filename, destPath);
        
        if (success) {
            // Update download count in hybrid system
            core.incrementDownloadCount(items[idx-1].filename);
            std::cout << "Downloaded successfully: " << message << "\n";
        } else {
            std::cout << "Download failed: " << message << "\n";
        }
        
        pause();
    }
    
    void searchInResourceType(const std::string& type) {
        std::cout << "\nSearch " << type << ": ";
        std::string query;
        std::getline(std::cin, query);
        
        auto suggestions = core.autocompleteResourceName(query);
        
        if (!suggestions.empty()) {
            std::cout << "\nSuggestions:\n";
            for (const auto& suggestion : suggestions) {
                std::cout << "- " << suggestion << "\n";
            }
        } else {
            std::cout << "No suggestions found.\n";
        }
        
        pause();
    }
    
    void showResourceDetails(const ResourceItem& item) {
        std::cout << "\n===== Resource Details =====\n";
        std::cout << "Name: " << item.displayName << "\n";
        std::cout << "Size: " << item.sizeBytes << " bytes\n";
        std::cout << "Path: " << item.filename << "\n";
        
        auto related = core.getRelatedResources(item.filename);
        if (!related.empty()) {
            std::cout << "\nRelated Resources:\n";
            for (const auto& rel : related) {
                std::cout << "- " << rel << "\n";
            }
        }
        
        pause();
    }

public:
    void run() {
        ensureDir(dataDir());
        ensureDir(usersDir());
        ensureDir(resourcesDir());
        
        while (true) {
            clearScreen();
            
            std::cout << "===== Welcome to UniHub CLI (Enhanced) =====\n";
            std::cout << "Hybrid Data Structure Version\n\n";
            
            auto currentUser = core.getCurrentUser();
            if (currentUser) {
                std::cout << "Logged in as: " << currentUser->profile.firstName;
                std::cout << " " << currentUser->profile.lastName;
                std::cout << " (" << currentUser->profile.email << ")\n\n";
                
                showBreadcrumbs();
                
                std::cout << "\n1) Profile\n";
                std::cout << "2) Subjects & Resources\n";
                std::cout << "3) Search All Resources\n";
                std::cout << "4) Popular Resources\n";
                std::cout << "5) User Directory\n";
                std::cout << "0) Logout\n";
                std::cout << "Choose: ";
                
                int choice;
                if (!(std::cin >> choice)) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    continue;
                }
                std::cin.ignore(1, '\n');
                
                switch (choice) {
                    case 0:
                        core.logoutUser();
                        break;
                    case 1:
                        showProfile();
                        break;
                    case 2:
                        showSubjectsMenu();
                        break;
                    case 3:
                        showResourceSearch();
                        break;
                    case 4:
                        showPopularResources();
                        break;
                    case 5:
                        showUserDirectory();
                        break;
                }
            } else {
                std::cout << "1) Login\n";
                std::cout << "2) Register\n";
                std::cout << "0) Exit\n";
                std::cout << "Choose: ";
                
                int choice;
                if (!(std::cin >> choice)) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    continue;
                }
                std::cin.ignore(1, '\n');
                
                if (choice == 0) break;
                if (choice == 1) loginFlow();
                if (choice == 2) registerFlow();
            }
        }
        
        std::cout << "\nGoodbye!\n";
    }
    
    void showUserDirectory() {
        core.navigateTo("user_directory", "User Directory");
        
        std::cout << "\n===== User Directory =====\n";
        std::cout << "1) Browse All Users (Sorted)\n";
        std::cout << "2) Recent Users\n";
        std::cout << "3) Search Users\n";
        std::cout << "0) Back\n";
        std::cout << "Choose: ";
        
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            return;
        }
        std::cin.ignore(1, '\n');
        
        if (choice == 1) {
            auto users = core.getSortedUsers();
            std::cout << "\nAll Users (Sorted by Email):\n";
            for (const auto& email : users) {
                std::cout << "- " << email << "\n";
            }
        } else if (choice == 2) {
            auto users = core.getRecentUsers();
            std::cout << "\nRecent Users:\n";
            for (const auto& email : users) {
                std::cout << "- " << email << "\n";
            }
        } else if (choice == 3) {
            std::cout << "Enter email prefix: ";
            std::string prefix;
            std::getline(std::cin, prefix);
            auto users = core.searchUsersByPrefix(prefix);
            std::cout << "\nMatching Users:\n";
            for (const auto& email : users) {
                std::cout << "- " << email << "\n";
            }
        }
        
        if (choice >= 1 && choice <= 3) {
            pause();
        }
    }
};

}