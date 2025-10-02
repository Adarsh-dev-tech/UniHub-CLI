/*
    subjects.h

    This header file defines structures and interfaces for managing academic subjects
    and their associated resources in the UniHub-CLI application. It provides the
    Subject structure to represent subject metadata, a fixed list of resource types,
    and functions for retrieving subjects and building resource directory names.

    These interfaces are used to organize and access subject-specific resources
    such as notes, assignments, and reference materials for students and teachers.
*/

#pragma once // Ensures this header is included only once during compilation

#include <string>      // Provides std::string for string handling
#include <vector>      // Provides std::vector for dynamic arrays
#include <map>         // Provides std::map for associative containers (may be used in implementation)

using namespace std;   // Allows usage of standard library types without std:: prefix

namespace uni {        // Encapsulates all declarations within the 'uni' namespace

// Structure representing metadata for an academic subject
struct Subject {
    string code;      // Subject code (e.g., CSE101)
    string name;      // Subject name (e.g., Data Structures)
    string teacher;   // Teacher's name (varies by section)
};

// Fixed list of resource types associated with subjects
inline const vector<string> kResourceTypes = {
    "Notes",           // Lecture notes
    "Assignments",     // Assignment files
    "PPTs",            // Presentation slides
    "EndSemPapers",    // End semester exam papers
    "CTs",             // Class tests
    "MidSemPapers",    // Mid semester exam papers
    "YouTubeLinks",    // Links to YouTube resources
    "ReferenceBooks"   // Reference books
};

// Returns a list of subjects for the specified year, semester, branch, and section
vector<Subject> getSubjects(int year, int semester, const string& branch, char section);

// Builds and returns the directory name for storing resources of a specific subject and type
string resourcesBase(int year, int semester, const string& branch, char section, const string& subjectName, const string& type);

} // namespace uni
