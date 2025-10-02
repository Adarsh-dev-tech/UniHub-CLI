/*
    menu.cpp

    This source file implements the main menu system and user interaction logic
    for the UniHub-CLI application. It provides functions for displaying and
    editing user profiles, browsing subjects and resources, handling registration
    and login flows, and managing the overall application workflow. The menu-driven
    interface guides users through authentication, profile management, and resource
    access in a structured manner.
*/

#include "menu.h"         // Includes the menu interface definitions
#include "auth.h"         // Includes authentication and profile management functions
#include "subjects.h"     // Includes subject and resource type definitions
#include "resources.h"    // Includes resource management functions
#include "storage.h"      // Includes file and directory utility functions
#include <iostream>       // Provides input/output stream operations
#include <filesystem>     // Provides file system operations

using namespace std; // Allows usage of standard library types without std:: prefix

namespace uni { // Begin namespace uni

// Displays the user's profile information
static void showProfile(Profile& p) {
    cout << "\nProfile:\n";
    cout << "Name: " << p.firstName << " " << p.lastName << "\n";
    cout << "Email: " << p.email << "\n";
    cout << "Year/Sem: " << p.year << "/" << p.semester << "\n";
    cout << "Branch: " << p.branch << "  Section: " << p.section << "\n";
}

// Allows the user to edit their profile fields interactively
static void editProfile(Profile& p) {
    cout << "\n-- Edit Profile (leave blank to keep) --\n";
    string in;
    cout << "First name [" << p.firstName << "]: "; getline(cin, in); if(!in.empty()) p.firstName=in;
    cout << "Last name [" << p.lastName << "]: "; getline(cin, in); if(!in.empty()) p.lastName=in;
    cout << "Year [" << p.year << "]: "; getline(cin, in); if(!in.empty()) p.year=stoi(in);
    cout << "Semester [" << p.semester << "]: "; getline(cin, in); if(!in.empty()) p.semester=stoi(in);
    cout << "Branch [" << p.branch << "]: "; getline(cin, in); if(!in.empty()) p.branch=in;
    cout << "Section [" << p.section << "]: "; getline(cin, in); if(!in.empty()) p.section=in[0];
    if (auto err = saveProfile(p)) {
        cout << "Save error: " << *err << "\n";
    } else {
        cout << "Profile updated.\n";
    }
}

// Allows the user to browse subjects and their associated resources
static void browseResources(const Profile& p) {
    auto subs = getSubjects(p.year, p.semester, p.branch, p.section); // Get subjects for the user's profile
    if (subs.empty()) { cout << "No subjects found.\n"; return; }
    while (true) {
        cout << "\n-- Subjects --\n";
        for (size_t i=0;i<subs.size();++i) {
            cout << (i+1) << ") " << subs[i].name << " [" << subs[i].teacher << "]\n";
        }
        cout << "0) Back\nChoose: ";
        int c; if(!(cin>>c)){cin.clear(); cin.ignore(10000,'\n'); continue;} cin.ignore(1,'\n');
        if (c==0) break;
        if (c<1 || c>(int)subs.size()) continue;
        const auto subj = subs[c-1];
        while (true) {
            cout << "\n-- Resource Types for " << subj.name << " --\n";
            for (size_t i=0;i<kResourceTypes.size();++i) cout << (i+1) << ") " << kResourceTypes[i] << "\n";
            cout << "0) Back\nChoose: ";
            int t; if(!(cin>>t)){cin.clear(); cin.ignore(10000,'\n'); continue;} cin.ignore(1,'\n');
            if (t==0) break;
            if (t<1 || t>(int)kResourceTypes.size()) continue;
            const string type = kResourceTypes[t-1];
            string folder = resourcesBase(p.year, p.semester, p.branch, p.section, subj.name, type);
            ensureDir(folder); // Ensure resource folder exists
            while (true) {
                auto items = listResources(folder); // List resources in the folder
                cout << "\n-- Resources (" << type << ") --\n";
                for (size_t i=0;i<items.size();++i) cout << (i+1) << ") " << items[i].displayName << " (" << items[i].sizeBytes << " bytes)\n";
                cout << "a) Upload  d) Download  r) Refresh  b) Back\nChoose: ";
                string op; getline(cin, op);
                if (op=="b") break;
                if (op=="r") continue;
                if (op=="a") {
                    string path; cout << "Local file path to upload: "; getline(cin, path);
                    auto [ok,msg] = uploadResource(path, folder); // Upload resource
                    cout << (ok?"Uploaded to ":"Failed: ") << msg << "\n";
                } else if (op=="d") {
                    if (items.empty()) { cout << "No files.\n"; continue; }
                    cout << "Index to download (1-" << items.size() << "): ";
                    int idx; if(!(cin>>idx)){cin.clear(); cin.ignore(10000,'\n'); continue;} cin.ignore(1,'\n');
                    if (idx<1 || idx>(int)items.size()) continue;
                    string dest; cout << "Destination path: "; getline(cin, dest);
                    auto [ok,msg] = downloadResource(items[idx-1].filename, dest); // Download resource
                    cout << (ok?"Downloaded to ":"Failed: ") << msg << "\n";
                }
            }
        }
    }
}

// Displays the main menu and handles user choices
static void mainMenu(Profile& profile) {
    while (true) {
        cout << "\n===== UniHub =====\n";
        cout << "1) Profile\n2) Subjects & Resources\n0) Logout\nChoose: ";
        int c; if(!(cin>>c)){cin.clear(); cin.ignore(10000,'\n'); continue;} cin.ignore(1,'\n');
        if (c==0) break;
        if (c==1) {
            showProfile(profile); // Show profile details
            cout << "e) Edit  b) Back\n";
            string op; getline(cin, op);
            if (op=="e") editProfile(profile); // Edit profile if chosen
        } else if (c==2) {
            browseResources(profile); // Browse subjects and resources
        }
    }
}

// Handles the user registration flow
static bool registerFlow() {
    Profile p; string password, confirm;
    cout << "First name: "; getline(cin, p.firstName);
    cout << "Last name: "; getline(cin, p.lastName);
    cout << "College email: "; getline(cin, p.email);
    cout << "Password: "; getline(cin, password);
    cout << "Confirm password: "; getline(cin, confirm);
    if (password != confirm) { cout << "Passwords do not match.\n"; return false; }
    cout << "Year (1-5): "; cin >> p.year; cin.ignore(1,'\n');
    cout << "Semester (1-10): "; cin >> p.semester; cin.ignore(1,'\n');
    cout << "Branch (full name or code): "; getline(cin, p.branch);
    {
        string b = p.branch; for (auto& c : b) c = toupper(c);
        // Normalize branch input to standard codes
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
            cout << "Unknown branch, defaulting to CSE.\n"; p.branch = "CSE";
        }
    }
    cout << "Section (A/B): "; string s; getline(cin, s); p.section = s.empty()? 'A' : s[0];
    if (auto err = registerUser(p, password)) { cout << *err << "\n"; return false; }
    cout << "Registered successfully.\n"; return true;
}

// Handles the user login flow
static optional<Profile> loginFlow() {
    string email, password;
    cout << "Email: "; getline(cin, email);
    cout << "Password: "; getline(cin, password);
    auto u = login(email, password); // Attempt login
    if (!u) { cout << "Invalid credentials.\n"; return nullopt; }
    return u->profile;
}

// Runs the main application loop and menu system
void runApp() {
    ensureDir(dataDir());      // Ensure data directory exists
    ensureDir(usersDir());     // Ensure users directory exists
    ensureDir(resourcesDir()); // Ensure resources directory exists
    while (true) {
        cout << "\n===== Welcome to UniHub CLI =====\n";
        cout << "1) Login\n2) Register\n0) Exit\nChoose: ";
        int c; if(!(cin>>c)){cin.clear(); cin.ignore(10000,'\n'); continue;} cin.ignore(1,'\n');
        if (c==0) break;
        if (c==2) { registerFlow(); continue; }
        if (c==1) {
            auto prof = loginFlow();
            if (prof) {
                mainMenu(*prof); // Enter main menu if login successful
            }
        }
    }
    std::cout << "Goodbye!\n";
}

} // End namespace uni
