#pragma once
#include "subjects.h"
#include "data_structures.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <optional>
#include <algorithm>

namespace uni {

// ============================================================================
// Enhanced Academic Management with Tree + DAG
// ============================================================================

// Tree node for academic hierarchy
template<typename T>
struct TreeNode {
    T data;
    std::vector<std::shared_ptr<TreeNode<T>>> children;
    std::weak_ptr<TreeNode<T>> parent;
    
    TreeNode(const T& val) : data(val) {}
};

// Branch information
struct Branch {
    std::string code;
    std::string fullName;
    int maxYears;
    
    Branch() : maxYears(4) {} // Default constructor
    Branch(const std::string& c, const std::string& fn, int my) 
        : code(c), fullName(fn), maxYears(my) {}
    
    bool operator==(const Branch& other) const { return code == other.code; }
};

// Enhanced Subject with prerequisites
struct EnhancedSubject {
    std::string code;
    std::string name;
    std::string teacher;
    int year;
    int semester;
    std::string branch;
    char section;
    std::vector<std::string> prerequisites;
    int credits;
    
    EnhancedSubject() : year(0), semester(0), section('A'), credits(3) {}
    
    bool operator==(const EnhancedSubject& other) const { return code == other.code; }
};

class AcademicManager {
private:
    // Tree: University hierarchy (University -> Branches -> Years -> Semesters)
    std::shared_ptr<TreeNode<std::string>> universityRoot;
    
    // DAG: Subject prerequisites
    DAG<std::string> prerequisiteGraph;
    
    // Hash Map: Quick subject lookup
    std::unordered_map<std::string, EnhancedSubject> subjectMap;
    
    // Branch information
    std::unordered_map<std::string, Branch> branches;
    
    void initializeBranches() {
        branches["CSE"] = Branch("CSE", "Computer Science and Engineering", 4);
        branches["ECE"] = Branch("ECE", "Electronics and Communication Engineering", 4);
        branches["EEE"] = Branch("EEE", "Electrical and Electronics Engineering", 4);
        branches["ICE"] = Branch("ICE", "Instrumentation and Control Engineering", 4);
        branches["ME"] = Branch("ME", "Mechanical Engineering", 4);
        branches["CHE"] = Branch("CHE", "Chemical Engineering", 4);
        branches["PROD"] = Branch("PROD", "Production Engineering", 4);
        branches["CIVIL"] = Branch("CIVIL", "Civil Engineering", 4);
        branches["MME"] = Branch("MME", "Metallurgical and Materials Engineering", 4);
        branches["ARCH"] = Branch("ARCH", "Architecture", 5);
    }
    
