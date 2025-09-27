#pragma once
#include <string>
#include <vector>
#include <map>

namespace uni {

struct Subject {
    std::string code;   // e.g., CSE101
    std::string name;   // e.g., Data Structures
    std::string teacher; // depends on section
};

// Resource types fixed list
inline const std::vector<std::string> kResourceTypes = {
    "Notes","Assignments","PPTs","EndSemPapers","CTs","MidSemPapers","YouTubeLinks","ReferenceBooks"
};

// Return list of subjects for year, semester, branch, section
std::vector<Subject> getSubjects(int year, int semester, const std::string& branch, char section);

// For convenience: directory name builder for resources
std::string resourcesBase(int year, int semester, const std::string& branch, char section, const std::string& subjectName, const std::string& type);

}
