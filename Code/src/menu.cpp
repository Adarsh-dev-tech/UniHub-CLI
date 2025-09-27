#include "menu.h"
#include "auth.h"
#include "subjects.h"
#include "resources.h"
#include "storage.h"
#include <iostream>
#include <filesystem>

namespace uni {

static void showProfile(Profile& p) {
    std::cout << "\nProfile:\n";
    std::cout << "Name: " << p.firstName << " " << p.lastName << "\n";
    std::cout << "Email: " << p.email << "\n";
    std::cout << "Year/Sem: " << p.year << "/" << p.semester << "\n";
    std::cout << "Branch: " << p.branch << "  Section: " << p.section << "\n";
}

static void editProfile(Profile& p) {
    std::cout << "\n-- Edit Profile (leave blank to keep) --\n";
    std::string in;
    std::cout << "First name [" << p.firstName << "]: "; std::getline(std::cin, in); if(!in.empty()) p.firstName=in;
    std::cout << "Last name [" << p.lastName << "]: "; std::getline(std::cin, in); if(!in.empty()) p.lastName=in;
    std::cout << "Year [" << p.year << "]: "; std::getline(std::cin, in); if(!in.empty()) p.year=std::stoi(in);
    std::cout << "Semester [" << p.semester << "]: "; std::getline(std::cin, in); if(!in.empty()) p.semester=std::stoi(in);
    std::cout << "Branch [" << p.branch << "]: "; std::getline(std::cin, in); if(!in.empty()) p.branch=in;
    std::cout << "Section [" << p.section << "]: "; std::getline(std::cin, in); if(!in.empty()) p.section=in[0];
    if (auto err = saveProfile(p)) {
        std::cout << "Save error: " << *err << "\n";
    } else {
        std::cout << "Profile updated.\n";
    }
}

static void browseResources(const Profile& p) {
    auto subs = getSubjects(p.year, p.semester, p.branch, p.section);
    if (subs.empty()) { std::cout << "No subjects found.\n"; return; }
    while (true) {
        std::cout << "\n-- Subjects --\n";
        for (size_t i=0;i<subs.size();++i) {
            std::cout << (i+1) << ") " << subs[i].name << " [" << subs[i].teacher << "]\n";
        }
        std::cout << "0) Back\nChoose: ";
        int c; if(!(std::cin>>c)){std::cin.clear(); std::cin.ignore(10000,'\n'); continue;} std::cin.ignore(1,'\n');
    if (c==0) break;
    if (c<1 || c>(int)subs.size()) continue;
        const auto subj = subs[c-1];
        while (true) {
            std::cout << "\n-- Resource Types for " << subj.name << " --\n";
            for (size_t i=0;i<kResourceTypes.size();++i) std::cout << (i+1) << ") " << kResourceTypes[i] << "\n";
            std::cout << "0) Back\nChoose: ";
            int t; if(!(std::cin>>t)){std::cin.clear(); std::cin.ignore(10000,'\n'); continue;} std::cin.ignore(1,'\n');
            if (t==0) break;
            if (t<1 || t>(int)kResourceTypes.size()) continue;
            const std::string type = kResourceTypes[t-1];
            std::string folder = resourcesBase(p.year, p.semester, p.branch, p.section, subj.name, type);
            ensureDir(folder);
            while (true) {
                auto items = listResources(folder);
                std::cout << "\n-- Resources (" << type << ") --\n";
                for (size_t i=0;i<items.size();++i) std::cout << (i+1) << ") " << items[i].displayName << " (" << items[i].sizeBytes << " bytes)\n";
                std::cout << "a) Upload  d) Download  r) Refresh  b) Back\nChoose: ";
                std::string op; std::getline(std::cin, op);
                if (op=="b") break;
                if (op=="r") continue;
                if (op=="a") {
                    std::string path; std::cout << "Local file path to upload: "; std::getline(std::cin, path);
                    auto [ok,msg] = uploadResource(path, folder);
                    std::cout << (ok?"Uploaded to ":"Failed: ") << msg << "\n";
                } else if (op=="d") {
                    if (items.empty()) { std::cout << "No files.\n"; continue; }
                    std::cout << "Index to download (1-" << items.size() << "): ";
                    int idx; if(!(std::cin>>idx)){std::cin.clear(); std::cin.ignore(10000,'\n'); continue;} std::cin.ignore(1,'\n');
                    if (idx<1 || idx>(int)items.size()) continue;
                    std::string dest; std::cout << "Destination path: "; std::getline(std::cin, dest);
                    auto [ok,msg] = downloadResource(items[idx-1].filename, dest);
                    std::cout << (ok?"Downloaded to ":"Failed: ") << msg << "\n";
                }
            }
        }
    }
}

static void mainMenu(Profile& profile) {
    while (true) {
        std::cout << "\n===== UniHub =====\n";
        std::cout << "1) Profile\n2) Subjects & Resources\n0) Logout\nChoose: ";
        int c; if(!(std::cin>>c)){std::cin.clear(); std::cin.ignore(10000,'\n'); continue;} std::cin.ignore(1,'\n');
        if (c==0) break;
        if (c==1) {
            showProfile(profile);
            std::cout << "e) Edit  b) Back\n";
            std::string op; std::getline(std::cin, op);
            if (op=="e") editProfile(profile);
        } else if (c==2) {
            browseResources(profile);
        }
    }
}

static bool registerFlow() {
    Profile p; std::string password, confirm;
    std::cout << "First name: "; std::getline(std::cin, p.firstName);
    std::cout << "Last name: "; std::getline(std::cin, p.lastName);
    std::cout << "College email: "; std::getline(std::cin, p.email);
    std::cout << "Password: "; std::getline(std::cin, password);
    std::cout << "Confirm password: "; std::getline(std::cin, confirm);
    if (password != confirm) { std::cout << "Passwords do not match.\n"; return false; }
    std::cout << "Year (1-5): "; std::cin >> p.year; std::cin.ignore(1,'\n');
    std::cout << "Semester (1-10): "; std::cin >> p.semester; std::cin.ignore(1,'\n');
    std::cout << "Branch (full name or code): "; std::getline(std::cin, p.branch);
    {
        std::string b = p.branch; for (auto& c : b) c = toupper(c);
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
            std::cout << "Unknown branch, defaulting to CSE.\n"; p.branch = "CSE";
        }
    }
    std::cout << "Section (A/B): "; std::string s; std::getline(std::cin, s); p.section = s.empty()? 'A' : s[0];
    if (auto err = registerUser(p, password)) { std::cout << *err << "\n"; return false; }
    std::cout << "Registered successfully.\n"; return true;
}

static std::optional<Profile> loginFlow() {
    std::string email, password;
    std::cout << "Email: "; std::getline(std::cin, email);
    std::cout << "Password: "; std::getline(std::cin, password);
    auto u = login(email, password);
    if (!u) { std::cout << "Invalid credentials.\n"; return std::nullopt; }
    return u->profile;
}

void runApp() {
    ensureDir(dataDir());
    ensureDir(usersDir());
    ensureDir(resourcesDir());
    while (true) {
        std::cout << "\n===== Welcome to UniHub CLI =====\n";
        std::cout << "1) Login\n2) Register\n0) Exit\nChoose: ";
        int c; if(!(std::cin>>c)){std::cin.clear(); std::cin.ignore(10000,'\n'); continue;} std::cin.ignore(1,'\n');
        if (c==0) break;
        if (c==2) { registerFlow(); continue; }
        if (c==1) {
            auto prof = loginFlow();
            if (prof) {
                mainMenu(*prof);
            }
        }
    }
    std::cout << "Goodbye!\n";
}

}