    void initializeCSECurriculum() {
        // Year 1 Semester 1
        addSubject("CSE11A", "Programming Fundamentals", "Prof. Kumar", 1, 1, "CSE", 'A', {}, 4);
        addSubject("CSE11B", "Mathematics I", "Prof. Iyer", 1, 1, "CSE", 'A', {}, 4);
        addSubject("CSE11C", "Physics", "Prof. Sharma", 1, 1, "CSE", 'A', {}, 3);
        addSubject("CSE11D", "English", "Prof. Gupta", 1, 1, "CSE", 'A', {}, 3);
        addSubject("CSE11E", "Basic Electronics", "Prof. Reddy", 1, 1, "CSE", 'A', {}, 3);
        
        // Year 1 Semester 2
        addSubject("CSE12A", "Object Oriented Programming", "Prof. Natarajan", 1, 2, "CSE", 'A', {"CSE11A"}, 4);
        addSubject("CSE12B", "Mathematics II", "Prof. Srinivasan", 1, 2, "CSE", 'A', {"CSE11B"}, 4);
        addSubject("CSE12C", "Chemistry", "Prof. Raman", 1, 2, "CSE", 'A', {}, 3);
        addSubject("CSE12D", "Engineering Graphics", "Prof. Kumar", 1, 2, "CSE", 'A', {}, 3);
        addSubject("CSE12E", "Digital Logic", "Prof. Iyer", 1, 2, "CSE", 'A', {"CSE11E"}, 3);
        
        // Year 2 Semester 3 - Custom curriculum for Section B
        addSubject("CSE23A", "Computer Organization", "Prof. Mala", 2, 3, "CSE", 'B', {"CSE12E"}, 4);
        addSubject("CSE23B", "Principles of Programming Languages", "Prof. Bala", 2, 3, "CSE", 'B', {"CSE12A"}, 3);
        addSubject("CSE23C", "Data Structures", "Prof. Oswald", 2, 3, "CSE", 'B', {"CSE12A"}, 4);
        addSubject("CSE23D", "Probability and Operations Research", "Prof. Shivaranjini", 2, 3, "CSE", 'B', {"CSE12B"}, 3);
        addSubject("CSE23E", "Combinatorics and Graph Theory", "Prof. Pavan", 2, 3, "CSE", 'B', {"CSE12B"}, 3);
        addSubject("CSE23F", "Digital Systems Design", "Prof. Shameedha", 2, 3, "CSE", 'B', {"CSE12E"}, 4);
        
        // Year 2 Semester 4
        addSubject("CSE24A", "Algorithms", "Prof. Sharma", 2, 4, "CSE", 'A', {"CSE23C"}, 4);
        addSubject("CSE24B", "Computer Networks", "Prof. Gupta", 2, 4, "CSE", 'A', {"CSE23A"}, 4);
        addSubject("CSE24C", "Database Systems", "Prof. Reddy", 2, 4, "CSE", 'A', {"CSE23C"}, 4);
        addSubject("CSE24D", "Software Engineering", "Prof. Natarajan", 2, 4, "CSE", 'A', {"CSE23B"}, 3);
        addSubject("CSE24E", "Operating Systems", "Prof. Srinivasan", 2, 4, "CSE", 'A', {"CSE23A"}, 4);
    }

public:
    AcademicManager() {
        universityRoot = std::make_shared<TreeNode<std::string>>("NIT Trichy");
        initializeBranches();
        initializeCSECurriculum();
    }
    
    void addSubject(const std::string& code, const std::string& name, const std::string& teacher,
                   int year, int semester, const std::string& branch, char section,
                   const std::vector<std::string>& prerequisites, int credits) {
        EnhancedSubject subject;
        subject.code = code;
        subject.name = name;
        subject.teacher = teacher;
        subject.year = year;
        subject.semester = semester;
        subject.branch = branch;
        subject.section = section;
        subject.prerequisites = prerequisites;
        subject.credits = credits;
        
        subjectMap[code] = subject;
        prerequisiteGraph.addNode(code);
        
        for (const auto& prereq : prerequisites) {
            prerequisiteGraph.addEdge(prereq, code);
        }
    }
    
    std::vector<EnhancedSubject> getSubjects(int year, int semester, const std::string& branch, char section) {
        std::vector<EnhancedSubject> result;
        
        for (const auto& [code, subject] : subjectMap) {
            if (subject.year == year && subject.semester == semester && 
                subject.branch == branch && subject.section == section) {
                result.push_back(subject);
            }
        }
        
        return result;
    }
    
    std::vector<std::string> getPrerequisites(const std::string& subjectCode) {
        return prerequisiteGraph.getPrerequisites(subjectCode);
    }
    
    std::vector<std::string> getSuggestedCourseSequence() {
        return prerequisiteGraph.topologicalSort();
    }
    
    bool canTakeSubject(const std::string& subjectCode, const std::vector<std::string>& completedSubjects) {
        auto prerequisites = getPrerequisites(subjectCode);
        
        for (const auto& prereq : prerequisites) {
            if (std::find(completedSubjects.begin(), completedSubjects.end(), prereq) == completedSubjects.end()) {
                return false;
            }
        }
        
        return true;
    }
    
    std::vector<std::string> getAvailableBranches() {
        std::vector<std::string> branchCodes;
        for (const auto& [code, branch] : branches) {
            branchCodes.push_back(code);
        }
        return branchCodes;
    }
    
    std::optional<Branch> getBranch(const std::string& code) {
        auto it = branches.find(code);
        if (it != branches.end()) {
            return it->second;
        }
        return std::nullopt;
    }
    
    std::optional<EnhancedSubject> getSubject(const std::string& code) {
        auto it = subjectMap.find(code);
        if (it != subjectMap.end()) {
            return it->second;
        }
        return std::nullopt;
    }
};

}