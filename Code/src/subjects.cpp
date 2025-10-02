/*
    subjects.cpp

    This source file implements subject and curriculum management for the UniHub-CLI application.
    It provides functions to retrieve subjects for a given year, semester, branch, and section,
    generate teacher names, and construct resource directory paths. The file contains subject pools
    for each branch and supports custom curriculum overrides for specific cases.
*/

#include "subjects.h"     // Include subject/resource type interface
#include "storage.h"      // Include storage utility functions
#include <map>            // Include map container for subject pools
#include <vector>         // Include vector container for lists
#include <string>         // Include string type
#include <sstream>        // Include string stream utilities
#include <algorithm>      // Include algorithms (e.g., transform for tolower)

using namespace std; // Allows usage of standard library types without std:: prefix

namespace uni { // Begin namespace uni

// List of supported branches
static const vector<string> kBranches = {
    "CSE","ECE","EEE","ICE","ME","CHE","PROD","CIVIL","MME","ARCH"
};

// Returns a map of branch codes to their subject lists
static map<string, vector<string>> subjectPool() {
    return {
        // Each branch maps to a vector of subject names
        {"CSE", {"Programming Fundamentals","Data Structures","Algorithms","Computer Networks","Operating Systems","DBMS","Software Engineering","AI Basics","ML Intro","Compilers"}},
        {"ECE", {"Circuit Theory","Signals and Systems","Digital Electronics","Communication Systems","Microprocessors","VLSI Basics","Control Systems","Embedded Systems","Antennas","DSP"}},
        {"EEE", {"Electrical Machines","Power Systems","Power Electronics","Control Systems","Measurements","Switchgear","Renewable Energy","HV Engineering","Microgrids","Drives"}},
        {"ICE", {"Sensors","Transducers","Process Control","Industrial Instrumentation","Biomedical","Analytical Instruments","Control Theory","Automation","Robotics","PLC"}},
        {"ME", {"Engineering Mechanics","Thermodynamics","Manufacturing","Fluid Mechanics","Heat Transfer","Design of Machines","IC Engines","Refrigeration","Dynamics","CAD/CAM"}},
        {"CHE", {"Material Balance","Energy Balance","Fluid Operations","Heat Operations","Mass Transfer","Chemical Reaction Engg","Process Control","Plant Design","Bioprocess","Polymer Tech"}},
        {"PROD", {"Foundry","Welding","Metrology","Manufacturing Systems","Operations Research","CIM","Quality Control","Supply Chain","Maintenance","Ergonomics"}},
        {"CIVIL", {"Surveying","Strength of Materials","Structural Analysis","Geotechnical","Transportation","Hydraulics","Environmental","Construction","Irrigation","Estimating"}},
        {"MME", {"Physical Metallurgy","Mechanical Metallurgy","Extractive","Phase Transformations","Materials Characterization","Welding Metallurgy","Powder Metallurgy","Corrosion","Nanomaterials","Heat Treatment"}},
        {"ARCH", {"Design Studio","Building Materials","History of Architecture","Structures","Climatology","Urban Design","Landscape","Housing","Conservation","Professional Practice"}}
    };
}

// Generates a pseudo teacher name for a subject based on branch, year, semester, section, and subject index
static string teacherFor(const string& /*branch*/, int year, int sem, char section, int idx) {
    // Deterministic pseudo teacher names based on inputs
    vector<string> first = {"A.","B.","C.","D.","E.","F."}; // List of first name initials
    vector<string> last = {"Raman","Iyer","Sharma","Gupta","Natarajan","Srinivasan","Kumar","Reddy"}; // List of last names
    int k = (year*13 + sem*7 + (section=='A'?1:2)*11 + idx*5) % (int)last.size(); // Calculate index for last name
    return string("Prof. ") + last[k] + " (" + (section=='A'?"Sec A":"Sec B") + ")"; // Build teacher name string
}

// Returns a list of subjects for the given year, semester, branch, and section
vector<Subject> getSubjects(int year, int semester, const string& branch, char section) {
    // Custom curriculum override for CSE Year-2 Semester-3 Section-B
    if ((branch == "CSE") && (year == 2) && (semester == 3) && (section == 'B' || section == 'b')) {
        vector<Subject> out; // Output vector
        const vector<pair<string,string>> custom = {
            {"computer organization", "Prof. Mala"},
            {"principles of programming languages", "Prof. Bala"},
            {"Data Structures", "Prof. Oswald"},
            {"Probability and operations research", "Prof. Shivaranjini"},
            {"Combinatorics and graph theory", "Prof. Pavan"},
            {"digital systems design", "Prof. Shameedha"}
        };
        for (int i = 0; i < (int)custom.size(); ++i) { // Iterate over custom subjects
            Subject s; // Create subject
            s.code = branch + to_string(year) + to_string(semester) + char('A' + i); // Generate subject code
            s.name = custom[i].first; // Set subject name
            s.teacher = custom[i].second; // Set teacher name
            out.push_back(move(s)); // Add subject to output
        }
        return out; // Return custom subjects
    }

    auto pool = subjectPool(); // Get subject pool map
    vector<Subject> out; // Output vector
    auto it = pool.find(branch); // Find branch in pool
    if (it == pool.end()) return out; // Return empty if branch not found
    const auto& names = it->second; // Get subject names for branch
    // Choose 5 subjects per semester
    for (int i = 0; i < 5 && i < (int)names.size(); ++i) {
        Subject s; // Create subject
        s.code = branch + std::to_string(year) + std::to_string(semester) + char('A'+i); // Generate subject code
        s.name = names[(semester*3 + i) % names.size()]; // Select subject name based on semester and index
        s.teacher = teacherFor(branch, year, semester, section, i); // Generate teacher name
        out.push_back(s); // Add subject to output
    }
    return out; // Return subjects
}

// Builds and returns the directory path for storing resources of a specific subject and type
string resourcesBase(int year, int semester, const string& branch, char section, const string& subjectName, const string& type) {
    string base = resourcesDir() + "/" + to_string(year) + "/" + to_string(semester) + "/" + branch + "/" + section + "/" + subjectName + "/" + type; // Build directory path
    ensureDir(base); // Ensure directory exists
    return base; // Return directory path
}

} // End namespace uni
